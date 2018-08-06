/**
  * @file    bsp.h
  * @brief   Board Support Package
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_H
#define  BSP_H


//#include <stdint.h>
#include <stdbool.h>
//#include "config.h"
#include "stm32f3xx_hal.h"

//#include "stm32f3xx_ll_bus.h"
//#include "stm32f3xx_ll_rcc.h"
//#include "stm32f3xx_ll_gpio.h"
//#include "stm32f3xx_ll_dma.h"
//#include "stm32f3xx_ll_usart.h"


#define BSP_SYSTICK_HZ                  1000ul

#define BSP_NVIC_PRIO_SER1_DMA_RX       9
#define BSP_NVIC_PRIO_SER1_DMA_TX       9
#define BSP_NVIC_PRIO_SER1_UART         11
#define BSP_NVIC_PRIO_SENS_ADC          7


typedef enum    bsp_ser_isr_sts_e
{
        BSP_SER_ISR_STS_NONE,
        BSP_SER_ISR_STS_IDLE,
} bsp_ser_isr_sts_t;


void    bsp_system_reset( void );

void    bsp_sens_init( void );

void    bsp_sens_run( void );

void    bsp_sens_isr( void );


/******************************************************************************/
/* SERIAL 1                                                                   */
/******************************************************************************/
void    bsp_ser1_init(                  const   size_t          baud );

void    ser1_dir_set_recv( void );

void    ser1_dir_set_xmit( void );


void    bsp_ser1_recv_start(                    uint8_t *       data,
                                                size_t          size );

void    bsp_ser1_recv_stop( void );


uint32_t bsp_ser1_dma_recv_get_ndtr( void );

bool    bsp_ser1_xmit(                          uint8_t *       data,
                                                size_t          size );

bsp_ser_isr_sts_t       bsp_ser1_isr( void );

void    bsp_ser1_dma_rx_isr( void );

void    bsp_ser1_dma_tx_isr( void );

void    bsp_ser1_xmit_complete( void );


#endif	//BSP_H
