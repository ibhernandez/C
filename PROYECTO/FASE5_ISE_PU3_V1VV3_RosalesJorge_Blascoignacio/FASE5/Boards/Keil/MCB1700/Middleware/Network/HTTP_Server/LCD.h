#ifndef __LCD_H
#define __LCD_H

#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "stdio.h"
#define PUERTO			0
#define PIN_RESET			8
#define PIN_A0 				6 
#define PIN_CS			18



//FUNCIONES
int EscribeLetra_L1 (uint8_t letra);
int EscribeLetra_L2 (uint16_t letra);
void posicion_l2_reset(void);
void init_lcd(void);
void wr_data(unsigned char data);
void wr_cmd(unsigned char cmd);
void LCD_reset(void);
void copy_to_lcd(void);
void escribir_frecuencia(float norte, float sur, float este, float oeste);
void actualizar(char lcd_text[2][20+1]);
void lcd_clean(void);

#endif
