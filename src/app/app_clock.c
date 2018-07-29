/**
  * @file    app_clock.c
  * @brief   Configure MCU clock tree
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <stdbool.h>
#include "app.h"
#include "bsp.h"


void    app_clock_config( void )
{
        RCC_OscInitTypeDef      osc;
        RCC_ClkInitTypeDef      clk;


        osc.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
        osc.HSEState            = RCC_HSE_ON;
        osc.HSEPredivValue      = RCC_CFGR2_PREDIV_DIV2;
        osc.PLL.PLLState        = RCC_PLL_ON;
        osc.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
        osc.PLL.PLLMUL          = RCC_PLL_MUL9;

        if( HAL_RCC_OscConfig( &osc ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }


        clk.ClockType           = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
        clk.SYSCLKSource        = RCC_SYSCLKSOURCE_PLLCLK;
        clk.AHBCLKDivider       = RCC_SYSCLK_DIV1;
        clk.APB1CLKDivider      = RCC_HCLK_DIV2;
        clk.APB2CLKDivider      = RCC_HCLK_DIV1;

        if( HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_2 ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}
