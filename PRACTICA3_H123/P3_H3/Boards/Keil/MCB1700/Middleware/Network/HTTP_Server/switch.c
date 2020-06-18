
#include "switch.h"

#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

void EINT3_IRQHandler (void){
		
	//bajada

 if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_MD)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_MD));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
		osThreadFlagsSet(TID_Thread_sw, md);
		
	}else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_MD)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_MD));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
		osThreadFlagsSet(TID_Thread_sw, md);

	}		
}
 void initPulsador(void){
	PIN_Configure(PUERTO_JOY, JOY_MD, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	NVIC_EnableIRQ(EINT3_IRQn);	
	LPC_GPIOINT->IO0IntEnR = (1<<JOY_MD);
 }
 