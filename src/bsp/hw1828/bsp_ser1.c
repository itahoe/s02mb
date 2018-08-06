/**
  * @file    bsp_ser1.c
  * @brief   Serial 1 port services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app_trace.h"
#include "bsp.h"
#include "bsp_ser.h"
#include "app.h"


static  UART_HandleTypeDef      huart;
static  DMA_HandleTypeDef       hdma_tx;
static  DMA_HandleTypeDef       hdma_rx;


static
void bsp_ser1_io_init( void )
{
        GPIO_InitTypeDef        pin_txd     =   {    .Pin       =    GPIO_PIN_9,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

        GPIO_InitTypeDef        pin_rxd     =   {    .Pin       =    GPIO_PIN_10,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

        GPIO_InitTypeDef        pin_dir     =   {    .Pin       =    GPIO_PIN_11,
	                                             .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };


        __HAL_RCC_GPIOA_CLK_ENABLE();

        HAL_GPIO_Init( GPIOA, &pin_txd          );
        HAL_GPIO_Init( GPIOA, &pin_rxd          );
        HAL_GPIO_Init( GPIOA, &pin_dir          );
}


static
void    bsp_ser1_dma_init( void )
{
        __HAL_RCC_DMA1_CLK_ENABLE();

        hdma_tx.Instance                 = DMA1_Channel4;
        hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_tx.Init.Mode                = DMA_NORMAL;
        hdma_tx.Init.Priority            = DMA_PRIORITY_MEDIUM;

        HAL_DMA_Init( &hdma_tx );
        __HAL_LINKDMA( &huart, hdmatx, hdma_tx );


        hdma_rx.Instance                 = DMA1_Channel5;
        hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_rx.Init.Mode                = DMA_NORMAL;
        hdma_rx.Init.Priority            = DMA_PRIORITY_MEDIUM;

        HAL_DMA_Init( &hdma_rx );
        __HAL_LINKDMA( &huart, hdmarx, hdma_rx );
}


static
void    bsp_ser1_uart_init(             const   size_t          baudrate    )
{
        huart.Instance                  = USART1;
        huart.Init.BaudRate             = baudrate;
        huart.Init.WordLength           = UART_WORDLENGTH_8B;
        huart.Init.StopBits             = UART_STOPBITS_1;
        huart.Init.Parity               = UART_PARITY_NONE;
        huart.Init.HwFlowCtl            = UART_HWCONTROL_NONE;
        huart.Init.Mode                 = UART_MODE_TX_RX;


        __HAL_RCC_USART1_CLK_ENABLE();

        if( HAL_UART_Init( &huart ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}


void    bsp_ser1_init(                  const   size_t          baud )
{
        bsp_ser1_uart_init( baud );
        bsp_ser1_io_init();
        bsp_ser1_dma_init();


        NVIC_SetPriority(       DMA1_Channel4_IRQn, BSP_NVIC_PRIO_SER1_DMA_TX   );
        NVIC_EnableIRQ(         DMA1_Channel4_IRQn                              );

        //NVIC_SetPriority(       DMA1_Channel5_IRQn, BSP_NVIC_PRIO_SER1_DMA_RX   );
        //NVIC_EnableIRQ(         DMA1_Channel5_IRQn                              );

        NVIC_SetPriority(       USART1_IRQn,        BSP_NVIC_PRIO_SER1_UART     );
        NVIC_EnableIRQ(         USART1_IRQn                                     );
}


static
void    bsp_ser1_xfer_dir_set(          bool            dir_tx  )
{
	HAL_GPIO_WritePin(      GPIOA,
                                GPIO_PIN_11,
                                dir_tx ? GPIO_PIN_SET : GPIO_PIN_RESET );
}


/******************************************************************************/
/* Transmit related                                                           */
/******************************************************************************/

bool bsp_ser1_xmit(                             uint8_t *               data,
                                                size_t                  size )
{
        HAL_StatusTypeDef       resp;


        bsp_ser1_xfer_dir_set( true );

        //__HAL_UART_ENABLE_IT( &huart, UART_IT_TC );

        resp    =   HAL_UART_Transmit_DMA( &huart, data, size );

        if( resp == HAL_OK )
        {
                while( HAL_UART_GetState( &huart ) != HAL_UART_STATE_READY );
        }

        //bsp_ser1_xfer_dir_set( false );


        return( false );
}


/******************************************************************************/
/* Receive related                                                            */
/******************************************************************************/

void bsp_ser1_recv_start(                       uint8_t *           data,
                                                size_t              size )
{
        bsp_ser1_xfer_dir_set( false );

        __HAL_UART_ENABLE_IT( &huart, UART_IT_IDLE );
        //HAL_UART_DMAStop( &huart );
        HAL_UART_Receive_DMA( &huart, data, size );

}


void bsp_ser1_recv_stop( void )
{
       //__HAL_UART_DISABLE_IT( &huart, UART_IT_IDLE );
        //HAL_UART_DMAStop( &huart );
        HAL_UART_AbortReceive( &huart );
}


uint32_t bsp_ser1_dma_recv_get_ndtr( void )
{
        return( huart.hdmarx->Instance->CNDTR );
}


/******************************************************************************/
/* Interrupt Service Routines                                                 */
/******************************************************************************/

bsp_ser_isr_sts_t
bsp_ser1_isr( void )
{
        volatile    bsp_ser_isr_sts_t   isr_sts = BSP_SER_ISR_STS_NONE;


        if( __HAL_UART_GET_IT( &huart, UART_IT_IDLE ) )
        {
                __HAL_UART_CLEAR_IT( &huart, USART_ICR_IDLECF );
                isr_sts         |=  BSP_SER_ISR_STS_IDLE;
        }

        HAL_UART_IRQHandler( &huart );

        return( isr_sts );
}


void
bsp_ser1_dma_tx_isr( void )
{
        HAL_DMA_IRQHandler( huart.hdmatx );
}


void
bsp_ser1_dma_rx_isr( void )
{
        HAL_DMA_IRQHandler( huart.hdmarx );
}


void
bsp_ser1_xmit_complete( void )
{
        bsp_ser1_xfer_dir_set( false );
}
