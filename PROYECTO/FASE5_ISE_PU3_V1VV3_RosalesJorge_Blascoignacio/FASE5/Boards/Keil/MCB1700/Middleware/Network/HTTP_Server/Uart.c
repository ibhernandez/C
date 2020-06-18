#include <lpc17xx.h>
#include "Uart.h"
#include "cmsis_os2.h" 

#define SBIT_WordLenght    0x00u
#define SBIT_DLAB          0x07u
#define SBIT_FIFO          0x00u
#define SBIT_RxFIFO        0x01u
#define SBIT_TxFIFO        0x02u

#define SBIT_RDR           0x00u
#define SBIT_THRE          0x05u

extern osThreadId_t TID_REC_UART;	
char recibido [19];
int indice=0;
/* Function to initialize the UART0 at specifief baud rate */
void uart_init(uint32_t baudrate)
{
    uint32_t var_UartPclk_u32,var_Pclk_u32,var_RegValue_u32;

    LPC_PINCON->PINSEL0 &= ~0x000000F0;
    LPC_PINCON->PINSEL0 |= 0x00000050;            // Enable TxD0 P0.2 and p0.3 
	  //configurar para generar interrupcion al recibir datos
	  LPC_UART0->IER =1;
		//habilita la fifo, y limpia la fifo de transmision y recepcion
    LPC_UART0->FCR = (1<<SBIT_FIFO) | (1<<SBIT_RxFIFO) | (1<<SBIT_TxFIFO); 
		//Longitud de 8 bits para el caracter, 1 bit de stop y habilita acceso a lo latches
    LPC_UART0->LCR = (0x03<<SBIT_WordLenght) | (1<<SBIT_DLAB);             // 8bit data, 1Stop bit, No parity


    /** Baud Rate Calculation :
       PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
       Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
       The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
       (Refer data sheet for more info)
       UART_PCLK    PCLK
         0x00       SystemFreq/4        
         0x01       SystemFreq
         0x02       SystemFreq/2
         0x03       SystemFreq/8   
     **/

    var_UartPclk_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

    switch( var_UartPclk_u32 )
    {
    case 0x00:
        var_Pclk_u32 = SystemCoreClock/4;
        break;
    case 0x01:
        var_Pclk_u32 = SystemCoreClock;
        break; 
    case 0x02:
        var_Pclk_u32 = SystemCoreClock/2;
        break; 
    case 0x03:
        var_Pclk_u32 = SystemCoreClock/8;
        break;
    }


    var_RegValue_u32 = ( var_Pclk_u32 / (16 * baudrate )); 
		// set baud rate
    LPC_UART0->DLL =  var_RegValue_u32 & 0xFF;
    LPC_UART0->DLM = (var_RegValue_u32 >> 0x08) & 0xFF;
    LPC_UART0->LCR &= ~(1<<SBIT_DLAB);  // Clear DLAB after setting DLL,DLM
		NVIC_EnableIRQ(UART0_IRQn);
		
		
}

/* Function to transmit a char */
void uart_TxChar(char ch)
{
    while((LPC_UART0->LSR & SBIT_THRE)==SBIT_THRE); // Wait for Previous transmission
    LPC_UART0->THR=ch;                                  // Load the data to be transmitted
}
void UART0_IRQHandler(void){
	if (LPC_UART0->IIR & (2<<1)){
		recibido[indice]=uart_RxChar();
		if((recibido[indice]=='r') | (recibido[indice]=='R'))
			indice=0;
		else{
			indice++;
			if (indice==20){
				indice=0;
				osThreadFlagsSet(TID_REC_UART, 0x001);
			}			
		}
	}
	

}
void getCaracteres(char* c){
	for (int i=0;i<20;i++){
		c[i]=recibido[i];
	}
}
/* Function to Receive a char */
char uart_RxChar()
{
    char ch; 
    ch = LPC_UART0->RBR;                                // Read received data    
    return ch;
}
