#include "Board_LED.h"
#include <stdio.h>
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "WTDG.h"
#include "switch.h"
#include "timer.h"

	int estado=0;
	int estado1=0;
	uint32_t contador=0;
void EINT3_IRQHandler (void){
		static uint32_t puls;

	if(puls++<10){
		estado=~estado;
		if(estado)
			LED_On(3);
		else
			LED_Off(3); 
		
	}
	//La solucion es  limpiar el flag cada vez que se entra en la interrupcion.
	//Otra posible solucion es establer que el Timer0 es mas prioritario que Eint3
	LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
}

void TIMER0_IRQHandler(void)
{
    LPC_TIM0->IR |=1 ;//lIMPIAR flag interrupcion 
	
	if(contador>8){
		if(estado1){
			LED_Off(0);
			LED_On(1);

		}else{
			LED_Off(1);
			LED_On(0);

		}
		estado1=~estado1;   
	}
	contador++;
	feedWTDG();
}

void WDT_IRQHandler(void)
{
	// Clear TimeOut flag
	clr_WTDG_Flag();	
	LED_On(2);
}


  int main (void)  {
	 LED_Initialize();
	LED_SetOut (0x01);
	initWTDG(0);
	timer0_config(500); 
		//Para que al pulsar 10 veces surja un problema, es necesario establecer prioridades en las interrupciones.
	NVIC_SetPriority(WDT_IRQn, 0x00);
	NVIC_SetPriority(EINT3_IRQn, 0x01);
	NVIC_SetPriority(TIMER0_IRQn, 0x02);
	initPulsador();	
	timer0_start_irq();
	WTDG_Start();
	feedWTDG();
		
}
