/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cmsis_os2.h"                  // CMSIS RTOS definitions
#include "rl_net.h"                     // Network definitions
#include "LCD.h"
#include "adc.h"
#include "Board_LED.h"
#include "RTC.h"
#include "SNTP.h"
#include "switch.h"
#include "lpc17xx.h"
#include "lpc17xx_iap.h"
#include "RGB.h"

 

/** The area will be erase and program */
#define FLASH_PROG_AREA_START       0x78000
#define FLASH_PROG_AREA_SIZE		0x1000


/** The origin buffer on RAM */
#define BUFF_SIZE           256
#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=4
uint8_t buffer[BUFF_SIZE];
#else
uint8_t __attribute__ ((aligned (4))) buffer[BUFF_SIZE];
#endif



// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
static uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};



extern uint16_t AD_in          (uint32_t ch);
extern uint8_t  get_button     (void);
extern void     netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);

extern bool LEDrun;
extern char lcd_text[2][20+1];
extern char time_text[2][20+1];
extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;




bool LEDrun;
char lcd_text[2][20+1] = { "LCD line 1",
                           "LCD line 2" };
char time_text[2][20+1] = { "XX:XX:XX",
                           "XX/XX/XXXX" };
/* Thread IDs */
osThreadId_t TID_Display;
osThreadId_t TID_Led;
osThreadId_t TID_Rtc_setTime;
osThreadId_t TID_Rtc_setDate;
osThreadId_t TID_Rtc_led;
osThreadId_t TID_SNTP; 
osThreadId_t TID_Thread_sw;
													 
													 
/* Timers IDs */	 
osTimerId_t Timer_SNTP;
osTimerId_t	TID_Timer_1s; 
/* Thread declarations */
__NO_RETURN static void BlinkLed (void *arg);
__NO_RETURN static void Display  (void *arg);
__NO_RETURN static void Rtc_setTime  (void *arg);
__NO_RETURN static void Rtc_setDate  (void *arg);
__NO_RETURN static void Rtc_led  (void *arg);
__NO_RETURN static void SW_Thread  (void *arg);
__NO_RETURN static void SNTP_thread(void*args);
__NO_RETURN void app_main (void *arg);
int getNumber(char caracter);
int  escribirFlash(uint32_t size);
int borrarSector(uint32_t flash_prog_area_sec_start, uint32_t flash_prog_area_sec_end);
void getEstadoLeds(void);
void actualizarIPMAC(uint8_t ip_addr[], uint8_t mac_addr[]);
extern void actualizarLed(uint8_t estado, uint8_t valor);
extern void getAdcSetRGB(uint8_t valor);
static void Timer_SNTP_callback(void *args);
													 					 
/* Read analog inputs */
uint16_t AD_in (uint32_t ch) {
  int32_t val = 0;

  if (ch == 0) {
    val=medida_adc();
  }
  return ((uint16_t)val);
}
/* Read digital inputs */
/**

uint8_t get_button (void) {
  return ((uint8_t)Buttons_GetState ());
}
*/
/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
		 sprintf (lcd_text[0],"IP address:");
    sprintf (lcd_text[1],"%s", val);//ip4_ntoa (val));
    osThreadFlagsSet (TID_Display, 0x01);
  }
}

 /*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {
  static uint8_t ip_addr[NET_ADDR_IP4_LEN];
	static uint8_t mac_addr[6];
  static char    ip_ascii[40];
  static char    buf[20+1];
  init_lcd();
	LCD_reset();
	lcd_clean();
  sprintf (lcd_text[0], "Starting...");
  sprintf (lcd_text[1], "           ");
	actualizar(lcd_text);
	 /* Retrieve and print IPv4 address */
	osDelay(1000); 
	netIF_GetOption (NET_IF_CLASS_ETH,netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr)); //obtenemos los valores de ip y mac
	netIF_GetOption (NET_IF_CLASS_ETH,netIF_OptionMAC_Address, mac_addr, sizeof(mac_addr));
	
	actualizarIPMAC(ip_addr, mac_addr); 
	
	sprintf (buf, "IP4:%-16s",ip_ascii);
	sprintf (lcd_text[0],"IP address:");
	sprintf (lcd_text[1],"%s", netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii)));
	actualizar(lcd_text);
  while(1) {
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
    actualizar(lcd_text);
		lcd_clean();
		actualizar(lcd_text);
		}
//	//	//Incializacion del lcd
//  init_lcd();
//	LCD_reset();
//	lcd_clean();
//	//Precarga de hora y fecha
//	sprintf (lcd_text[0],"HORA:  %0.2d:%0.2d:%0.2d", getHora(), getMin(), getSeg());
//  sprintf (lcd_text[1],"FECHA: %0.2d:%0.2d:%0.4d",getDia(), getMes(), getAnio());
//  actualizar(lcd_text);
//	while(1){
//	osThreadFlagsWait (0x01U, osFlagsWaitAny, 1000);
//	sprintf (lcd_text[0],"HORA:  %0.2d:%0.2d:%0.2d", getHora(), getMin(), getSeg());
//  sprintf (lcd_text[1],"FECHA: %0.2d:%0.2d:%0.4d",getDia(), getMes(), getAnio());
//  actualizar(lcd_text);
//	}
  }

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static __NO_RETURN void BlinkLed (void *arg) {
  const uint8_t led_val[8] = { 0x01,0x03, 0x06, 0x0C, 0x08, 0x0C, 0x06, 0x03};
  int cnt = 0;

  while(1) {
    // Every 100 ms
    if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0;
      }
    }
    osDelay (200);
  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LED 4 every minute for 5 seconds
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Rtc_led (void *arg) {
  const uint8_t led_val[2] = { 0x00,0x08};
  int cnt = 0;
  while(1) {
		osThreadFlagsWait (0x02U, osFlagsWaitAny, osWaitForever);
		if(!LEDrun) {
			while(getSeg()<=5){
				osThreadFlagsWait (0x80U, osFlagsWaitAny, 100);//timeout de 100ms
				if(getSeg()>=5){  
					LED_SetOut (led_val[0]);
				}else{
					LED_SetOut (led_val[cnt]);
					if (++cnt >= sizeof(led_val)) {
						cnt = 0;
					}
				}
			}
		}
  }
}

/*----------------------------------------------------------------------------
  Thread 'Rtc_setTime': Refresh lcd every sec
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Rtc_setTime (void *arg) {

	char date[8];
	uint8_t  seg, min, hor ;
	while(1){

	osThreadFlagsWait (0x20U, osFlagsWaitAny, osWaitForever);
	sprintf (date, "%s",time_text[0]);
	date[2] = date[5] = '\0';
	hor=10*getNumber(date[0])+getNumber(date[1]);
	min=10*getNumber(date[3])+getNumber(date[4]);
	seg=10*getNumber(date[6])+getNumber(date[7]);
	setHora(seg, min, hor);
	}
	

}
/*----------------------------------------------------------------------------
  Thread 'Rtc_setTime': Refresh lcd every sec
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Rtc_setDate (void *arg) {

	char date[10];
	uint8_t  dia, mes;
	uint16_t anio;
	while(1){

	osThreadFlagsWait (0x20U, osFlagsWaitAny, osWaitForever);
	sprintf (date, "%s",time_text[1]);
	date[2] = date[5] = '\0';
	dia=10*getNumber(date[0])+getNumber(date[1]);
	mes=10*getNumber(date[3])+getNumber(date[4]);
	anio=1000*getNumber(date[6])+100*getNumber(date[7])+10*getNumber(date[8])+getNumber(date[9]);
	setFecha(dia, mes, anio);
	}
}

/*----------------------------------------------------------------------------
  SW_Thread Controls sw bounce and resets time and date
 *---------------------------------------------------------------------------*/

static __NO_RETURN  void SW_Thread  (void *arg){
	initPulsador(); 
	bool estado=true;

	while(1){
		osThreadFlagsWait( 0x0100, osFlagsWaitAll, osWaitForever);
			if(estado){
				resetHoraFecha0(); 
				osThreadFlagsWait (0x80U, osFlagsWaitAny, 250);
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_MD);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
				estado=!estado;

			}else{													
				osThreadFlagsWait (0x80U, osFlagsWaitAny, 250);											
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_MD);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
				estado=!estado;
		}
	}
}

/*----------------------------------------------------------------------------
  TIMER_SNTP, gets time every 3 minutes.
 *---------------------------------------------------------------------------*/
static  void Timer_SNTP_callback(void *args){

	osThreadFlagsSet (TID_SNTP, 0x04);
}

/*----------------------------------------------------------------------------
  SNTP_Thread, gets date every 3 min
 *---------------------------------------------------------------------------*/
static __NO_RETURN void SNTP_thread(void *args){

	while(1) {
		osThreadFlagsWait (0x04U, osFlagsWaitAny, osWaitForever);
		get_time (); 
	 //Encender y apagar el led 3
		osThreadFlagsWait (0x10U, osFlagsWaitAny, osWaitForever);
     if(!LEDrun) 
			LED_SetOut (0x04);
		osThreadFlagsWait (0x80U, osFlagsWaitAny, 1000);
		if(!LEDrun) 
		 LED_SetOut (0x00);
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	uint32_t exec1=1U;
	uint32_t time; 
	initRTC();
	init_RGB();
  netInitialize ();
		//INicializacion del RTC
 getEstadoLeds();
  TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  TID_Display = osThreadNew (Display,  NULL, NULL);
	TID_Rtc_setTime		= osThreadNew (Rtc_setTime, 		 NULL, NULL);
	TID_Rtc_setDate		= osThreadNew (Rtc_setDate, 		 NULL, NULL);
	TID_Rtc_led			= osThreadNew (Rtc_led, 		 NULL, NULL);
	TID_SNTP				= osThreadNew (SNTP_thread, NULL, NULL); 

	TID_Thread_sw		= osThreadNew (SW_Thread, 	NULL, NULL);
	//Creacion de un timer de 3 min 
	Timer_SNTP=osTimerNew(Timer_SNTP_callback, osTimerPeriodic, &exec1, NULL);
	 if (Timer_SNTP != NULL)  {
    time = 15000U;//60*3*1000U;// 
	  osStatus_t status = osTimerStart(Timer_SNTP, time);       // start timer
  }
	 
	osThreadFlagsWait (0x80U, osFlagsWaitAny, 13000);//Esperamos 13 segundoss
	osThreadFlagsSet (TID_SNTP, 0x04);//mandamos señal para actualizar hora del sntp
	
  osThreadExit();
}


/**
Actualiza el valor de la ip y mac en la flash al inicio.*/
void actualizarIPMAC(uint8_t ip_addr [], uint8_t mac_addr []){
	
	uint8_t read[12] ;
	uint32_t i;
	uint32_t flash_prog_area_sec_start;
	uint32_t flash_prog_area_sec_end;
 
	
	
	//Limpiamos buffer:
  for (i = 0;i < sizeof(buffer);i++)
  {
    buffer[i] = 0xFF;
  }
	//Leemos los bytes que interesan antes de borrar: 
   memcpy(read, (void *)FLASH_PROG_AREA_START, 12); 
	//Guardamos datos leidos:
		for (i = 0;i < sizeof(read);i++)
  {
    buffer[i] =read[i];
  }
	//Borramos sector:
  flash_prog_area_sec_start = GetSecNum(FLASH_PROG_AREA_START);
  flash_prog_area_sec_end =  GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);
	//Borramos:
	borrarSector(flash_prog_area_sec_start, flash_prog_area_sec_end);

	//Actualizamos los valores de ip y mac:
			for (i = 0;i < 4;i++)
  {
    buffer[i] =ip_addr[i];
  }
				for (i = 4;i < 6+4;i++)
  {
    buffer[i] =mac_addr[i-4];
  }
	//Aprovechamos la escritura para establecer el valor de comparacion para el adc
	buffer[11]=120; 
	
	//Escribimos de nuevo: 
	escribirFlash(sizeof(read));

	
}
/**
Actualiza el estado y el valor de los leds en la flash*/
void actualizarLed(uint8_t estado, uint8_t valor){
	
	uint8_t read[12] ;
	uint32_t i;
	uint32_t flash_prog_area_sec_start;
	uint32_t flash_prog_area_sec_end;
 
	
	
	//Limpiamos buffer:
  for (i = 0;i < sizeof(buffer);i++)
  {
    buffer[i] = 0xFF;
  }
	//Leemos los bytes que interesan antes de borrar: 
   memcpy(read, (void *)FLASH_PROG_AREA_START, 12); 
	//Guardamos datos leidos:
		for (i = 0;i < sizeof(read);i++)
  {
    buffer[i] =read[i];
  }
	//Borramos sector:
  flash_prog_area_sec_start = GetSecNum(FLASH_PROG_AREA_START);
  flash_prog_area_sec_end =  GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);
	//Borramos:
	borrarSector(flash_prog_area_sec_start, flash_prog_area_sec_end);

	//Actualizamos el estado de los leds y el valor:

	if(estado==0){
    buffer[10] =valor;
  }else{
		buffer[10]=valor+16;
	}
	//Escribimos de nuevo: 
	escribirFlash(sizeof(read));

	
}
/**
Obtiene el estado de los leds para la inicializacion
*/
void getEstadoLeds(void){
	uint8_t read[12] ;
	uint32_t i;

	//Leemos los bytes que interesan antes de borrar: 
   memcpy(read, (void *)FLASH_PROG_AREA_START, 12); 
	//Guardamos datos leidos:
	if(read[10]>=16 ){
		LEDrun=true;
	}else{
		LEDrun=false;
		LED_SetOut(read[10]-16);
	}
}

/**
Comparar el estado del adc y actuar en consecuencia.
*/
void getAdcSetRGB(uint8_t valor){
	
		uint8_t read[12] ;
	uint32_t i;

	//Leemos los bytes que interesan antes de borrar: 
   memcpy(read, (void *)FLASH_PROG_AREA_START, 12); 
	//Guardamos datos leidos:
	if(read[11]<=valor*255/15 ){
		//Para convertir el valor del adc a uno entre 0 y 255
		// hay que multiplicar por 255 y dividir por 15
		blueOn(); 
	}else{
		RGBoff(); 
	}
}

/**
Escribe en la flash
Devuelve 1 en caso de operacion correcta y 0 en caso de error.
*/
int  escribirFlash(uint32_t size){
	uint8_t ret=1;
	uint8_t *ptr; 
	int i;
	IAP_STATUS_CODE status;
	uint32_t veces=size/IAP_WRITE_256; 
	//Si veces  es mayor que 256 se hara mas de una vez el bucle for, de lo contrario solo se hara 1 vez
	// en este caso 16%256=
	for ( i = 0; i < veces+1; i++){//FLASH_PROG_AREA_SIZE/BUFF_SIZE; i++ ){
		ptr = (uint8_t*)(FLASH_PROG_AREA_START + i*BUFF_SIZE);
		status =  CopyRAM2Flash(ptr, buffer,IAP_WRITE_256);
		if(status != CMD_SUCCESS){
			ret=0; 
		}
	}
	
	return ret; 
}


/**
Borra el sector que se encuentra entre los valores start y end
Devuelve 0 en caso de error y 1 en caso de que la operacion sea correcta

*/
int borrarSector(uint32_t flash_prog_area_sec_start, uint32_t flash_prog_area_sec_end){
	uint32_t result[2];
  IAP_STATUS_CODE status;
	int ret=1;
	status = EraseSector(flash_prog_area_sec_start, flash_prog_area_sec_end); 
	if(status != CMD_SUCCESS){
		ret=0;
	}
	//COMPROBAR QUE SE HA BORRADO EL SECTOR
	status = BlankCheckSector(flash_prog_area_sec_start, flash_prog_area_sec_end, &result[0], &result[1]);
	if(status != CMD_SUCCESS){
		ret=0; 
		if(status == SECTOR_NOT_BLANK){
			ret=0;
		}
	}
	return ret; 
}


int getNumber(char caracter){
	int ret=0;
	switch (caracter){
		case '1':
			ret= 1;
			break;
		case '2':
			ret= 2;
			break;
		case '3':
			ret= 3;
				break;
		case '4':
			ret= 4;
				break;
		case '5':
			ret= 5;
				break;
		case '6':
			ret= 6;
				break;
		case '7':
			ret= 7;
				break;
		case '8':
			ret= 8;	
				break;
		case '9':
			ret= 9;			
				break;
		default: 
			ret= 0;
			break;
	}
	return ret;
}
