/*
 * Rover_Test1.c
 *
 * Created: 7/31/2024 4:59:40 AM
 * Author : ahmed
 */ 

#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "DIO_interface.h"
#include "SG90_interface.h"
#include "TIMER0_Interface.h"
#include "TIMER0_reg.h"
#include "TIMER1_interface.h"
#include "TIMER2_interface.h"
#include "UltraSonic_interface.h"
#include "TIMER1_interface.h"
#include "TIMER1_reg.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#include "Rover_interface.h"
#include "UART_interface.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

#define F_CPU 16000000UL
#include <util/delay.h>





int main(void)
{
			
			
			Radar_LCD_Init();
			Rover_voidMotorsInit();
			IrSensor_voidInit();
			Buzzer_voidInit();
			RoverTransmit_Init();
		
			//xTaskCreate(vTask_RoverInit,"Initialize",configMINIMAL_STACK_SIZE,NULL,INIT_TASK_PRIORITY,NULL);
			
			xTaskCreate(vTask_UltraSonicSendData,"UltraS",configMINIMAL_STACK_SIZE,NULL,UltraSonic_TASK_PRIORITY,NULL);
			
			xTaskCreate(vTask_RoverMove,"Movem",configMINIMAL_STACK_SIZE,NULL,MOVEMENT_TASK_PRIORITY,NULL);
			xTaskCreate(vTask_DataDisplay,"Displa",configMINIMAL_STACK_SIZE,NULL,DISPLAY_TASK_PRIORITY,NULL);
			//xTaskCreate(vTask_BT_SendStatus,"BT",configMINIMAL_STACK_SIZE,NULL,BLUETOOTH_TASK_PRIORITY,NULL);
		    
			vTaskStartScheduler();
			
			
			/**** Should Never Reach Here *******/
			while(1);
			
			
			
}

