/*
 * Rover_prog.c
 *
 * Created: 8/12/2024 7:25:17 PM
 *  Author: ahmed
 */ 

#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#include "DIO_interface.h"
#include "SG90_interface.h"
#include "TIMER2_interface.h"
#include "UltraSonic_interface.h"
#include "Rover_interface.h"
#include "UART_interface.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#define F_CPU 16000000UL
#include <util/delay.h>

Rover_Status Rover_state; // Extern declaration to access the current rover status across multiple files



/******************** Initialize Radar and LCD ********************/
/* Initializes the radar and LCD modules for operation. */
void Radar_LCD_Init(void){
	
			DIO_voidSetPinDir(LCD_DPORT,LCD_D4_PIN,OUTPUT);	
			DIO_voidSetPinDir(LCD_DPORT,LCD_D5_PIN,OUTPUT);	
			DIO_voidSetPinDir(LCD_DPORT,LCD_D6_PIN,OUTPUT);	
			DIO_voidSetPinDir(LCD_DPORT,LCD_D7_PIN,OUTPUT);	
			DIO_voidSetPinDir(LCD_CPORT,LCD_RS_PIN,OUTPUT);	
			DIO_voidSetPinDir(LCD_CPORT,LCD_RW_PIN,OUTPUT);
			DIO_voidSetPinDir(LCD_CPORT,LCD_EN_PIN,OUTPUT);
			Servo_Init();
			DIO_voidSetPinDir(TRIG_PORT,TRIG_PIN,OUTPUT);
			DIO_voidSetPinDir(ECHO_PORT,ECHO_PIN,INPUT);
			LCD_voidInit();
}

/************** Sweep Radar and Display Angles on LCD **************/
/* Performs a radar sweep and updates the angles on the LCD.
   Takes pointers to store the angles swept. Returns the status. */
void Radar_LCD_Sweep(u8 *angle_1 , u8 *angle_2){
	
			/*
			UltraSonic_Sendpulse();
			Rover_state.Distance = UltraSonic_Calc_Distance(UltraSonic_Read_Echo);
			ROVER_LCD_PrintStatus(&Rover_state);
			*/
			// Sweep from 0 to 180 degrees
			for(*angle_1=90 ; *angle_1 < 180; *angle_1 += 1) {
				
				Servo_SetAngle(*angle_1);
				_delay_ms(15);
			}
			/*
			UltraSonic_Sendpulse();
			Rover_state.Distance = UltraSonic_Calc_Distance(UltraSonic_Read_Echo);
			ROVER_LCD_PrintStatus(&Rover_state);
			*/
			// Sweep back from 180 to 0 degrees
			for(*angle_2 =*angle_1; *angle_2 >0; *angle_2 -= 1) {

				Servo_SetAngle(*angle_2);
				_delay_ms(15);
			}
			/*
			UltraSonic_Sendpulse();
			Rover_state.Distance = UltraSonic_Calc_Distance(UltraSonic_Read_Echo);
			ROVER_LCD_PrintStatus(&Rover_state);
			*/
			for (*angle_2=0 ;*angle_2 <=90 ; *angle_2 +=1)
			{
				Servo_SetAngle(*angle_2);
				_delay_ms(15);
			}
			/*
			UltraSonic_Sendpulse();
			Rover_state.Distance = UltraSonic_Calc_Distance(UltraSonic_Read_Echo);
			ROVER_LCD_PrintStatus(&Rover_state);
			*/
			
	
	
	
}

/***************** Initialize Rover Motor Control *****************/
/* Initializes the motor control system for the rover. */
void Rover_voidMotorsInit(void){
			DIO_voidSetPinDir(DIO_PORTC,DIO_PIN6,OUTPUT);
			DIO_voidSetPinDir(DIO_PORTC,DIO_PIN7,OUTPUT);
			DIO_voidSetPinDir(DIO_PORTD,DIO_PIN2,OUTPUT);
			DIO_voidSetPinDir(DIO_PORTD,DIO_PIN3,OUTPUT);
			DIO_voidSetPinDir(DIO_PORTD,DIO_PIN7,OUTPUT);
			
}

/****************** Move Rover Forward (Alternative) ******************/
/* Moves the rover forward at the specified speed (alternative function). */
void Rover_voidMOVFWD(u8 speed){
	
			
			DIO_voidSetPinVal(DIO_PORTC,DIO_PIN6,HIGH);
			DIO_voidSetPinVal(DIO_PORTC,DIO_PIN7,LOW);
			DIO_voidSetPinVal(DIO_PORTD,DIO_PIN2,HIGH);
			DIO_voidSetPinVal(DIO_PORTD,DIO_PIN3,LOW);
			TIMER2_voidFastPWM(speed);
			Rover_state.Direction=1;
}

/***************** Move Rover Backward at Given Speed *****************/
/* Moves the rover backward at the specified speed. */
void Rover_voidMOVBCWD(u8 speed){
			
			DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN1,LOW);
			DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN2,HIGH);
			DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN1,LOW);
			DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN2,HIGH);
			TIMER2_voidFastPWM(speed);
			Rover_state.Direction=2;
}

/*************** Turn Rover Right at Given Speed ****************/
/* Turns the rover to the right at the specified speed. */
void Rover_voidMOVRW(u8 Rspeed){
		   
		   DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN1,LOW);
		   DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN2,HIGH);
		   DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN1,HIGH);
		   DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN2,LOW);
		   TIMER2_voidFastPWM(Rspeed);
		   Rover_state.Direction=3;
}

/*************** Turn Rover Left at Given Speed ****************/
/* Turns the rover to the left at the specified speed. */
void Rover_voidMOVLF(u8 Lspeed){
		
		DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN1,HIGH);
		DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN2,LOW);
		DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN1,LOW);
		DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN2,HIGH);
		TIMER2_voidFastPWM(Lspeed);
		Rover_state.Direction=4;
}

/***************** Stop Rover Movement *****************/
/* Stops all movement of the rover. */
void Rover_voidStop(void){
			
			DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN1,LOW);
			DIO_voidSetPinVal(CTRL1_PORT,CTRL1_PIN2,LOW);
			DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN1,LOW);
			DIO_voidSetPinVal(CTRL2_PORT,CTRL2_PIN2,LOW);
			Rover_state.Direction=5;
			TIMER2_voidFastPWM(0);
}

/*************** Display Rover Status on LCD ****************/
/* Prints the current status of the rover on the LCD.
   Takes a pointer to the Rover_Status structure. */
void ROVER_LCD_PrintStatus(Rover_Status *rover_state){
	
			LCD_SetGridPos(0,0);
			LCD_voidSendString("Dist:");
			LCD_SetGridPos(6,0);
			LCD_voidSendNum(rover_state->Distance);
			// Print direction
			LCD_SetGridPos(9, 0);
			LCD_voidSendString("Dir:");
			LCD_SetGridPos(13, 0);
			switch(rover_state->Direction) {
				case 1:
				LCD_voidSendString("FWD");
				break;
				case 2:
				LCD_voidSendString("BCW");
				break;
				case 3:
				LCD_voidSendString("RW");
				break;
				case 4:
				LCD_voidSendString("LF");
				break;
				case 5:
				LCD_voidSendString("Stop");
				break;
				default:
				LCD_voidSendString("UNK"); // Unknown direction for safety
				break;
			}
			LCD_SetGridPos(0,1);
			LCD_voidSendString("OBJ");
			LCD_SetGridPos(4,1);
			if (rover_state->F_Obj==0)
			{
				LCD_voidSendString("F");
			}else if(rover_state->B_Obj==0){
				LCD_voidSendString("B");
			}
			else if(rover_state->R_Obj==0){
				LCD_voidSendString("R");
				
			}
			else if(rover_state->L_Obj==0){
				
				LCD_voidSendString("L");
			}
	
	
}

/*************** Initialize IR Sensors ***************/
/* Initializes the IR sensors used for object detection. */
void IrSensor_voidInit(void){
	
	
			DIO_voidSetPinDir(IRF_PORT,IRF_PIN,INPUT);
			DIO_voidSetPinDir(IRB_PORT,IRB_PIN,INPUT);
			DIO_voidSetPinDir(IRR_PORT,IRR_PIN,INPUT);
			DIO_voidSetPinDir(IRL_PORT,IRL_PIN,INPUT);
	
	
}

/*************** Get Direction from IR Sensors ***************/
/* Reads the direction from the IR sensors to determine obstacles. */
void IrSensor_GetDir(void){
	
			// Update the Rover state based on IR sensor readings
			Rover_state.B_Obj = DIO_u8GetPinVal(IRB_PORT, IRB_PIN);
			Rover_state.R_Obj = DIO_u8GetPinVal(IRR_PORT, IRR_PIN);
			Rover_state.L_Obj = DIO_u8GetPinVal(IRL_PORT, IRL_PIN);
			Rover_state.F_Obj = DIO_u8GetPinVal(IRF_PORT, IRF_PIN);
			
}

/*************** Initialize Buzzer Module ***************/
/* Initializes the buzzer for sound alerts. */
void Buzzer_voidInit(void){
	
			DIO_voidSetPinDir(DIO_PORTA,DIO_PIN3,OUTPUT);
	
}

/************ Toggle Buzzer Sound for Alerts ************/
/* Toggles the buzzer sound on and off. */
void Buzzer_voidToggle(void){
	
			DIO_voidSetPinVal(DIO_PORTA,DIO_PIN3,HIGH);
			_delay_ms(100);
			DIO_voidSetPinVal(DIO_PORTA,DIO_PIN3,LOW);
			_delay_ms(100);
}

/********** Function to Initialize Data Transmission From Rover ********/
void RoverTransmit_Init(void){
	UART_voidInit();
}
void SendNumber(u16 num) {
	char buffer[6]; // Buffer to hold the string representation of the number
	u8 i = 0;
	if (num == 0) {
		UART_voidTX('0');
		return;
	}

	// Convert number to string manually
	while (num != 0) {
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}

	// Send the string in the correct order
	while (i > 0) {
		UART_voidTX(buffer[--i]);
	}
}
/********** Function to Transmit Rover Status  ********/
void RoverTransmitStatus(void){
	
	UART_voidTX('D');
	UART_voidTX(':');
	SendNumber(Rover_state.Distance);  // Function to send number
	UART_voidTX('F');
	UART_voidTX(':');
	SendNumber(Rover_state.F_Obj);    // Function to send number
	UART_voidTX('B');
	UART_voidTX(':');
	SendNumber(Rover_state.B_Obj);    // Function to send number
	UART_voidTX('R');
	UART_voidTX(':');
	SendNumber(Rover_state.R_Obj);    // Function to send number
	UART_voidTX('L');
	UART_voidTX(':');
	SendNumber(Rover_state.L_Obj);    // Function to send number
	UART_voidTX('D');
	UART_voidTX('i');
	UART_voidTX('r');
	UART_voidTX(':');
	SendNumber(Rover_state.Direction);  // Function to send number
	UART_voidTX('\r'); // Carriage return
	UART_voidTX('\n'); // New line
}

void vTask_RoverInit(void){
	
	Radar_LCD_Init();
	Rover_voidMotorsInit();
	IrSensor_voidInit();
	Buzzer_voidInit();
	RoverTransmit_Init();

	// Once initialization is complete, delete this task
	vTaskDelete(NULL);
	
	
	
}

void vTask_RoverMove(void){
	
	

	while(1){
		
		 u8 frontBlockedCheckCount = 0;
		 // Case 1: Move forward if the front is clear and there's enough distance
		 if (Rover_state.F_Obj == 1 && Rover_state.Distance > 10) {
			 Rover_voidMOVFWD(55);  // Move forward at speed 55
			 while (Rover_state.F_Obj == 1 && Rover_state.Distance > 10) {
				 vTaskDelay(pdMS_TO_TICKS(100));  // Short delay to avoid jitter
				 // Stop if the front becomes blocked
				 if (Rover_state.F_Obj != 1 || Rover_state.Distance <= 10) {
					 Rover_voidStop();
					 break;
				 }
			 }
			 frontBlockedCheckCount = 0;  // Reset the counter if moving forward
		 }
		 // Case 2: If the front is blocked, decide next action
		 else {
			 Rover_voidStop();
			 vTaskDelay(pdMS_TO_TICKS(500));  // Pause for a moment

			 // Check if the back is clear and no objects are detected behind
			 if (Rover_state.B_Obj == 1) {
				 Rover_voidMOVBCWD(55);  // Move backward at speed 55
				 vTaskDelay(pdMS_TO_TICKS(300));  // Backward duration

				 // Recheck the environment after moving backward
				 if (Rover_state.F_Obj == 1 && Rover_state.Distance > 10) {
					 frontBlockedCheckCount++;
					 } else {
					 frontBlockedCheckCount = 0;  // Reset the counter if the front is clear
				 }

				 // If the front is blocked twice, perform a turn
				 if (frontBlockedCheckCount >= 2) {
					 // Check sensor states to decide the next move
					 if (Rover_state.L_Obj != 1 && Rover_state.R_Obj == 1) {
						 // Left is clear, right is blocked -> Turn left
						 Rover_voidMOVLF(85);  // Turn left at speed 85
						 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
					 }
					 else if (Rover_state.R_Obj != 1 && Rover_state.L_Obj == 1) {
						 // Right is clear, left is blocked -> Turn right
						 Rover_voidMOVRW(85);  // Turn right at speed 85
						 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
					 }
					 else if (Rover_state.L_Obj != 1 && Rover_state.R_Obj != 1) {
						 // Both left and right are clear, choose to turn left
						 Rover_voidMOVLF(85);  // Turn left at speed 85
						 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
					 }
					 else {
						 // Both left and right are blocked
						 Rover_voidMOVBCWD(55);  // Move backward at speed 55
						 vTaskDelay(pdMS_TO_TICKS(300));  // Backward duration

						 Rover_voidMOVLF(85);  // Turn left at speed 85
						 vTaskDelay(pdMS_TO_TICKS(1500));  // Longer turn to ensure clearance
					 }

					 Rover_voidStop();
					 vTaskDelay(pdMS_TO_TICKS(150));  // Pause after turning

					 // Reset the counter after turning
					 frontBlockedCheckCount = 0;
					 } else {
					 continue;  // Skip further checks and recheck conditions
				 }
				 } else {
				 // The back is blocked or no objects detected behind, handle turning
				 // Check sensor states to decide the next move
				 if (Rover_state.L_Obj != 1 && Rover_state.R_Obj == 1) {
					 // Left is clear, right is blocked -> Turn left
					 Rover_voidMOVLF(85);  // Turn left at speed 85
					 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
				 }
				 else if (Rover_state.R_Obj != 1 && Rover_state.L_Obj == 1) {
					 // Right is clear, left is blocked -> Turn right
					 Rover_voidMOVRW(85);  // Turn right at speed 85
					 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
				 }
				 else if (Rover_state.L_Obj != 1 && Rover_state.R_Obj != 1) {
					 // Both left and right are clear, choose to turn left
					 Rover_voidMOVLF(85);  // Turn left at speed 85
					 vTaskDelay(pdMS_TO_TICKS(1500));  // Turn duration
				 }
				 else {
					 // Both left and right are blocked
					 Rover_voidMOVBCWD(55);  // Move backward at speed 55
					 vTaskDelay(pdMS_TO_TICKS(300));  // Backward duration

					 Rover_voidMOVLF(85);  // Turn left at speed 85
					 vTaskDelay(pdMS_TO_TICKS(1500));  // Longer turn to ensure clearance
				 }

				 Rover_voidStop();
				 vTaskDelay(pdMS_TO_TICKS(150));  // Pause after turning
			 }
		 }

		 // Small delay before the next control loop iteration
		 vTaskDelay(pdMS_TO_TICKS(100));  // General task delay
		
		
	}
	
	
	
}

void vTask_DataDisplay(void){
	
	
	while(1){
		
		ROVER_LCD_PrintStatus(&Rover_state);
		RoverTransmitStatus();
		vTaskDelay(pdMS_TO_TICKS(150));
	}
	
}

void vTask_SensorRead(void){
	
	
	while(1){
		
		IrSensor_GetDir();
		
		vTaskDelay(20);
	}
	
	
}

void vTask_UltraSonicSendData(void){
	
	while(1){
		
	UltraSonic_Sendpulse();
	Rover_state.Distance = UltraSonic_Calc_Distance(UltraSonic_Read_Echo);
	IrSensor_GetDir();
	vTaskDelay(20);
	}
	
	
}


void vTask_BT_SendStatus(void){
	while(1){
		
		RoverTransmitStatus();
		vTaskDelay(pdMS_TO_TICKS(50));
		
	}
	
}