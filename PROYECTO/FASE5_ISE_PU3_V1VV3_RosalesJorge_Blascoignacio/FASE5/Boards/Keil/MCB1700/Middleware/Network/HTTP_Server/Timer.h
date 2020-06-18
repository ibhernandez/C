#ifndef __Timer_H
#define __Timer_H


#include "LPC17xx.h"


//Uso de un timer que cada vez que llegue match haga toggle en el pin, con esto se simula un se�al entrante de frecuencia variante.
//El valor de match se cambia seg�n la temperatura.


void timer_init(void);
void timer_set(float tiempo);
float freqzin (void);
#endif
