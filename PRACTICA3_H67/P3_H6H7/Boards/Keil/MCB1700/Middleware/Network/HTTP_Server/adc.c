#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "adc.h"
	uint16_t adc_result=0x00;
void init_adc(){
	PIN_Configure(PUERTO_ADC, PIN_ADC, PIN_FUNC_3, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);	//Selecciona el puerto y el pin del ADC 0.5(1.31) y la función que realiza que es 1(PWM)
	LPC_SC->PCONP |= (1 << 12);      /* Enable CLOCK for internal ADC controller */
	LPC_ADC->ADCR = ((1<<21) | (1<<8));  //Set the clock and Power ON ADC module	/* Select Channel 5 by setting 0th bit of ADCR */
}

uint8_t medida_adc(){
	//Añadir un delay para que la medida se tome cada x tiempo y no cada ns
	
		LPC_ADC->ADCR |= (1<<5);	//seleccionamos el canal 5
		LPC_ADC->ADCR	|= (1<<24);
		while(LPC_ADC->ADGDR &= 31u == 0);  /* wait till conversion completes */
		adc_result = (LPC_ADC->ADGDR >> 12u) & 0xf;   /*Read the 12bit adc result*/
	return adc_result;
}
