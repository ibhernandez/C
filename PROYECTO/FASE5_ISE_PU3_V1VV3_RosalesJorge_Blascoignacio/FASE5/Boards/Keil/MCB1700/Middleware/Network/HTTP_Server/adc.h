#ifndef __ADC_H
#define __ADC_H

#include "LPC17xx.h"
#define PUERTO_ADC	1
#define PIN_ADC			31
#define P0_ADC	0
#define PIN_2V5			23
//#define PIN_0V4			24
#define PIN_DATA		25

void init_adc(void);
uint16_t medida_adc(uint8_t peticion);

#endif
