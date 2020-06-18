
#include "LPC17xx.h"
#include "Board_LED.h"

void initWTDG();
void WTDG_Start();
void feedWTDG ();
void clr_WTDG_Flag ();
extern void WDT_IRQHandler(void);
void setTimeOut(uint32_t timeout);
int WDT_ReadTimeOutFlag (void);