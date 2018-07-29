/**
  * @file    app_sens.c
  * @brief   Sensors
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include "app.h"
#include "bsp.h"


void    app_sens_init( void )
{
        bsp_sens_init();
}


void    app_sens_run( void )
{
        bsp_sens_run();
}
