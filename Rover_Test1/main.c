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
			
			xTaskCreate(vTask_RoverInit,"Initialize",configMINIMAL_STACK_SIZE,NULL,INIT_TASK_PRIORITY,NULL);
			xTaskCreate(vTask_RoverMove,"Movement",configMINIMAL_STACK_SIZE,NULL,MOVEMENT_TASK_PRIORITY,NULL);
			xTaskCreate(vTask_DataDisplay,"Display",configMINIMAL_STACK_SIZE,NULL,DISPLAY_TASK_PRIORITY,NULL);
			xTaskCreate(vTask_BT_SendStatus,"BT",configMINIMAL_STACK_SIZE,NULL,BLUETOOTH_TASK_PRIORITY,NULL);
			xTaskCreate(vTask_SensorRead,"Sensors",configMINIMAL_STACK_SIZE,NULL,SENSORS_TASK_PRIORITY,NULL);
			vTaskStartScheduler();
			
			
			while(1);
			
			
			
}

