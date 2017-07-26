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
// Define the position of each battery relative to the available ADC pins.
#define Position_One 100 //This Battery is connected to Pin A7.
#define Position_Two 200 //This Battery is connected to Pin A10.
//Define Charging and Discharging thresholds.
#define Charging_Threshold 3000
#define Discharging_Threshold 1000
typedef enum {Not_Connected,Charging,Discharging} Status;
typedef struct Battery{
    Status Battery_Status;
   // volatile unsigned long Voltage[10];
    volatile unsigned long Voltage;
}Battery;

extern void Init_Clock(void);
extern void Init_Gpio(void);
extern void Init_Timer0(void);
extern void Init_Timer1(void);
extern void Init_ADC(void);
extern void Init_UART(void);
extern void Init_Battery(Battery*, Status , uint8_t);
extern uint8_t Circuit_Logic(Battery*);
extern void uartSend(unsigned char *pucData, unsigned char ucLength);


#endif /* MAIN_H_ */
