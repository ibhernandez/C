#ifndef __UART_H
#define __UART_H
#include "cmsis_os2.h" 
#include "LPC17xx.h"

extern osThreadId_t TID_REC_UART;	
/* Function to initialize the UART0 at specifief baud rate */
void uart_init(uint32_t baudrate);
/* Function to transmit a char */
void uart_TxChar(char ch);
/* Function to Receive a char */
char uart_RxChar();
void UART0_IRQHandler(void);
void getCaracteres(char* c); 


#endif
