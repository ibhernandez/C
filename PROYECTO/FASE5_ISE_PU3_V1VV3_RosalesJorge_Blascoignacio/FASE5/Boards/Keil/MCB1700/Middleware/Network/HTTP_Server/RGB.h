#ifndef __RGB_H
#define __RGB_H

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"

void init_RGB(void);
void redOn(float temp);
void RGBoff(void);
void blueOn(float temp); 
#endif
