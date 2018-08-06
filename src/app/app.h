/**
  * @file    app.c
  * @brief   Application Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_H
#define APP_H

#include "app_trace.h"
#include "modbus.h"
#include "stm32f3xx_hal.h"



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

void    app_ser1_msg_complete( void );


#endif  //APP_H
