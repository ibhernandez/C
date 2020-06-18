#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "DAC.h"


/*********************************************************************//**
 * @brief 		Initial ADC configuration
 * 					- Maximum	current is 700 uA
 * 					- Value to AOUT is 0
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @return 		None
 ***********************************************************************/
void DAC_Init()
{
	LPC_PINCON->PINSEL1 |= (1 << 21);
	/* Set default clock divider for DAC */
	//CLK del reloj a CCLK/4 por defecto
	//Set maximum current output
	DAC_SetBias(DAC_MAX_CURRENT_700uA);
}

/*********************************************************************//**
 * @brief 		Update value to DAC
 * @param[in] 	DACx pointer to LPC_DAC_TypeDef, should be: LPC_DAC
 * @param[in] 	dac_value : value 10 bit to be converted to output
 * @return 		None
 ***********************************************************************/
void DAC_UpdateValue (uint32_t dac_value)
{
	uint32_t tmp;
	
	tmp = LPC_DAC->DACR & DAC_BIAS_EN;
	tmp |= DAC_VALUE(dac_value);
	// Update value
	LPC_DAC->DACR = tmp;
}
void DAC_SetBias (uint32_t bias)
{

	LPC_DAC->DACR &=~DAC_BIAS_EN;
	if (bias  == DAC_MAX_CURRENT_350uA)
	{
		LPC_DAC->DACR |= DAC_BIAS_EN;
	}
}


