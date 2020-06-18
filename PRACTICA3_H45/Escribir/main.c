/**********************************************************************
* $Id$		iaptest.c			2012-04-18
*//**
* @file		lpc17xx_iap.h
 * @brief	IAP demo
* @version	1.0
* @date		18. April. 2012
* @author	NXP MCU SW Application Team
* 
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#include "lpc17xx.h"
#include "lpc17xx_iap.h"

 

/** The area will be erase and program */
#define FLASH_PROG_AREA_START       0x8000
#define FLASH_PROG_AREA_SIZE		0x1000


/** The origin buffer on RAM */
#define BUFF_SIZE           256
#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=4
uint8_t buffer[BUFF_SIZE];
#else
uint8_t __attribute__ ((aligned (4))) buffer[BUFF_SIZE];
#endif

/**
Comprueba el contenido de la flash
*/
void compararFlash(){
	uint8_t *ptr; 
	int i;
	IAP_STATUS_CODE status;
	
  for ( i = 0; i < FLASH_PROG_AREA_SIZE/BUFF_SIZE; i++ ){
		ptr = (uint8_t*)(FLASH_PROG_AREA_START + i*BUFF_SIZE);
		status =  Compare(ptr, buffer,BUFF_SIZE);
		if(status != CMD_SUCCESS){
  	}
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
 int main (void)
{
	uint8_t datos[16]={0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A,0x69, 0x78, 0x87, 0x96, 0xA5, 0xB4, 0xC3, 0xD2, 0xE1, 0xF0 };
  uint32_t i;
	uint32_t flash_prog_area_sec_start;
	uint32_t flash_prog_area_sec_end;
  // Initialize

  for (i = 0;i < sizeof(buffer);i++)
  {
    buffer[i] = 0xFF;
  }
	for (i = 0;i < sizeof(datos);i++)
  {
    buffer[i] =datos[i];
  }
	
	
  flash_prog_area_sec_start = GetSecNum(FLASH_PROG_AREA_START);
  flash_prog_area_sec_end =  GetSecNum(FLASH_PROG_AREA_START + FLASH_PROG_AREA_SIZE);


	borrarSector(flash_prog_area_sec_start, flash_prog_area_sec_end);
	escribirFlash(sizeof(datos));
   return 0;
}



