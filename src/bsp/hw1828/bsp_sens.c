/**
  * @file    bsp_sens.c
  * @brief   Sensors Routins
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp.h"


static  ADC_HandleTypeDef       hadc;
static  TIM_HandleTypeDef       htim;


static
void    bsp_sens_init_adc( void )
{
        ADC_ChannelConfTypeDef  cfg;


        __HAL_RCC_ADC1_FORCE_RESET();
        __HAL_RCC_ADC1_RELEASE_RESET();
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_ADC1_CONFIG( RCC_ADC1PCLK2_DIV6 );

        hadc.Instance                   = ADC1;
        hadc.Init.ScanConvMode          = DISABLE;
        hadc.Init.ContinuousConvMode    = DISABLE;
        hadc.Init.DiscontinuousConvMode = DISABLE;
        hadc.Init.NbrOfDiscConversion   = 0;
        hadc.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T3_TRGO;
        hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
        hadc.Init.NbrOfConversion       = 1;

        if( HAL_ADC_Init( &hadc ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }

        cfg.Channel                     = ADC_CHANNEL_9;
        cfg.Rank                        = 1;
        cfg.SamplingTime                = ADC_SAMPLETIME_7CYCLES_5;

        if( HAL_ADC_ConfigChannel( &hadc, &cfg ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}


static
void    bsp_sens_init_timebase( void )
{
        TIM_MasterConfigTypeDef cfg;


        __HAL_RCC_TIM3_FORCE_RESET();
        __HAL_RCC_TIM3_RELEASE_RESET();
        __HAL_RCC_TIM3_CLK_ENABLE();

        htim.Instance                   = TIM3;
        htim.Init.Period                = 3600;
        //htim.Init.Prescaler             = 0;
        htim.Init.Prescaler             = 16000;
        htim.Init.ClockDivision         = TIM_CLOCKDIVISION_DIV1;
        htim.Init.CounterMode           = TIM_COUNTERMODE_UP;
        htim.Init.RepetitionCounter     = 0x0;
        htim.Init.AutoReloadPreload     = TIM_AUTORELOAD_PRELOAD_DISABLE;

        if( HAL_TIM_Base_Init( &htim ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }

        cfg.MasterOutputTrigger         = TIM_TRGO_UPDATE;
        cfg.MasterSlaveMode             = TIM_MASTERSLAVEMODE_DISABLE;

        if( HAL_TIMEx_MasterConfigSynchronization( &htim, &cfg ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}


static
void    bsp_sens_init_io( void )
{
        GPIO_InitTypeDef        pin_sens    =   {    .Pin       =    GPIO_PIN_1,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    0 };

        GPIO_InitTypeDef        pin_temp    =   {    .Pin       =    GPIO_PIN_0,
	                                             .Mode      =    GPIO_MODE_ANALOG,
	                                             .Pull      =    GPIO_NOPULL,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    0 };


        __HAL_RCC_GPIOB_CLK_ENABLE();

        HAL_GPIO_Init( GPIOB, &pin_temp );
        HAL_GPIO_Init( GPIOB, &pin_sens );
}


void    bsp_sens_init( void )
{
        bsp_sens_init_adc();
        bsp_sens_init_timebase();
        bsp_sens_init_io();

        NVIC_SetPriority(       ADC1_IRQn,      BSP_NVIC_PRIO_SENS_ADC  );
        NVIC_EnableIRQ(         ADC1_IRQn                               );
}


void    bsp_sens_run( void )
{
        //HAL_ADC_Start_DMA( &hadc, (uint32_t *) data, size );

        if( HAL_ADC_Start_IT( &hadc ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }

        if( HAL_TIM_Base_Start( &htim ) != HAL_OK )
        {
                app_error( __FILE__, __LINE__ );
        }
}


void    bsp_sens_isr( void )
{
        HAL_ADC_IRQHandler( &hadc );
}
