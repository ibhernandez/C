#include "LPC17xx.h"

/** Peripheral clock divider bit position for DAC */
#define	CLKPWR_PCLKSEL_DAC  		((uint32_t)(22))

typedef enum
{
	DAC_MAX_CURRENT_700uA = 0, 	/*!< The settling time of the DAC is 1 us max,
								and the maximum	current is 700 uA */
	DAC_MAX_CURRENT_350uA		/*!< The settling time of the DAC is 2.5 us
								and the maximum current is 350 uA */
} DAC_CURRENT_OPT;

/** If this bit = 0: The settling time of the DAC is 1 microsecond max,
 * and the maximum current is 700 microAmpere
 * If this bit = 1: The settling time of the DAC is 2.5 microsecond
 * and the maximum current is 350 microAmpere */
#define DAC_BIAS_EN			((uint32_t)(1<<16))

/** After the selected settling time after this field is written with a
new VALUE, the voltage on the AOUT pin (with respect to VSSA)
is VALUE/1024 × VREF */
#define DAC_VALUE(n) 		((uint32_t)((n&0x3FF)<<6))

void 	DAC_Init();
void    DAC_UpdateValue (uint32_t dac_value);
void    DAC_SetBias (uint32_t bias);

