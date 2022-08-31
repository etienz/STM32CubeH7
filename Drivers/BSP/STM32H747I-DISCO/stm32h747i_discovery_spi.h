
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H747I_DISCO_SPI_H
#define STM32H747I_DISCO_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32h747i_discovery_conf.h"

#if defined(BSP_USE_CMSIS_OS)
#include "cmsis_os.h"
#endif
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCO
  * @{
  */

/** @addtogroup STM32H747I_DISCO_SPI
  * @{
  */

/** @defgroup STM32H747I_DISCO_SPI_Exported_Types Exported Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  void (* pMspInitCb)(I2C_HandleTypeDef *);
  void (* pMspDeInitCb)(I2C_HandleTypeDef *);
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
/**
  * @}
  */
/** @defgroup STM32H747I_DISCO_SPI_Exported_Constants Exported Constants
  * @{
  */
/* Definition for SPI2 clock resources */
#define SPI_SPI2                              SPI2
#define SPI_SPI2_CLK_ENABLE()                  __HAL_RCC_SPI2_CLK_ENABLE()
#define SPI_SPI2_CLK_DISABLE()                 __HAL_RCC_SPI2_CLK_DISABLE()

#define SPI_SPI2_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI_SPI2_SCK_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPI_SPI2_NSS_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI_SPI2_NSS_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()

#define SPI_SPI2_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI_SPI2_MISO_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()

#define SPI_SPI2_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI_SPI2_MOSI_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOC_CLK_DISABLE()

#define SPI_SPI2_FORCE_RESET()                 __HAL_RCC_SPI2_FORCE_RESET()
#define SPI_SPI2_RELEASE_RESET()               __HAL_RCC_SPI2_RELEASE_RESET()


#define SPI2_SCK_Pin GPIO_PIN_12
#define SPI2_SCK_GPIO_Port GPIOA
#define PMOD_1_Pin GPIO_PIN_11
#define PMOD_1_GPIO_Port GPIOA

/* Definition for SPI2 Pins */
#define SPI_SPI2_SCK_PIN                       GPIO_PIN_12
#define SPI_SPI2_NSS_PIN                       GPIO_PIN_11
#define SPI_SPI2_MISO_PIN                      GPIO_PIN_2
#define SPI_SPI2_MOSI_PIN                      GPIO_PIN_3
#define SPI_SPI2_SCK_GPIO_PORT                 GPIOA
#define SPI_SPI2_NSS_GPIO_PORT                 GPIOA
#define SPI_SPI2_MISO_GPIO_PORT                GPIOC
#define SPI_SPI2_MOSI_GPIO_PORT                GPIOC
#define SPI_SPI2_SCK_AF                        GPIO_AF5_SPI2
#define SPI_SPI2_NSS_AF                        GPIO_AF5_SPI2
#define SPI_SPI2_MISO_AF                       GPIO_AF5_SPI2
#define SPI_SPI2_MOSI_AF                       GPIO_AF5_SPI2

#ifndef SPI_SPI2_FREQUENCY
   #define SPI_SPI2_FREQUENCY  100000U /* Frequency of I2Cn = 100 KHz*/
#endif

/**
  * @}
  */

/** @addtogroup STM32H747I_DISCO_SPI_Exported_Variables
  * @{
  */
extern SPI_HandleTypeDef hspi_spi2;
/**
  * @}
  */

/** @addtogroup STM32H747I_DISCO_SPI_Exported_Functions
  * @{
  */
int32_t BSP_SPI2_Init(void);
int32_t BSP_SPI2_DeInit(void);
int32_t BSP_GetTick(void);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_SPI2_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI2_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_SPI2_Init(I2C_HandleTypeDef *phi2c, uint32_t timing);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H747I_DISCO_SPI_H */
