/**
  * @file    app.c
  * @brief   Application Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_H
#define APP_H

#include "app_trace.h"


/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
//#include "stm32373c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
//#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
//#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's DMA */

//#define USARTx_TX_DMA_CHANNEL             DMA1_Channel4
//#define USARTx_RX_DMA_CHANNEL             DMA1_Channel5



/* Definition for USARTx's NVIC */
//#define USARTx_DMA_TX_IRQn                DMA1_Channel4_IRQn
//#define USARTx_DMA_RX_IRQn                DMA1_Channel5_IRQn
//#define USARTx_DMA_TX_IRQHandler          DMA1_Channel4_IRQHandler
//#define USARTx_DMA_RX_IRQHandler          DMA1_Channel5_IRQHandler

/* Definition for USARTx's NVIC */
//#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE               (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (COUNTOF(aTxEndMessage) - 1)

/* Size of Reception buffer */
#define RXBUFFERSIZE                      64

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


static
void    app_error(                      const   char *          file,
                                        const   int             line )
{
        #if defined( NDEBUG )
        bsp_system_reset();
        #else
        APP_TRACE( "error: line %d @ %s\n", line, file );
        while( 1 );
        #endif
}


void    app_clock_config( void );

void    app_sens_init( void );

void    app_sens_run( void );


#endif  //APP_H
