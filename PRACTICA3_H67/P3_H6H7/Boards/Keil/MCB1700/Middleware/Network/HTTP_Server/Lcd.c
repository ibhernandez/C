#include "LCD.h"
#include "Driver_SPI.h"
#include "Arial12x12.h"
#include "cmsis_os2.h"                       // RTOS object definitions
#include <stdio.h>
extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI1;
//VARIABLES GLOBALES
uint8_t posicionL1=0;
uint16_t posicionL2=256;
uint16_t comienzo = 0;
int j;
unsigned char bufferLCD[512];
const int valores[11]={0xAE, 0xA2, 0xA0, 0xC8, 0x22, 0x2F, 0x40, 0xAF, 0x81, 0x17, 0xA6};
char L1[128];	//El máximo de caracteres pueden ser 128 por linea, en concreto 128 exclamaciones (!)
char L2[128];
uint8_t recoger1;
uint16_t recoger2;
int EscribeLetra_L1 (uint8_t letra){
		uint8_t i, valor1 , valor2;

	if(posicionL1<=115){	//Si la cadena se va a salir de la linea dejas de escribir, hemos tomado como referencia el caracter mas grande (@) que son 12 lineas 128 -12 -1=115
	comienzo = 25 *(letra - ' ');	//Coge el primer byte de la tabla (que es el ancho del caracter)
	for (i=0; i<12; i++){

		valor1 = Arial12x12[comienzo+i*2+1];	//guardas el byte de la parte superior del caracter
		valor2 = Arial12x12[comienzo+i*2+2];	//guardas el byte de la parte inferior del caracter
		
		bufferLCD[i+posicionL1] = valor1;				//copias en el bufferLCD el byte guardado y lo pone
		bufferLCD[i+128+posicionL1] = valor2;
	}

	
	posicionL1 = posicionL1 + Arial12x12[comienzo];	// actualiza el valor de la dirección para que escriba el siguiente caracter (le suma el ancho del caracter)
}
	return 0;	
}

int EscribeLetra_L2 (uint16_t letra){
	uint8_t i, valor1 , valor2;
	
	if(posicionL2<=371){	//Si la cadena se va a salir de la linea dejas de escribir, hemos tomado como referencia el caracter mas grande (@) que son 12 lineas 384 -12 -1=371
	
	comienzo = 25 *(letra - ' ');	//Coge el primer byte de la tabla (que es el ancho del caracter)
	for (i=0; i<12; i++){

		valor1 = Arial12x12[comienzo+i*2+1];	//guardas el byte de la parte superior del caracter
		valor2 = Arial12x12[comienzo+i*2+2];	//guardas el byte de la parte inferior del caracter
		
		bufferLCD[i+posicionL2] = valor1;				//copias en el bufferLCD el byte guardado y lo pone
		bufferLCD[i+128+posicionL2] = valor2;
	}

	
	posicionL2 = posicionL2 + Arial12x12[comienzo];	// actualiza el valor de la dirección para que escriba el siguiente caracter (le suma el ancho del caracter)
}
	return 0;	
}
void posicion_l2_reset(void){
posicionL2=256;

}
void init_lcd(void){

	GPIO_SetDir(PUERTO, PIN_RESET, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PUERTO, PIN_A0, GPIO_DIR_OUTPUT);
	GPIO_SetDir(PUERTO, PIN_CS, GPIO_DIR_OUTPUT);
	GPIO_PinWrite(PUERTO, PIN_CS, 1);
	GPIO_PinWrite(PUERTO, PIN_RESET, 1);
	GPIO_PinWrite(PUERTO, PIN_A0, 1);
	
	GPIO_PinWrite(PUERTO, PIN_RESET, 0);
	osDelay(2);
	GPIO_PinWrite(PUERTO, PIN_RESET, 1);
	osDelay(20);
	
	SPIdrv->Initialize(NULL);
	SPIdrv->PowerControl(ARM_POWER_FULL);
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8), 20000000);
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);

}

void wr_data(unsigned char data){
  
	
	GPIO_PinWrite(PUERTO, PIN_CS, 0);	// Seleccionar CS = 0;
	GPIO_PinWrite(PUERTO, PIN_A0, 1);	// Seleccionar A0 = 1;
	SPIdrv -> Send(&data,sizeof(data));		// Escribir un dato (data) 
	GPIO_PinWrite(PUERTO, PIN_CS, 1);	// Seleccionar CS = 1;
   
}

void wr_cmd(unsigned char cmd){
	
	GPIO_PinWrite(PUERTO, PIN_CS, 0);	// Seleccionar CS = 0;
  GPIO_PinWrite(PUERTO, PIN_A0, 0);	// Seleccionar A0 = 1;
  SPIdrv -> Send(&cmd,sizeof(cmd));		// Escribir un dato (data) 
  GPIO_PinWrite(PUERTO, PIN_CS, 1);	// Seleccionar CS = 1;
	

}
void LCD_reset(){
	int i;
		for(i=0;i<11;i++){
			wr_cmd(valores[i]);
	}

}

void copy_to_lcd(void){
    int i;
    wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0
    wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0
    wr_cmd(0xB0);      // Página 0
    
		GPIO_PinWrite(PUERTO, PIN_A0, 1);	// Seleccionar A0 = 1;
    for(i=0;i<128;i++){
        wr_data(bufferLCD[i]);
        }
  
     
    wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0
    wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0
    wr_cmd(0xB1);      // Página 1
    
					
    for(i=128;i<256;i++){
        wr_data(bufferLCD[i]);
        }
    
    wr_cmd(0x00);       
    wr_cmd(0x10);      
    wr_cmd(0xB2);      //Página 2
				
		
    for(i=256;i<384;i++){
        wr_data(bufferLCD[i]);
        }
    
				
		GPIO_PinWrite(PUERTO, PIN_A0, 1);	// Seleccionar A0 = 1;
    wr_cmd(0x00);       
    wr_cmd(0x10);       
    wr_cmd(0xB3);      // Pagina 3
     
    GPIO_PinWrite(PUERTO, PIN_CS,0);
				
    for(i=384;i<512;i++){
        wr_data(bufferLCD[i]);
        }
}


void escribir_frecuencia(float norte, float sur, float este, float oeste){
	posicionL1=0;
	posicionL2=256;
  int cadena1 = 0;
	float cantidad=norte+sur+este+oeste;
	cadena1=sprintf(L2,"%0.2f eur", cantidad);
		for (j=0; j<cadena1;j++){
		recoger2= (uint8_t)L2[j];
		EscribeLetra_L2(recoger2);
		}
	sprintf(L1,"Cantidad");
	for (j=0; j<strlen(L1);j++){
		recoger1= (uint8_t)L1[j];
		EscribeLetra_L1(recoger1);
		}
	copy_to_lcd();
}

void actualizar(char lcd_text[2][20+1]){
	posicionL1=0;
	posicionL2=256;
		for (j=0; j<strlen(lcd_text[0]);j++){
		recoger1= lcd_text[0][j];
		EscribeLetra_L1(recoger1);
		}
	for (j=0; j<strlen(lcd_text[1]);j++){
		recoger2= lcd_text[1][j];
		EscribeLetra_L2(recoger2);
		}
	copy_to_lcd();
}
void lcd_clean(){
	for(int i=0; i<512;i++){
		bufferLCD[i]=0;
	}
	copy_to_lcd();
}
