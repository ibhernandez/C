#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"
#include "timer.h"
#define MiliToMicroSec(x)  (x*1000)  /* ms is multiplied by 1000 to get us*/

void timer0_config(uint32_t tiempo_ms){
	LPC_SC->PCLKSEL0 &=~((1<<3)|(1<<2));
	LPC_TIM0->MCR	|=1 | (1<<1); //Generar interrupcion en match 0 y resetearlo.
  LPC_TIM0->PR   = getPrescalarForUs(2);      /* Prescalar for 1us */
  LPC_TIM0->MR0  = MiliToMicroSec(tiempo_ms);  

}

void timer0_start_irq(void){
	//habilitacion del timer
	LPC_TIM0->TCR |=1; //Habilitar la cuenta del reloj;
	NVIC_EnableIRQ(TIMER0_IRQn);  
}

 int getPrescalarForUs(uint8_t timerPclkBit){
    unsigned int pclk,prescalarForUs;
    pclk = (LPC_SC->PCLKSEL0 >> timerPclkBit) & 0x03;  /* get the pclk info for required timer */

    switch ( pclk )                                    /* Decode the bits to determine the pclk*/
    {
    case 0x00:
        pclk = SystemCoreClock/4;
        break;

    case 0x01:
        pclk = SystemCoreClock;
        break; 

    case 0x02:
        pclk = SystemCoreClock/2;
        break; 

    case 0x03:
        pclk = SystemCoreClock/8;
        break;

    default:
        pclk = SystemCoreClock/4;
        break;  
    }
		    prescalarForUs =pclk/1000000 - 1;                    /* Prescalar for 1us (1000000Counts/sec) */

    return prescalarForUs;
}
	
