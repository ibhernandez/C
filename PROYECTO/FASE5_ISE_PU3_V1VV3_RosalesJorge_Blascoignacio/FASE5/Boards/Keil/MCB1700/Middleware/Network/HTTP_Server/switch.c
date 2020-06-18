
#include "switch.h"
#include "cmsis_os2.h" 
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

extern osThreadId_t TID_Thread_sw;

void EINT3_IRQHandler (void){
		
	//bajada
	if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_LF)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_LF));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_LF);	
		osThreadFlagsSet(TID_Thread_sw, lff);
			
	}
	else if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_RG)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_RG));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_RG) ;	
		osThreadFlagsSet(TID_Thread_sw, rgf);
	}
	else if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_MD)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_MD));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
		osThreadFlagsSet(TID_Thread_sw, mdf);
		
	}else if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_UP)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_UP));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_UP);	
		osThreadFlagsSet(TID_Thread_sw, upf);
		
	}else if(LPC_GPIOINT-> IO0IntStatF & (1<<JOY_DW)){
		LPC_GPIOINT->IO0IntEnF &= (~(1<<JOY_DW));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_DW);	
		osThreadFlagsSet(TID_Thread_sw, dwf);
		
	}
		
	//subida
	else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_LF)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_LF));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_LF);	
		osThreadFlagsSet(TID_Thread_sw, lf);
	
	}else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_RG)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_RG));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_RG);	
		osThreadFlagsSet(TID_Thread_sw, rg);
	
	}else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_MD)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_MD));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
		osThreadFlagsSet(TID_Thread_sw, md);
		
	}else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_UP)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_UP));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_UP);	
		osThreadFlagsSet(TID_Thread_sw, up);
	
	}	else if(LPC_GPIOINT-> IO0IntStatR & (1<<JOY_DW)){
		LPC_GPIOINT->IO0IntEnR &= (~(1<<JOY_DW));
		LPC_GPIOINT-> IO0IntClr|=(1<<JOY_DW);	
		osThreadFlagsSet(TID_Thread_sw, dw);

	}	
}
 void initPulsador(void){
	// configura el puerto 0 como entrada de pull down para todos los pines conectados al joystick
	PIN_Configure(PUERTO_JOY, JOY_LF, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_JOY, JOY_RG, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_JOY, JOY_MD, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	//activa la interrupcion
	NVIC_EnableIRQ(EINT3_IRQn);	
	//configura el interruptor con flanco de subida 
	LPC_GPIOINT->IO0IntEnR =( 1<<JOY_MD) | (1<<JOY_LF) | (1<<JOY_RG) ;
 }
 void initPulsFase3(void){
	PIN_Configure(PUERTO_JOY, JOY_UP, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(PUERTO_JOY, JOY_DW, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	LPC_GPIOINT->IO0IntEnR |= (1<<JOY_UP)| (1<<JOY_DW);
 }