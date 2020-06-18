#ifndef __SWITCH_H
#define __SWITCH_H

#include "cmsis_os2.h"                      // RTOS object definitions
#include "LPC17xx.h"				//libreria de la placa
#include "GPIO_LPC17xx.h"		//libreria para el establecimiento de pines  
#include "PIN_LPC17xx.h"		//librería de los pines



#define rg 		0x0001	//00100
#define lf 		0x0002	//01000
#define rgf		0x0004	//...01000000
#define lff		0x0008	//...10000000
#define md		0x0010
#define mdf		0x0020
#define up		0x0040
#define upf   0x0080
#define	dw		0x0100	
#define dwf		0x0200

#define PUERTO_JOY	0			
#define JOY_DW			17		
#define JOY_LF			15		
#define JOY_RG			24			
#define JOY_UP			23	
#define JOY_MD			16


extern osThreadId_t TID_Thread_sw;

//Funciones 
void initPulsador(void);
void EINT3_IRQHandler (void);
void initPulsFase3(void);

#endif
