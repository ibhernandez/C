#ifndef __SWITCH_H
#define __SWITCH_H

#include "LPC17xx.h"				//libreria de la placa
#include "GPIO_LPC17xx.h"		//libreria para el establecimiento de pines  
#include "PIN_LPC17xx.h"		//librer�a de los pines


#define md		0x0100


#define PUERTO_JOY	0			//los valores en comentarios son los que te da el fabricante

#define JOY_MD			17


//Funciones 
void initPulsador(void);
void EINT3_IRQHandler (void);

#endif
