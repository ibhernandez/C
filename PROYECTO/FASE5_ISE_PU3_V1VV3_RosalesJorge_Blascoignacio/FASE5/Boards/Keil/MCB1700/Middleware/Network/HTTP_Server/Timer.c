#include "Timer.h"
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

#define portTimer0	1
#define pinTimer0	26  //P1.26 CAP0.0


unsigned int period = 0;
unsigned int previous = 0;
unsigned int current = 0 ;
int limitFlag = 0;
#define TIMER_RES 0.01
// POR SI TIENES DUDAS, AQUI EXPLICA COMO FUNCIONA
// FUENTE:  http://www.ocfreaks.com/lpc1768-timer-input-capture-frequency-counter-tutorial/

//Uso de un timer que cada vez que llegue match haga toggle en el pin, con esto se simula un señal entrante de frecuencia variante.
//El valor de match se cambia según la temperatura.
void TIMER2_IRQHandler(void)
{
	LPC_TIM2->IR |= (1<<4); //Clear Interrupt Flag
	current = LPC_TIM2->CR0;
	if(current < previous) //TC has overflowed
	{
		period = 0xFFFFFFFF + current - previous;
	}
	else
	{
		period = current - previous;
	}
	previous = current; 
	
}

void timer_init(void){
	//Configuracion del timer de medida:
	/*Using CCLK = 100Mhz and PCLK_TIMER2 = 100Mhz.*/
	LPC_SC->PCONP |= (1<<22);//Encendemos Timer2
	LPC_SC->PCLKSEL1 |= (1<<12); //Timer 2 a frez PCLK_TIMER2 = CCLK  100Mhz
	LPC_PINCON->PINSEL0 |= (1<<9) | (1<<8); //Seleccionamos el cap 2.0 funcion 3 DIP 30
	LPC_TIM2->CTCR = 0x0; //Configurado como timer.
	LPC_TIM2->PR = 0; //tc incrementa cada pclks resolucion de 0.01us pag 506
	LPC_TIM2->TCR = 0x02; //Reset Timer1
	LPC_TIM2->CCR = 0x01 | (1<<2); //Captua el flanco de subida y genera interrupcion.
	LPC_TIM2->TCR = 0x01; //Enable timer1
	//Habilitamos la interrupcion
	NVIC_EnableIRQ(TIMER2_IRQn); //Habilitamos las interrupciones
	
	
	//Configuracion del timer de toggle:
	PIN_Configure(0,10, PIN_FUNC_3, PIN_PINMODE_PULLUP, PIN_PINMODE_NORMAL);

	LPC_SC->PCONP |= (1<<23); //Encendemos timer 3
	LPC_SC->PCLKSEL1 |= (1<<14);//Timer3 a frez PCLK_TIMER2 = CCLK  100Mhz
	
	LPC_TIM3->PR  = 0;
	LPC_TIM3->MR0 =0x02; //Reset on MR0
	LPC_TIM3->EMR= 0x01 | (1<<4)|(1<<5); //Toggle en el MAT 3.0
	LPC_TIM3 ->	MCR |= 1<<1; //CONFIGURAR PARA RESET EN MR0



}
void timer_set(float tiempo_ns){
	LPC_TIM3->TCR = 0x02; //Reset Timer3
	LPC_TIM3->MR0	=(int)((double)tiempo_ns*(10^9))*100*(10^6)/2;  //1 tick cada 10ns
	LPC_TIM3->TCR = 0x01; //Enable timer3
}
float freqzin (void){
	//Return frecuencia en KHz
	return ((1.0/(period*TIMER_RES*2.78027465)) * 100000000.0);//el 2.78027 sirve para escalar dado que todos los valores salian multiplicados por ese valor (no se bien porque)
}