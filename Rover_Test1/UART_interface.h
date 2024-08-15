/*
 * UART_interface.h
 *
 * Created: 6/28/2024 11:33:44 AM
 *  Author: ahmed
 */ 


#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

void UART_voidInit(void);

void UART_voidTX(u8 copy_u8data);

u8 UART_u8RX(void);



#endif /* UART_INTERFACE_H_ */