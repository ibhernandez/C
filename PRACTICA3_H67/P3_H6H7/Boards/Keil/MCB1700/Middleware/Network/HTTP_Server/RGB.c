	
#include "RGB.h"
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"

#define portRGB	2
#define green		2
#define red			3
#define blue 		1

void init_RGB(void){
	PIN_Configure (portRGB, green, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	GPIO_SetDir   (portRGB,green, GPIO_DIR_OUTPUT);
	PIN_Configure (portRGB, red, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	GPIO_SetDir   (portRGB,red, GPIO_DIR_OUTPUT);
	PIN_Configure (portRGB, blue, PIN_FUNC_0, PIN_PINMODE_PULLDOWN, PIN_PINMODE_NORMAL);
	GPIO_SetDir   (portRGB,blue, GPIO_DIR_OUTPUT);
	RGBoff();
}

void redOn(void){
	GPIO_PinWrite (portRGB, red, 0);
}

void greenOn(void){
	GPIO_PinWrite (portRGB, green, 0);
}

void blueOn(void){
	GPIO_PinWrite (portRGB, blue, 0);
}

void RGBoff(void){
	GPIO_PinWrite (portRGB, green, 1);
	GPIO_PinWrite (portRGB, red, 1);
	GPIO_PinWrite (portRGB, blue, 1);
}
