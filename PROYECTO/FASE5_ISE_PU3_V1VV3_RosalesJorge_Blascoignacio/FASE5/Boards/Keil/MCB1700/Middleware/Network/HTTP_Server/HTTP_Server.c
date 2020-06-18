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
#include "Uart.h"
#include "DAC.h"
#include "Timer.h"

 //Etiquetas bloque 2
//#define ganancia 
#define res1		1000
//#define alpha 	
#define res2		923

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

//Variables bloque 2
extern  bool formato; 
extern  float medidav;
extern float medidaf;
extern float temperatura;
 uint8_t tasa_refresco[]={5, 10, 20, 30, 40, 50};
 uint8_t estado_refresco=3;
 float value; 
bool formato; 
  float medidav;
 float medidaf;
 float temperatura;

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
osThreadId_t TID_RGB;															 
extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;
 osThreadId_t TID_REC_UART;										 
/* Timers IDs */	 
osTimerId_t Timer_SNTP;
osTimerId_t	Timer_LCD; 
/* Thread declarations */
__NO_RETURN static void BlinkLed (void *arg);
__NO_RETURN static void Display  (void *arg);
__NO_RETURN static void Rtc_setTime  (void *arg);
__NO_RETURN static void Rtc_setDate  (void *arg);
__NO_RETURN static void Rtc_led  (void *arg);
__NO_RETURN static void SW_Thread  (void *arg);
__NO_RETURN static void SNTP_thread(void*args);
__NO_RETURN static void RGB_Control(void*args);
__NO_RETURN void app_main (void *arg);
__NO_RETURN void rec_Uart (void *arg);
int getNumber(char caracter);
int  escribirFlash(uint32_t size);
int borrarSector(uint32_t flash_prog_area_sec_start, uint32_t flash_prog_area_sec_end);
void getEstadoLeds(void);
void actualizarIPMAC(uint8_t ip_addr[], uint8_t mac_addr[]);
void escribirTimeStamp(uint8_t posicion);

extern void actualizarLed(uint8_t estado, uint8_t valor);
char * timeStampToString (char c[]);
static void Timer_SNTP_callback(void *args);
static void Timer_LCD_func(void *args);
/* Read analog inputs */
uint16_t AD_in (uint32_t ch) {
  int32_t val = 0;

  if (ch == 0) {
    val=medida_adc(ch);
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

}

 /*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {
  static uint8_t ip_addr[NET_ADDR_IP4_LEN];
	static uint8_t mac_addr[6];
  static char    ip_ascii[40];
  static char    buf[20+1];
	char a[23];
	int i=0;
  init_lcd();
	LCD_reset();
	lcd_clean();
  sprintf (lcd_text[0], "Starting...");
	actualizar(lcd_text);
	 /* Retrieve and print IPv4 address */
	osDelay(1000); 
	netIF_GetOption (NET_IF_CLASS_ETH,netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr)); //obtenemos los valores de ip y mac
	netIF_GetOption (NET_IF_CLASS_ETH,netIF_OptionMAC_Address, mac_addr, sizeof(mac_addr));
	actualizarIPMAC(ip_addr, mac_addr); 
	
	while(1) {
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);
		lcd_clean();
		if(formato){
			sprintf (lcd_text[0],"%0.0fHz||%0.1fC||R:%d", medidaf, temperatura,estado_refresco+1 );
		}else{
			sprintf (lcd_text[0],"%0.3fV||%0.1fC||R:%d",medidav, temperatura, estado_refresco+1);
		}
		actualizar(lcd_text);
   
		timeStampToString(a);
    for(i=0;i<23;i++){  //transmit a predefined string
    uart_TxChar(a[i]);
		osDelay(3);
		}
		if(temperatura>=25){
			redOn(255*(1-(temperatura*2-30)/100));
		}else{
			blueOn((int)255*(1-(temperatura*(-1.6)+58.9)/100));
		}
	}
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
  Thread 'Rtc_setTime': Set time
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
  Thread 'Rtc_setDate': Set date
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
	uint32_t exec1=1U;
	uint32_t senial=0;
	uint32_t time; 
	uint8_t indice_buffer=0; 
	
	//Buffer codifo para tensiones tipicas
	uint32_t tensiones[]={127, 180, 234, 288, 340, 392, 442, 492, 540, 588, 635, 681, 727, 771 ,815 ,858 ,898};
	float niveles[]={0.4093, 0.5801, 0.7541, 0.9281, 1.0957, 1.2633, 1.4244, 1.5855, 1.7402, 1.8949, 2.0464, 2.1946, 2.3429, 2.4847, 2.6265, 2.7650, 2.8939};
	float  periodos[]={5019.6078, 4155.8441, 3535.9116, 3076.9230, 2735.0427, 2461.5384, 2245.6140, 2064.5161, 1916.1676, 1787.7094, 1677.5884, 1582.2002, 1497.0760, 1423.8042, 1357.3700, 1298.1744, 1247.5633, }; //en ns , 
	float frecuencias[]={199.218, 240.625, 282.8125, 325, 365.625, 406.25, 445.312 ,484.375, 521.875, 559.375, 596.093, 632.031, 667.968, 702.343, 736.718, 770.31, 801.56 }; //En KHz
	uint8_t codigo=0; 
	DAC_Init(); //Quitar esta linea cuando se disponga de subsistema anlógico
	timer_init();//Quitar esta linea cuando se disponga de subsistema anlógico
	initPulsFase3(); //Lo mismo que con la linea de arriba, si se borra esto, no se producirian las interrupciones de los joy up y down y por tanto no se enviria estas señales
	DAC_UpdateValue(tensiones[codigo]);//iniciliazamos a -20º. Borrar tambien en caso de disponer de subsistema analogico.
	timer_set(periodos[codigo]);
	//Previsualización
	if(formato)
		sprintf (lcd_text[1],"Frecuencia %0.3f", frecuencias[codigo]);
	else
		sprintf (lcd_text[1],"Tension %0.4f", niveles[codigo]);
	DAC_UpdateValue(tensiones[codigo]);
	

	while(1){
		senial=osThreadFlagsWait( 0x03FF, osFlagsWaitAny, osWaitForever);//Importante el Any para que espere a cualquier señal del rango 0x3F
			switch (senial){

			case rg:																			
				osDelay(150);
				if(estado_refresco<sizeof(tasa_refresco)-1){
					estado_refresco++;
					osTimerDelete(Timer_LCD);
					Timer_LCD=osTimerNew(Timer_LCD_func, osTimerPeriodic, &exec1, NULL);
					if (Timer_LCD != NULL)  {
						time = tasa_refresco[estado_refresco]*100U; 
						osTimerStart(Timer_LCD, time);       // start timer
					}
					osThreadFlagsSet (TID_Display, 0x01U);
				}
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_RG);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_RG);
				break;
			case lf:						
				osDelay(150);
				if(estado_refresco>0){
					estado_refresco--;
					osTimerDelete(Timer_LCD);
					Timer_LCD=osTimerNew(Timer_LCD_func, osTimerPeriodic, &exec1, NULL);
					if (Timer_LCD != NULL)  {
						time = tasa_refresco[estado_refresco]*100U; 
						osTimerStart(Timer_LCD, time);       // start timer
					}
					osThreadFlagsSet (TID_Display, 0x01U);
				}
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_LF);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_LF);	
				break;
			case md:
				osDelay(150);
				escribirTimeStamp(indice_buffer); 
				if(indice_buffer==9)
					indice_buffer=0; 
				else
					indice_buffer++; 
			
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_MD);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
				break;
			case up:																			
				osDelay(150);
				if (codigo==16)
					codigo=0;
				else
					codigo++;
				if(formato==0){
				sprintf (lcd_text[1],"Tension %0.4f", niveles[codigo]);
				DAC_UpdateValue(tensiones[codigo]);
				}else{
				sprintf (lcd_text[1],"Frecuencia %0.3f", frecuencias[codigo]);
				timer_set(periodos[codigo]);
				}
				osThreadFlagsSet (TID_Display, 0x01U);
				//Aumentar tension 
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_UP);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_UP);
				break;
			case dw:																			
				osDelay(150);
				if (codigo==0)
					codigo=16;
				else
					codigo--;
				if(formato==0){
				sprintf (lcd_text[1],"Tension %0.4f", niveles[codigo]);
				DAC_UpdateValue(tensiones[codigo]);
				}else{
				sprintf (lcd_text[1],"Frecuencia %0.3f", frecuencias[codigo]);
				timer_set(periodos[codigo]);
				}
				osThreadFlagsSet (TID_Display, 0x01U);
				//Disminuir tension 
				LPC_GPIOINT->IO0IntEnF |=(1<<JOY_DW);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_DW);
				break;				
				
			case rgf:
				osDelay(150);
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_RG);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_RG);	
				break;
			case lff:
				osDelay(150);
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_LF);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_LF);	
				break;
			case mdf:													
				osDelay(150);												
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_MD);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_MD);	
				break;
			case upf:													
				osDelay(150);												
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_UP);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_UP);	
				break;
			case dwf:													
				osDelay(150);												
				LPC_GPIOINT->IO0IntEnR |=(1<<JOY_DW);
				LPC_GPIOINT-> IO0IntClr|=(1<<JOY_DW);	
				break;
			
			default:
				break;
		}
	}
}
/*-----------------------------------------------------------------------------
Hilo que controla los leds.
*-----------------------------------------------------------------------------*/
__NO_RETURN static void RGB_Control(void*args){
	uint8_t tiempo=100; 
	
	while(1){
	osThreadFlagsWait( 0x1000, osFlagsWaitAll,  tiempo);
//	
//		if (temperatura>=25){
//			tiempo=temperatura*2*100;
//			redOn(temperatura);
//		}else{
//			blueOn(temperatura);
//		tiempo=temperatura*1.56*100;
//	}
}
	}
/*----------------------------------------------------------------------------
  TIMER_LCD, gets temperature and refresh value every 3 sec
 *---------------------------------------------------------------------------*/
static  void Timer_LCD_func(void *args){
 
	//conv_adc(medidav+0.1);
	//medidav+=0.1;
	if(formato==0){
	medidav=medida_adc(2)*3.3/4096;
	temperatura=(((1000.0/(-1.0+(1.0/(((((medidav)-0.4)/13.923)/2.5)+(923.0/(1000.0+923.0))))))/1000.0)-1.0)/0.00385;
	}else{
	medidaf=freqzin();
	temperatura=(((1000.0/(-1.0+(1.0/(((((((medidaf/1000.0)-0.1)*3.3/0.8)-0.4)/13.923)/2.5)+(923.0/(1000.0+923.0))))))/1000.0)-1.0)/0.00385;
	
	}
	osThreadFlagsSet (TID_Display, 0x01U);
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
  rec_Uart, recibe un comando por uart para actualizar el valor de temperatura
 *---------------------------------------------------------------------------*/
static __NO_RETURN void rec_Uart (void *args){
  char trama [20];  //buffer con los datos recibidos
	char reply [30];
	for(int i=0; i<19; i++) //inicializacion del buffer a x
	   trama[i]='x';
	float tmp, tension, periodo, freqz, r_eq;
	uint8_t codigo;
	while(1) {
		osThreadFlagsWait (0x001U, osFlagsWaitAny, osWaitForever);
    getCaracteres(trama);


//		//si el indice llega a 18, vuelve a 0 si la trama es distinto a SIMULA Temp= 12.0 ºC, se descarta.
		if(trama[12] == '-')
			tmp=(getNumber(trama[13])*10.0+getNumber(trama[14])+getNumber(trama[16])*0.1)*-1.0;
		else
			tmp=getNumber(trama[13])*10.0+getNumber(trama[14])+getNumber(trama[16])*0.1;
		
		//actualizamos la tension y la frecuencia segun la trama recibida.
		//conversion a tension
		//(1000.0/(-1.0+1.0/(((((medidav)-0.4)/13.923)/2.5)+(923.0/(1000.0+923.0)))));
		r_eq= (1000.0*(1.0+(0.00385*tmp)));
		tension=2.5*((r_eq/(r_eq+1000.0))-(923.0/1923.0));
		tension=(tension*13.923)+0.4;//en voltios
		codigo=tension*1024/3.3;
		//conversion a periodo en ns
		freqz=1000000.0*(0.1+(tension*(0.8/3.3)));//en HZ
		periodo=(1.0/freqz)*1000000000;//en ns
		//actualizacion de valores.
		DAC_UpdateValue(codigo);
		timer_set(periodo);
				//devolvemos info con la trama recibida T=xx.x; V=y.yyyy; F=zzz.zzzz
		sprintf(reply, "\nT=%0.1f; V=%0.4f; F=%0.1f\n",tmp, tension,freqz);
		for(int i=0; i<sizeof(reply);i++){
			uart_TxChar(reply[i]);
			osDelay(3);
		}
		if(formato==0){
			sprintf (lcd_text[1],"Tension %0.4f", tension);
		}else{
			sprintf (lcd_text[1],"Frecuencia %0.1f", freqz/1000);
		}
		
		osThreadFlagsSet (TID_Display, 0x01U);
		

		
	}
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	medidav=0.4f;
	temperatura=-20;
	medidaf=200;
	formato=1; 
	uint32_t exec1=1U;
	uint32_t time; 
	initRTC();
	init_RGB();
	uart_init(9600);
  netInitialize ();
		//INicializacion del RTC
	getEstadoLeds();
  TID_Led     		= osThreadNew (BlinkLed, NULL, NULL);
  TID_Display 		= osThreadNew (Display,  NULL, NULL);
	TID_Rtc_setTime	= osThreadNew (Rtc_setTime, 		 NULL, NULL);
	TID_Rtc_setDate	= osThreadNew (Rtc_setDate, 		 NULL, NULL);
	TID_Rtc_led			= osThreadNew (Rtc_led, 		 NULL, NULL);
	TID_SNTP				= osThreadNew (SNTP_thread, NULL, NULL); 
	TID_RGB					= osThreadNew	(RGB_Control, NULL, NULL); 
	TID_Thread_sw		= osThreadNew (SW_Thread, 	NULL, NULL);
	TID_REC_UART 		= osThreadNew (rec_Uart, NULL, NULL);
		//Creacion de un timer de 3 segundos para refrescar el ldc
	osThreadFlagsSet (TID_Display, 0x01U);
	Timer_LCD=osTimerNew(Timer_LCD_func, osTimerPeriodic, &exec1, NULL);
	 if (Timer_LCD != NULL)  {
    time = 3*1000U; 
	  osStatus_t status = osTimerStart(Timer_LCD, time);       // start timer
  }
	
	//Creacion de un timer de 3 min para llamar al sntp
	Timer_SNTP=osTimerNew(Timer_SNTP_callback, osTimerPeriodic, &exec1, NULL);
	 if (Timer_SNTP != NULL)  {
    time = 60*3*1000U;// 
	  osStatus_t status = osTimerStart(Timer_SNTP, time);       // start timer
  }
	 
	osThreadFlagsWait (0x80U, osFlagsWaitAny, 13000);//Esperamos 13 segundos
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

void escribirTimeStamp(uint8_t posicion){ //del 0 al 10
	
	uint8_t read[12+12*10] ;//12 del ip/mac+leds y 120 timestamps
	uint32_t i;
	uint32_t flash_prog_area_sec_start;
	uint32_t flash_prog_area_sec_end;

	
	
	
	//Limpiamos buffer:
  for (i = 0;i < sizeof(buffer);i++)
  {
    buffer[i] = 0x00;
  }
	//Leemos los bytes que interesan antes de borrar: 
   memcpy(read, (void *)FLASH_PROG_AREA_START, 12+12*10); 
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

    buffer[12+posicion*12]=getDia();
		buffer[13+posicion*12]=getMes();
		buffer[14+posicion*12]=(int)getAnio()/100;
		buffer[15+posicion*12]=(int)getAnio()%100;
		buffer[16+posicion*12]=getHora();
		buffer[17+posicion*12]=getMin();
		buffer[18+posicion*12]=getSeg();
		if(temperatura<0)
			buffer[19+posicion*12]=0x01;	
		else
			buffer[19+posicion*12]=0x00;
		if(temperatura>0){
			buffer[20+posicion*12]=0x00+(int)temperatura;
			if(((int)(temperatura*1000)%100)>=50)
				buffer[21+posicion*12]=0x01+(int)(10*temperatura)%10;
			else
				buffer[21+posicion*12]=0x00+(int)(10*temperatura)%10;
		}else{
			buffer[20+posicion*12]=0x00+(int)temperatura*-1;
			if(((int)(temperatura*1000)%100)>=50)
				buffer[21+posicion*12]=0x00+(int)(-10*temperatura)%10;
			else
				buffer[21+posicion*12]=0x01+(int)(-10*temperatura)%10;
		}
		
		if(formato==1)
			buffer[22+posicion*12]=0x01;
		else
			buffer[22+posicion*12]=0x00;

	//Escribimos de 0nuevo: 
	escribirFlash(sizeof(read));

	
}

/**
Obtiene el estado de los leds para la inicializacion
*/
void getEstadoLeds(void){
	uint8_t read[12] ;


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
/*Pasae el timeStamp a string
*/
char * timeStampToString (char c[]){
	uint8_t signo=0, medida=0;
	if(temperatura<0)
		signo =1;
	if(formato==1)
		medida=1;
	sprintf(c, "\n\r%0.2d%0.2d%0.4d%0.2d%0.2d%0.2d%d%0.1f%d", getDia(),getMes(), getAnio(), getHora(), getMin(), getSeg(), signo, temperatura, medida);
	return c;
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
