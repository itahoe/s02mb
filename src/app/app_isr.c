/**
  * @file    app_isr.c
  * @brief   Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "app.h"
#include "bsp_ser.h"
#include "bsp.h"


void    hard_fault_handler(                     uint32_t *      arg     );
void    NMI_Handler( void );
void    HardFault_Handler( void );
void    MemManage_Handler( void );
void    BusFault_Handler( void );
void    UsageFault_Handler( void );
void    SVC_Handler( void );
void    DebugMon_Handler( void );
void    PendSV_Handler( void );
void    SysTick_Handler( void );

void    DMA1_Channel4_IRQHandler( void );
void    DMA1_Channel5_IRQHandler( void );
void    USART1_IRQHandler( void );
void    ADC1_IRQHandler( void );


/**
 * @brief Hard Fault handler
 */
void    hard_fault_handler(                     uint32_t *      arg     )
{
	volatile uint32_t stack_ptr;
	volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
	volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;

	stack_ptr = (uint32_t)arg;
	stacked_r0 = arg[0];
	stacked_r1 = arg[1];
	stacked_r2 = arg[2];
	stacked_r3 = arg[3];
	stacked_r12 = arg[4];
	stacked_lr = arg[5];
	stacked_pc = arg[6];
	stacked_psr = arg[7];

	APP_TRACE("\r\n--- Hard fault handler ---\n");
	APP_TRACE("\nstck_ptr\t= 0x%08X", stack_ptr);
	APP_TRACE("\nR0\t= 0x%08X", stacked_r0);
	APP_TRACE("\nR1\t= 0x%08X", stacked_r1);
	APP_TRACE("\nR2\t= 0x%08X", stacked_r2);
	APP_TRACE("\nR3\t= 0x%08X", stacked_r3);
	APP_TRACE("\nR12\t= 0x%08X", stacked_r12);
	APP_TRACE("\nLR[R14]\t= 0x%08X\tsubroutine call return address", stacked_lr);
	APP_TRACE("\nPC[R15]\t= 0x%08X\tprogram counter", stacked_pc);
	APP_TRACE("\nPSR\t= 0x%08X", stacked_psr);
	APP_TRACE("\nBFAR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED38))));
	APP_TRACE("\nCFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED28))));
	APP_TRACE("\nHFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED2C))));
	APP_TRACE("\nDFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED30))));
	APP_TRACE("\nAFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED3C))));
	APP_TRACE("\nSCB_SHCSR\t= 0x%08X", SCB->SHCSR);
	// memstat();
	__asm volatile ("bkpt #0\n");         // Break into the debugger
	while(1);
}


/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void    NMI_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
        while( 1 );
	#endif //NDEBUG
}


/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void    HardFault_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	asm volatile
	(
	        "TST	LR,		#4			\n"
	        "ITE	EQ					\n"
	        "MRSEQ	R0,		MSP			\n"
	        "MRSNE	R0,		PSP			\n"
	        "B		hard_fault_handler"
	);
	#endif //NDEBUG
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void    MemManage_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
        while( 1 );
	#endif //NDEBUG
}


/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void    BusFault_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
        while( 1 );
	#endif //NDEBUG
}


/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void    UsageFault_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
        while( 1 );
	#endif //NDEBUG
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void    DebugMon_Handler( void )
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
        while( 1 );
	#endif //NDEBUG
}




/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*
void SVC_Handler(void)
{
}
*/

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
        HAL_IncTick();
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
        if( huart->Instance == USART1 )
        {
                bsp_ser1_xmit_complete();
        }
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
}


/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for USART data reception
  */
void    DMA1_Channel4_IRQHandler( void )
{
        bsp_ser1_dma_tx_isr();
}


/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for USART data transmission
  */
void    DMA1_Channel5_IRQHandler( void )
{
        bsp_ser1_dma_rx_isr();
}


/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA
  *         used for USART data transmission
  */
void    USART1_IRQHandler( void )
{
        bsp_ser_isr_sts_t       isr_sts    = bsp_ser1_isr();

        if( isr_sts & BSP_SER_ISR_STS_IDLE )
        {
                app_ser1_msg_complete();
        }
}


void    ADC1_IRQHandler( void )
{
        bsp_sens_isr();
}