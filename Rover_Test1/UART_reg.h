/*
 * UART_reg.h
 *
 * Created: 6/28/2024 11:33:17 AM
 *  Author: ahmed
 */ 


#ifndef UART_REG_H_
#define UART_REG_H_

#define UDR_REG			*((volatile u8*)0x2c)
#define UCSRA_REG		*((volatile u8*)0x2B)
#define UCSRB_REG		*((volatile u8*)0x2A)
#define UBRRL_REG		*((volatile u8*)0x29)
#define UCSRC_REG		*((volatile u8*)0x40)



#endif /* UART_REG_H_ */