/**
 *******************************************************************************
 * @file  usart/usart_uart_int/source/main.c
 * @brief This example demonstrates UART data receive and transfer by interrupt.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Read USART_DR.RDR when USART overrun error occur
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "app_main.h"
#include "qs_log.h"
#include "sys_clock.h"
#include "sys_uart.h"
#include "sys_tmra.h"
#include "GC9a01_drv.h"


#define LED_R_PORT          (GPIO_PORT_B)
#define LED_R_PIN           (GPIO_PIN_09)

#define LED_G_PORT          (GPIO_PORT_B)
#define LED_G_PIN           (GPIO_PIN_08)

#define DLY_MS              (10UL)

#define LED_R_TOGGLE()      (GPIO_TogglePins(LED_R_PORT, LED_R_PIN))
#define LED_G_TOGGLE()      (GPIO_TogglePins(LED_G_PORT, LED_G_PIN))

void LED_Init(void)
{
		// LL_PERIPH_WE(LL_PERIPH_GPIO);
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinState = PIN_STAT_RST;
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    (void)GPIO_Init(LED_R_PORT, LED_R_PIN, &stcGpioInit);
    (void)GPIO_Init(LED_G_PORT, LED_G_PIN, &stcGpioInit);
	
	   // LL_PERIPH_WP(LL_PERIPH_GPIO);
}


int32_t main(void)
{
	/* Register write unprotected for some required peripherals. */
	  LL_PERIPH_WE(LL_PERIPH_ALL);
		SYS_Init();				// 200Mhz 
		TMR0_Config();		// 1ms 
		TmrA_Config();		// pwm 
		// LED_Init();
		qs_log_init();
		Lcd_init();
		sys_uart_init();
	
	
		LL_PERIPH_WP(LL_PERIPH_ALL);
		
		SystemCoreClockUpdate();
		QS_LOG_INFO("PCLK1 freq = %lu Hz", CLK_GetBusClockFreq(CLK_BUS_PCLK1));
		QS_LOG_INFO("System Clock: %lu Hz", SystemCoreClock);

		// Picture_display(0, 0, 240, 240, gImage_green);
			//	LCD_ShowPicture_DMA(0, 0, 240, 240, gImage_green);
			//LCD_ShowPicture_DMA(0, 0, 240, 240, gImage_red);
	
			//TFT_clear();
    for (;;) 
	{

			// Buzzer_on();

			//QS_LOG_INFO("Buzzer_on");
			//SysTick_Delay(500);
			  appRun();
		/*
		
					QS_LOG_INFO("Buzzer_on");
			SysTick_Delay(50);
			Buzzer_off();
			SysTick_Delay(1000);
		
		
			Picture_display(0, 0, 240, 240, gImage_green);
			QS_LOG_INFO("TIME = %d\n",TMR0_GetMsTicks());
			Picture_display(0, 0, 240, 240, gImage_blue);
			QS_LOG_INFO("TIME = %d\n",TMR0_GetMsTicks());
		*/
    }
}




/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
