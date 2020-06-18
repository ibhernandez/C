#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "adc.h"
	uint16_t adc_result=0x00;

	//Peticion es el numero del canal del que se necesita medida
	//Peticion 0 Medida de 2.5V
	//Peticion 1 Medida de 0.4V
	//Peticion 2 Medida de DATO temp
	//Peticion 5 Medida de Pot2
	
void init_adc(){
	PIN_Configure(PUERTO_ADC, PIN_ADC, PIN_FUNC_3, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);	
	//PIN_Configure(PUERTO_ADC, 30, PIN_FUNC_3, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);	 
	//PIN_Configure(P0_ADC, PIN_2V5, PIN_FUNC_1, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	//PIN_Configure(P0_ADC, PIN_0V4, PIN_FUNC_1, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	PIN_Configure(P0_ADC, PIN_DATA, PIN_FUNC_1, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	
	LPC_SC->PCONP |= (1 << 12);      /* Enable CLOCK for internal ADC controller */
	LPC_ADC->ADCR = ((1<<21) | (1<<8));  //Set the clock and Power ON ADC module	/* Select Channel 5 by setting 0th bit of ADCR */
}

uint16_t medida_adc(uint8_t peticion){
		LPC_ADC->ADCR =(LPC_ADC->ADCR  & 0xFFFFFF00)| (0x01<<peticion);	//seleccionamos el canal 5
	LPC_ADC->ADCR	|= (1<<24); //Comenzar la medida
		while((LPC_ADC->ADGDR &= 31u == 0));  /* wait till conversion completes */
	if(peticion==5){
			adc_result = (LPC_ADC->ADGDR >> 12u) & 0xF; 
	LPC_ADC->ADCR	&= ~(1<<24); //Paramos medida la medida
	}else{
	adc_result = (LPC_ADC->ADGDR >> 4u) & 0xFFF; 
	LPC_ADC->ADCR	&= ~(1<<24); //Paramos medida la medida
	}
	return adc_result;
}
