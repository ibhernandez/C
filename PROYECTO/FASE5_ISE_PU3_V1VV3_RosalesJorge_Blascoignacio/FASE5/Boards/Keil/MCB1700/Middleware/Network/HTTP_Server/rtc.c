#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "RTC.h"
#include <time.h>
#include <stdio.h>
#include "mktime.h"
uint8_t valor =1;
#define RTCCIF	0
#define RTCALF	1
#define s		0
#define mi	0
#define h		0
#define d		1
#define m		1
#define y		2020 //desde 1970
#define dw		1
#define wy		1


void resetHoraFecha0(void){
  LPC_RTC->SEC=s;
  LPC_RTC->MIN=mi;
  LPC_RTC->HOUR=h;
  LPC_RTC->MONTH=m;
  LPC_RTC->YEAR=y;
  LPC_RTC->DOM=d;
  LPC_RTC->DOW=dw;
  LPC_RTC->DOY=wy;
}
void setHora(uint8_t seg, uint8_t min, uint8_t hor){
  LPC_RTC->SEC=seg;
  LPC_RTC->MIN=min;
  LPC_RTC->HOUR=hor;
}
void setFecha(uint8_t dia, uint8_t mes, uint16_t anio){
  LPC_RTC->MONTH=mes;
  LPC_RTC->YEAR=anio;
  LPC_RTC->DOM=dia;
} 
uint8_t getSeg(void){
return ((LPC_RTC->CTIME0<< 26) >> 26);
}
uint8_t getMin(void){
  return (((LPC_RTC->CTIME0>>8)<<26)>>26);
}
uint8_t getHora(void){
  return (((LPC_RTC->CTIME0>>16)<<27)>>27);
}
uint8_t getDia(void){
  return ((LPC_RTC->CTIME1<<27)>>27);
}
uint8_t getMes(void){
  return (((LPC_RTC->CTIME1>>8)<<27)>>27);
}
uint16_t getAnio(void){
  return (((LPC_RTC->CTIME1>>16)<<20)>>20);
}
//devuelves segundos pasados desde 1 de enero de 1970
uint32_t getTotalSeconds(void){
uint32_t segundos;
	segundos=unix_time_in_seconds(getSeg(), getMin(), getHora(), getDia(), getMes(), getAnio()); 
	return segundos;
}


void IAlarma(void){
LPC_RTC->CIIR =0x01;
LPC_RTC-> AMR =0xFE;
LPC_RTC->ALSEC=0;
}
void RTC_IRQHandler (void){ 
  if(LPC_RTC->ILR==(1<<RTCALF)){//la de 1min
    LPC_RTC->ILR|=(1<<RTCALF);
		//osThreadFlagsSet (TID_Rtc_led, 0x02);
	//Enviar señal para el led
  }else if(LPC_RTC->ILR==(1<<RTCCIF)){
    LPC_RTC->ILR|=(1<<RTCCIF);
	//Enviar señal para el lcd
		//osThreadFlagsSet (TID_Rtc_setTime, 0x01);
		
	}

}
void initRTC(void){
	NVIC_DisableIRQ(RTC_IRQn);
	
    /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(RTC_IRQn, ((0x01<<3)|0x01));
	LPC_RTC->CCR = ((1 << SBIT_CTCRST ) | (1 << SBIT_CCALEN));
	LPC_RTC->CALIBRATION = 0x00;
	LPC_RTC->CCR = 1;    /* Enable the clock for RTC */
		IAlarma();  
	LPC_RTC->ILR|=(1<<RTCCIF) | (1<<RTCALF);//LImpiamos los flags

	NVIC_EnableIRQ(RTC_IRQn);

}
