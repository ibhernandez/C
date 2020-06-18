#ifndef __TIMER_H
#define __TIMER_H

#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"


void timer0_config(uint32_t tiempo_ms);
void timer0_start_irq(void);
int getPrescalarForUs(uint8_t timerPclkBit);
#endif
