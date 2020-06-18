	
#include "RGB.h"
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"


#define portRGB	2
#define green		2
#define red			3
#define blue 		1
//El rojo es PWM1.4
//El azul es PWM1.2
void init_RGB(void){
	PIN_Configure (portRGB, red, PIN_FUNC_1, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	PIN_Configure (portRGB, blue, PIN_FUNC_1, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);

	LPC_PWM1->TCR = (1<<1); //Reset en la cuenta
	LPC_PWM1->PR  =  0x0;               /* No Prescalar */
	LPC_PWM1->MCR = 2;
  LPC_PWM1->MR0=255; 
  LPC_PWM1->MR2=255; 
  LPC_PWM1->MR4=255; 
	//habilita el cambio en el ciclo de trabajo del mr2 y mr4
	LPC_PWM1->LER |= 1;
	LPC_PWM1->LER |= (1<<2)|(1<<4);
	LPC_PWM1->TCR =1| (1<<3);//Habilitamos la cuenta y el pwm
	LPC_PWM1->PCR |= (1<<10)|(1<<12);

}

void redOn(float temp){
	
	LPC_PWM1->MR2 = 255; //Ciclo de trabajo para el azul
	LPC_PWM1->MR4 = temp;//255*(1-(temp*2-30)/100); 	//Ciclo de trabajo para el rojo
	//habilita el cambio en el ciclo de trabajo del mr2 y mr4
	LPC_PWM1->LER |= (1<<2)|(1<<4);

	
}


void blueOn(float temp){
	LPC_PWM1->MR4 = 255; 	//Ciclod de trabajo para el rojo
	LPC_PWM1->MR2 = temp;//(int)255*(1-(temp*(-1.6)+58.9)/100); //Ciclo de trabajo para el azul
	//habilita el cambio en el ciclo de trabajo del mr2 y mr4
	LPC_PWM1->LER |= (1<<2)|(1<<4);

}


