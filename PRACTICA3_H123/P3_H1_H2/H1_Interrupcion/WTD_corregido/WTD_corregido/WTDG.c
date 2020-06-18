#include "WTDG.h"


	
void initWTDG(int mode){
	LPC_WDT->WDCLKSEL = 0x02; // Desactivamos los dos posibles clock source
	NVIC_SetPriority(WDT_IRQn, 0x10);
	if(mode == 0) //SI esta a 0 modo interrupción
		LPC_WDT->WDMOD &= ~0x03;  //WTDG en modo interrupcion (0x00 y cuando empieze el WTDG 0x01)
	else
		LPC_WDT->WDMOD |= 0x02;		//WTDG en modo reset (0x02 y cuando empieze 0x03)
		
}

void WTDG_Start (){
	uint32_t clk ;
//setTimeOut(5000);	//5 segundos de WTDG
	LPC_WDT->WDCLKSEL = 0x1;                // Set CLK src to PCLK
  clk = SystemCoreClock / 16;    // WD has a fixed /4 prescaler, PCLK default is /4
  LPC_WDT->WDTC = 5 * (float)clk;
	LPC_WDT->WDMOD |= (1<<0);	//Habilitas el WTDG
	feedWTDG();
}
void setTimeOut(uint32_t timeout){
	uint32_t tempval = 0;
	uint32_t pclk_wdt = 4000000;
    	// Calculate TC in WDT
    	tempval  = ((((uint64_t)pclk_wdt * (uint64_t)timeout)));// / 4) / 1000000));
    	// Check if it valid
    	if (tempval >= 0xFF)
    	{
    		LPC_WDT->WDTC = tempval;
			}
}



void feedWTDG(){
		NVIC_DisableIRQ(WDT_IRQn);
	LPC_WDT->WDFEED = 0xAA;
	LPC_WDT->WDFEED = 0x55;
	NVIC_EnableIRQ(WDT_IRQn);
}
void clr_WTDG_Flag (){
	LPC_WDT->WDMOD &=~(1<<2);
}