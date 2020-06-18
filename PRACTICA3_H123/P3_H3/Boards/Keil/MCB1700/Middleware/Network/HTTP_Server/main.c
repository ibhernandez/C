/*------------------------------------------------------------------------------
 * Example main module
 * Copyright (c) 2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: Main module
 *----------------------------------------------------------------------------*/

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "WTDG.h"
#include "LPC17xx.h"                    // Device header
#include "adc.h"
#include "LCD.h"
#include "Board_LED.h"


extern       void           app_main (void *arg);
extern const osThreadAttr_t app_main_attr;

/*------------------------------------------------------------------------------
 * main function
 *----------------------------------------------------------------------------*/
int main(void) {
	initWTDG(2); 							
	WTDG_Start();
	
	//Inicializar el watchdog
	SystemCoreClockUpdate ();                     // Update System Core Clock info
  LED_Initialize ();                            // Initialize LED
	init_adc();                      							// Initialize ADC
	osKernelInitialize ();                        // Initialize CMSIS-RTOS2
  osThreadNew (app_main, NULL, &app_main_attr); // Create application main thread
  osKernelStart ();                             // Start thread execution

  for (;;) {}
}
