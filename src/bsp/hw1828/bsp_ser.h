/**
  * @file    bsp_ser.h
  * @brief   BSP - Serial port services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BSP_SER_H
#define  BSP_SER_H


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

//#include "stm32f3xx_ll_bus.h"
//#include "stm32f3xx_ll_rcc.h"
//#include "stm32f3xx_ll_gpio.h"
//#include "stm32f3xx_ll_dma.h"
//#include "stm32f3xx_ll_usart.h"


void    bsp_ser1_init(                  const   size_t                  baud );

void    ser1_dir_set_recv( void );
void    ser1_dir_set_xmit( void );


void    bsp_ser1_recv(                          uint8_t *               data,
                                                size_t                  size );

uint32_t bsp_ser1_dma_recv_get_ndtr(            void                         );

bool    bsp_ser1_xmit(                          uint8_t *               data,
                                                size_t                  size );

void    bsp_ser1_recv(                          uint8_t *               data,
                                                size_t                  size );

void    bsp_ser1_isr( void );

void    bsp_ser1_dma_rx_isr( void );

void    bsp_ser1_dma_tx_isr( void );



#endif	//BSP_SER_H
