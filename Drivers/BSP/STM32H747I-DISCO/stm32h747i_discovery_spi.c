
/* Includes ------------------------------------------------------------------*/
#include "stm32h747i_discovery_bus.h"
#include "stm32h747i_discovery_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCO
  * @{
  */

/** @defgroup STM32H747I_DISCO_BUS BUS
  * @{
  */


/** @defgroup STM32H747I_DISCO_BUS_Private_Constants Private Constants
  * @{
  */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
static uint32_t IsI2c4MspCbValid = 0;
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
static uint32_t Spi2InitCounter = 0;
#if defined(BSP_USE_CMSIS_OS)
static osSemaphoreId BspSpi2Semaphore = NULL;
#endif
/**
  * @}
  */
  */


/** @defgroup STM32H747I_DISCO_BUS_Exported_Variables Exported Variables
  * @{
  */
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi5;
/**
  * @}
  */

/** @defgroup STM32H747I_DISCO_BUS_Private_FunctionPrototypes Private FunctionPrototypes
  * @{
  */
static void SPI2_MspInit(SPI_HandleTypeDef *phspi);
static void SPI2_MspDeInit(SPI_HandleTypeDef *phspi);
static int32_t SPI2_WriteReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t SPI2_ReadReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static uint32_t SPI_GetTiming(uint32_t clock_src_freq, uint32_t spi_freq);
/**
  * @}
  */

/** @defgroup STM32H747I_DISCO_BUS_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Initializes I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C4_Init(void)
{
    int32_t ret = BSP_ERROR_NONE;

    hspi2.Instance = SPI_SPI2;

    if (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET)
    {
#if defined(BSP_USE_CMSIS_OS)
      if(BspSpi2Semaphore == NULL)
      {
        /* Create semaphore to prevent multiple I2C access */
        osSemaphoreDef(BSP_SPI2_SEM);
        BspSpi2Semaphore = osSemaphoreCreate(osSemaphore(BSP_SPI2_SEM), 1);
      }
#endif
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the I2C4 Msp */
      SPI2_MspInit(&hspi2);
#endif
      if (MX_SPI2_Init(&hbus_i2c4, I2C_GetTiming(HAL_RCC_GetPCLK2Freq(), BUS_I2C4_FREQUENCY)) != HAL_OK)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
    }
}

/**
  * @brief  LTDC Clock Config for LCD 2 DPI display.
  * @param  hltdc  LTDC Handle
  *         Being __weak it can be overwritten by the application
  * @retval HAL_status
  */
__weak HAL_StatusTypeDef MX_SPI2_ClockConfig(SPI_HandleTypeDef *phspi)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  UNUSED(phspi);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.PLL3.PLL3M      = 1U;
  PeriphClkInitStruct.PLL3.PLL3N      = 13U;
  PeriphClkInitStruct.PLL3.PLL3P      = 2U;
  PeriphClkInitStruct.PLL3.PLL3Q      = 2U;
  PeriphClkInitStruct.PLL3.PLL3R      = 12U;
  PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLLCFGR_PLL3RGE_2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN  = 0U;
  return HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

static void SPI2_MspInit(SPI_HandleTypeDef *phspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  SPI_SPI2_SCK_GPIO_CLK_ENABLE();
  SPI_SPI2_NSS_GPIO_CLK_ENABLE();
  SPI_SPI2_MISO_GPIO_CLK_ENABLE();
  SPI_SPI2_MOSI_GPIO_CLK_ENABLE();
  /**SPI2 GPIO Configuration
  PA12     ------> SPI2_SCK
  PA11     ------> SPI2_NSS
  PC2     ------> SPI2_MISO
  PC3     ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = SPI_SPI2_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = SPI_SPI2_SCK_AF;
  HAL_GPIO_Init(SPI_SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI_SPI2_NSS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = SPI_SPI2_NSS_AF;
  HAL_GPIO_Init(SPI_SPI2_NSS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI_SPI2_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = SPI_SPI2_MISO_AF;
  HAL_GPIO_Init(SPI_SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SPI_SPI2_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = SPI_SPI2_MOSI_AF;
  HAL_GPIO_Init(SPI_SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
  /* Enable SPI clock */
  SPI_SPI2_CLK_ENABLE();

  /* Force the SPI peripheral clock reset */
  SPI_SPI2_FORCE_RESET();

  /* Release the SPI peripheral clock reset */
  SPI_SPI2_RELEASE_RESET();
}


__weak HAL_StatusTypeDef MX_SPI2_Init(void)
{
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  return HAL_SPI_Init(&hspi2);
}

static void SPI5_MspInit(SPI_HandleTypeDef *phspi)
{
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI5;
  PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* SPI5 clock enable */
  __HAL_RCC_SPI5_CLK_ENABLE();

  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  /**SPI5 GPIO Configuration
  PK0     ------> SPI5_SCK
  PK1     ------> SPI5_NSS
  PJ11     ------> SPI5_MISO
  PJ10     ------> SPI5_MOSI
  */
  GPIO_InitStruct.Pin = ARD_D13_Pin|ARD_D10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ARD_D12_Pin|ARD_D11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

}