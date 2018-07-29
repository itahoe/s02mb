/**
  * @file    bsp_system.c
  * @brief   System Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp.h"


void bsp_system_reset( void )
{
        NVIC_SystemReset();
}
