/*
******************************************************************************
**

**  File        : LinkerScript.ld
**
**
**
**  Abstract    : Linker script for STM32H7 series
**                131072Kbytes FLASH and 192Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** Copyright (c) 2019 STMicroelectronics.
** All rights reserved.
**
** This software is licensed under terms that can be found in the LICENSE file
** in the root directory of this software component.
** If no LICENSE file comes with this software, it is provided AS-IS.
**
******************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20020000;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x1000;      /* required amount of heap  */
_Min_Stack_Size = 0x1000; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
FLASH (rx)           : ORIGIN = 0x90040000, LENGTH = 4K
ROM_STEW (rx)        : ORIGIN = 0x90041000, LENGTH = 764K
RAM (xrw)            : ORIGIN = 0x24000000, LENGTH = 512K
DTCMRAM (xrw)        : ORIGIN = 0x20000000, LENGTH = 128K
SDRAM (rw)           : ORIGIN = 0xD0000000, LENGTH = 8192K
QSPI_STEW (rx)       : ORIGIN = 0x92000000, LENGTH = 32768K
}

/* Define output sections */
SECTIONS
{

  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  .IntThirdPartyHeaderSection : 
   {   . = ALIGN(4);
   KEEP(*(.IntThirdPartyHeaderSection)) 
  . = ALIGN(4);
  } >ROM_STEW
  

_siQSPI = LOADADDR(.QSPISection);

  .ExtThirdPartyHeaderSection : 
   {   . = ALIGN(4);
   KEEP(*(.ExtThirdPartyHeaderSection)) 
  . = ALIGN(4);
  } >QSPI_STEW

 /* Initialized data sections goes into RAM, load LMA copy after code */
  .QSPISection : {
    . = ALIGN(4);
    _sQSPI = .;        /* create a global symbol at data start */
    
   *FComic24B_ASCII.o (.rodata .rodata*)
   *F13B_1.o (.rodata .rodata*)
   *F13B_ASCII.o (.rodata .rodata*)
   *F13_1.o (.rodata .rodata*)
   *F13_ASCII.o (.rodata .rodata*)
   *F16B_1.o (.rodata .rodata*)
   *F16B_ASCII.o (.rodata .rodata*)
   *F16_ASCII.o (.rodata .rodata*)
   *Arial18.o (.rodata .rodata*)
   *ArialRoundedBold18.o (.rodata .rodata*)
   *ArialRoundedBold20.o (.rodata .rodata*)
   *ArialRoundedBold24.o (.rodata .rodata*)
   *F32B_ASCII.o (.rodata .rodata*)
    _eQSPI = .;          
 } >QSPI_STEW 
 

  
  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >ROM_STEW

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >ROM_STEW

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ROM_STEW
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >ROM_STEW

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >ROM_STEW
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >ROM_STEW
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >ROM_STEW

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> ROM_STEW

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCMRAM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCMRAM
  
demo_icon_section :
  {
    demo_icon = .;
    KEEP(*(demo_icon_section));
    demo_icon_end = .;
  } >QSPI_STEW
 
  demo_logo_section :
  {
    demo_logo = .;
    KEEP(*(demo_logo_section ));
    demo_logo_end = .;
  } >QSPI_STEW
  
  .ExtQSPIFlashSection  :  
   {   . = ALIGN(4);
   KEEP(*(.ExtQSPIFlashSection)) 
  . = ALIGN(4);
  } >QSPI_STEW
  
  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

    .ARM.attributes 0    : { *(.ARM.attributes) }
   .STemWinMemPool     : { *(.STemWinMemPool) } >RAM
    heap_mem            : { *(heap_mem) } >RAM
   .audio_buffers      : { *(.audio_buffers) } >RAM
   .avi_video_buffers  : { *(.avi_video_buffers) } >RAM
  .sdio_heap            : { *(.sdio_heap) } >DTCMRAM
  .framebuffer  (NOLOAD) : { *(.framebuffer) } >SDRAM
  .dma2d_palette (NOLOAD) : { *(.dma2d_palette) } >SDRAM
  .rgb_video_buffers (NOLOAD) : { *(.rgb_video_buffers) } >SDRAM
  .gfx_buffers  (NOLOAD) : { *(.gfx_buffers) } >SDRAM
  .viz_back_buffer (NOLOAD) : { *(.viz_back_buffer) } >SDRAM
  .viz_bitmap_buffer  (NOLOAD) : { *(.viz_bitmap_buffer) } >SDRAM
  .viz_deform_x_buffer  (NOLOAD) : { *(.viz_deform_x_buffer) } >SDRAM
  .viz_deform_y_buffer  (NOLOAD) : { *(.viz_deform_y_buffer) } >SDRAM
  .viz_front_buffer  (NOLOAD) : { *(.viz_front_buffer) } >SDRAM
}


