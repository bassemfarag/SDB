/*
 * main.h
 *
 *  Created on: Jul 22, 2017
 *      Author: Bassem Farag
 */
#include "driverlib.h"
#include "msp430fr5969.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#ifndef MAIN_H_
#define MAIN_H_

extern void Init_Clock(void);
extern void Init_Gpio(void);
extern void Init_Timer0(void);
extern void Init_Timer1(void);
extern void Init_ADC(void);
extern void Init_UART(void);
extern void circuit_logic(void);
extern void uartSend(unsigned char *pucData, unsigned char ucLength);


#endif /* MAIN_H_ */
