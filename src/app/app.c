/**
  * @file    app.c
  * @brief   Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdbool.h>
#include "app_trace.h"
#include "app.h"
#include "bsp.h"

#include <string.h>

/*
static  modbus_dev_t            dev     =
{
        //.addr                                           = 13,
        .reg.device_status                              = { 31001, 1 },
        .reg.alarm_status                               = { 31002, 1 },
        .reg.device_year_month                          = { 31005, 1 },
        .reg.device_day_hour                            = { 31006, 1 },
        .reg.device_minute_second                       = { 31007, 1 },
        .reg.sensor_temperature                         = { 31011, 1 },
        .reg.board_temperature                          = { 31012, 1 },
        .reg.sensor_signal_real                         = { 31021, 1 },
        .reg.sensor_signal_exponent                     = { 31022, 1 },
        .reg.sensor_calc_zero_offset_real               = { 31023, 1 },
        .reg.sensor_calc_zero_offset_exponent           = { 31024, 1 },
        .reg.sensor_calc_sensitivity_coef_real          = { 31025, 1 },
        .reg.sensor_calc_sensitivity_coef_exponent      = { 31026, 1 },
        .reg.sensor_value                               = { 33000, 2 },
        .reg.sensor_value_high                          = { 33000, 1 },
        .reg.sensor_value_low                           = { 33001, 1 },
        .reg.current_loop_value                         = { 33002, 1 },
        .reg.sensor_id                                  = { 40401, 6 },
        .reg.version                                    = { 40417, 3 },
        .reg.sensor_scale_min_real                      = { 41001, 1 },
        .reg.sensor_scale_min_exponent                  = { 41002, 1 },
        .reg.sensor_scale_max_real                      = { 41003, 1 },
        .reg.sensor_scale_max_exponent                  = { 41004, 1 },
        .reg.sensor_scale_dimension                     = { 41005, 1 },
        .reg.sensor_threshold_warning_real              = { 41011, 1 },
        .reg.sensor_threshold_warning_exponent          = { 41012, 1 },
        .reg.sensor_threshold_alarm_real                = { 41013, 1 },
        .reg.sensor_threshold_alarm_exponent            = { 41014, 1 },
};
*/

static  modbus_dev_t            dev     =
{
        //.addr                                           = 13,
        .reg.device_status                              = { 31001 },
        .reg.alarm_status                               = { 31002 },
        .reg.device_year_month                          = { 31005 },
        .reg.device_day_hour                            = { 31006 },
        .reg.device_minute_second                       = { 31007 },
        .reg.sensor_temperature                         = { 31011 },
        .reg.board_temperature                          = { 31012 },
        .reg.sensor_signal_real                         = { 31021 },
        .reg.sensor_signal_exponent                     = { 31022 },
        .reg.sensor_calc_zero_offset_real               = { 31023 },
        .reg.sensor_calc_zero_offset_exponent           = { 31024 },
        .reg.sensor_calc_sensitivity_coef_real          = { 31025 },
        .reg.sensor_calc_sensitivity_coef_exponent      = { 31026 },
        .reg.sensor_value                               = { 33000 },
        .reg.sensor_value_high                          = { 33000 },
        .reg.sensor_value_low                           = { 33001 },
        .reg.current_loop_value                         = { 33002 },
        .reg.sensor_id                                  = { 40401 },
        .reg.version                                    = { 40417 },
        .reg.sensor_scale_min_real                      = { 41001 },
        .reg.sensor_scale_min_exponent                  = { 41002 },
        .reg.sensor_scale_max_real                      = { 41003 },
        .reg.sensor_scale_max_exponent                  = { 41004 },
        .reg.sensor_scale_dimension                     = { 41005 },
        .reg.sensor_threshold_warning_real              = { 41011 },
        .reg.sensor_threshold_warning_exponent          = { 41012 },
        .reg.sensor_threshold_alarm_real                = { 41013 },
        .reg.sensor_threshold_alarm_exponent            = { 41014 },
};


        UART_HandleTypeDef      UartHandle;
        uint16_t                sens;
        bool                    sens_rdy                = false;
        bool                    uart_tx_complete        = false;
        //uint8_t                 modbus_buf_xmit[ MODBUS_RTU_FRAME_SIZE_MAX_OCT ];
        uint8_t                 modbus_raw[ MODBUS_RTU_FRAME_SIZE_MAX_OCT ];
        uint16_t                modbus_tbl0[ 256 ];
        uint16_t                modbus_tbl1[ 256 ];
        uint16_t                modbus_tbl2[ 256 ];
        uint16_t                modbus_tbl3[ 256 ];
static  uint8_t         const   modbus_dev_addr         = 13;


typedef union
{
    float               f;
    uint16_t            u16[ 2 ];
    uint8_t             u8[ 4 ];
} u_t;


static
void    update(                 float *         f )
{
        u_t             u;


        u.f             =   *f;
        u.f             =   *f;

        modbus_reg_update( &dev.reg.sensor_value_high, (u.u8[3] << 8) |  (u.u8[2] & 0xFF));
        modbus_reg_update( &dev.reg.sensor_value_low,  (u.u8[1] << 8) |  (u.u8[0] & 0xFF));
}


/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void    HAL_ADC_ConvCpltCallback(       ADC_HandleTypeDef       *hadc )
{
        float           f;


        sens            = HAL_ADC_GetValue( hadc );        //Get the converted value of regular channel
        f       =   sens;
        f       /=  10;

        update( &f );
}


void app_ser1_msg_complete( void )
{
        sens_rdy        =   true;
}


/*

uint16_t        device_id
uint16_t        device_firmware_version
uint64_t        device_serial

int16_t         sensor_ch0_value_raw
int16_t         sensor_ch1_value_raw
q15_t           sensor_ch0_value_ppm
q15_t           sensor_ch1_value_ppm
uint64_t        unix_time

*/

int main( void )
{
        //const   uint32_t        sens_R_Ohm     = 5600;
        //const   uint32_t        sens_step_uV    = 3300000L / 4096;
                //uint32_t        sens_uV;
                //uint32_t        sens_nA;

                //uint32_t        temp    = 0;
                //uint32_t        cnt;
                modbus_rslt_t   rslt;
                //modbus_rqst_t   rqst;

        /* STM32F3xx HAL library initialization:
                - Configure the Flash prefetch
                - Systick timer is configured by default as source of time base, but user
                        can eventually implement his proper time base source (a general purpose
                        timer for example or other time source), keeping in mind that Time base
                        duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
                        handled in milliseconds basis.
                - Set NVIC Group Priority to 4
                - Low Level Initialization
        */
        app_clock_config();

        HAL_Init();


        bsp_ser1_init( 9600 );

        //bsp_ser1_xmit( str_startup_msg, sizeof(str_startup_msg) );
        //HAL_Delay( 100 );

        app_sens_init();
        app_sens_run();


        modbus_rtu_init(        &dev,
                                //modbus_buf_xmit,
                                modbus_raw,
                                MODBUS_RTU_FRAME_SIZE_MAX_OCT,
                                modbus_tbl0,
                                modbus_tbl1,
                                modbus_tbl2,
                                modbus_tbl3 );

        modbus_cfg( &dev, MODBUS_CFG_DEV_ADDR, &modbus_dev_addr );


        while( true )
        {
                //bsp_ser1_recv( data_recv, sizeof( data_recv ) );
                //bsp_ser1_recv_start( dev.recv.buf, dev.recv.cnt );
                bsp_ser1_recv_start( dev.raw.buf, MODBUS_RTU_FRAME_SIZE_MAX_OCT );

                while( !sens_rdy );

                //dev.recv.cnt    =   sizeof( dev.recv.buf ) - bsp_ser1_dma_recv_get_ndtr();
                dev.raw.cnt     =   MODBUS_RTU_FRAME_SIZE_MAX_OCT - bsp_ser1_dma_recv_get_ndtr();

                HAL_Delay( 10 );

                rslt    =   modbus_rtu_rqst_recv( &dev );

                //.reg.sensor_value                               = { 33000, 2 },

                //APP_TRACE( "%02X %02X %02X %02X %02X %02X %02X %02X - %d\n", modbus_buf_recv[0], modbus_buf_recv[1], modbus_buf_recv[2], modbus_buf_recv[3], modbus_buf_recv[4], modbus_buf_recv[5], modbus_buf_recv[6], modbus_buf_recv[7], cnt);
                //memset( modbus_buf_recv, 0, 10 );

                switch( rslt )
                {
                        case MODBUS_RSLT_OK:
                                APP_TRACE( "MODBUS_RSLT_OK\n" );
                                rslt    =   modbus_rtu_resp_xmit( &dev );
                                break;

                        case MODBUS_RSLT_INVALID_CRC:
                                APP_TRACE( "MODBUS_RSLT_INVALID_CRC\n" );
                                break;

                        case MODBUS_RSLT_INVALID_ADDR:
                                APP_TRACE( "MODBUS_RSLT_INVALID_ADDR\n" );
                                break;

                        default:
                                APP_TRACE( "MODBUS_RSLT_XXX\n" );
                                break;
                }

                //sens_uV         =   sens * sens_step_uV;
                //sens_nA         =   (sens_uV * 1000) / sens_R_Ohm;
                //snprintf( (char *) data_xmit, sizeof(data_xmit), "%4d,%10d,%10d,\r\n", sens, sens_uV, sens_nA  );
                //bsp_ser1_xmit( data_xmit, 29 );

                sens_rdy        = false;
        }




        /*##-3- Put UART peripheral in reception process ###########################*/
        /* Any data received will be stored in "RxBuffer" buffer : the number max of
        data received is 10 */
/*
        if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        {
                Error_Handler();
        }
*/

        /*##-4- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */
/*
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }
*/

        /*##-5- Send the received Buffer ###########################################*/
/*
        if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        {
                Error_Handler();
        }
*/

        /*##-6- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
/*
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }
*/
        /*##-7- Send the End Message ###############################################*/
/*
        if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxEndMessage, TXENDMESSAGESIZE)!= HAL_OK)
        {
                Error_Handler();
        }
*/

        /*##-8- Wait for the end of the transfer ###################################*/
/*
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }
*/
}
