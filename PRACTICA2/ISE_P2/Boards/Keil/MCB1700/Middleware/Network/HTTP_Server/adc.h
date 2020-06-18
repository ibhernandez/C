#ifndef __ADC_H
#define __ADC_H

#include "LPC17xx.h"
#define PUERTO_ADC	1
#define PIN_ADC			31

void init_adc(void);
uint8_t medida_adc(void);


#endif
