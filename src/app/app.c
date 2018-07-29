/**
  * @file    app.c
  * @brief   Application
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdbool.h>
#include "app.h"
#include "bsp_ser.h"


                UART_HandleTypeDef      UartHandle;
                uint8_t                 aTxStartMessage[]       = "1234567890\n\r";
                uint8_t                 aTxEndMessage[]         = "\n\r Example Finished\n\r";
                uint8_t                 aRxBuffer[ RXBUFFERSIZE ];

                uint16_t                sens;
                bool                    sens_rdy                = false;
                bool                    uart_tx_complete        = false;
                uint8_t                 str_startup_msg[]       = " ADC,  ADC (uV), SENS (nA),\r\n";


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static
void Error_Handler(void)
{
        //BSP_LED_On(LED3);
        while( 1 );
}


/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
        /* Turn LED1 on: Transfer in transmission process is correct */
        //BSP_LED_On(LED1);
}


/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
        /* Turn LED2 on: Transfer in reception process is correct */
        //BSP_LED_On(LED2);
}


/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
        /* Turn LED3 on: Transfer error in reception/transmission process */
        //BSP_LED_On(LED3);
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
        sens            = HAL_ADC_GetValue( hadc );        //Get the converted value of regular channel
        sens_rdy        = true;
}


int main( void )
{
        const   uint32_t        sens_R_Ohm     = 5600;
        const   uint32_t        sens_step_uV    = 3300000L / 4096;
                uint32_t        sens_uV;
                uint32_t        sens_nA;

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
        HAL_Init();

        app_clock_config();

        bsp_ser1_init( 9600 );

        bsp_ser1_xmit( str_startup_msg, sizeof(str_startup_msg) );

        HAL_Delay( 1000 );

        app_sens_init();
        app_sens_run();


        while( true )
        {
                while( !sens_rdy );


                //bsp_ser1_xmit( aTxStartMessage, TXSTARTMESSAGESIZE );
                //bsp_ser1_xmit( (uint8_t *) &sens, 2 );

                sens_uV         =   sens * sens_step_uV;
                sens_nA         =   (sens_uV * 1000) / sens_R_Ohm;
                snprintf( (char *) aRxBuffer, sizeof(aRxBuffer), "%4d,%10d,%10d,\r\n", sens, sens_uV, sens_nA  );

                //printf("Distance = %"PRIu32" cm\n", sens_uV );

                bsp_ser1_xmit( aRxBuffer, 29 );

                sens_rdy        = false;

                //HAL_Delay( 1000 );
        }




        /*##-3- Put UART peripheral in reception process ###########################*/
        /* Any data received will be stored in "RxBuffer" buffer : the number max of
        data received is 10 */
        if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        {
                Error_Handler();
        }

        /*##-4- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }

        /*##-5- Send the received Buffer ###########################################*/
        if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
        {
                Error_Handler();
        }

        /*##-6- Wait for the end of the transfer ###################################*/
        /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }

        /*##-7- Send the End Message ###############################################*/
        if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxEndMessage, TXENDMESSAGESIZE)!= HAL_OK)
        {
                Error_Handler();
        }

        /*##-8- Wait for the end of the transfer ###################################*/
        while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
        {
        }

        while ( 1 )
        {
        }
}
