
#include "switch.h"

#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

 void initPulsador(void){
	PIN_Configure(PUERTO_JOY, JOY_MD, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	NVIC_EnableIRQ(EINT3_IRQn);	
	LPC_GPIOINT->IO0IntEnR = (1<<JOY_MD);
 }
 