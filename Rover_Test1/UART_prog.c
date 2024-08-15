/*
 * UART_prog.c
 *
 * Created: 6/28/2024 11:34:00 AM
 *  Author: ahmed
 */ 
#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "UART_reg.h"


void UART_voidInit(void){
	
	 u8 temp=0;
	 
	 /****** Set bit 7 to select UCSRC REG **********/
	 SET_BIT(temp,7);
	 
	 /****** Set bit 1 & bit 2 in UCSRC Reg to write 8 bit data ***/
	 
	 SET_BIT(temp,1);
	 SET_BIT(temp,2);
	
	UCSRC_REG=temp;
	

	
	/****** Set baud rate **********/
	UBRRL_REG=103;
	
	/***** Enable tx & rx ************/
	SET_BIT(UCSRB_REG,3);
	SET_BIT(UCSRB_REG,4);	
	
}

void UART_voidTX(u8 copy_u8data){
	
	UDR_REG=copy_u8data;
	while (GET_BIT(UCSRA_REG,5)==0);
	
	
	
	
}

u8 UART_u8RX(void){
	
	while(GET_BIT(UCSRA_REG,7)==0);
	
	return UDR_REG;
	
	
	
}
