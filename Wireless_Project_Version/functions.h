/*
 * main.h
 *
 *  Created on: Jul 22, 2017
 *      Author: Bassem Farag
 */
//#include "driverlib.h"
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
#define Charging_Threshold 3600
#define Discharging_Threshold 2000
/*#define Not_Connected 0
#define Charging 1
#define Discharging 2
*/
#define Num_of_Results   50
volatile uint8_t Charge_Bat_Num;
volatile uint8_t Discharge_Bat_Num;
//volatile uint8_t i;
//volatile uint8_t p=0;
volatile uint8_t Button1_Pressed;
volatile uint8_t Button4_Pressed;
unsigned char outbuffer[Num_of_Results];
volatile int counter;
volatile int counter1;
volatile int count;
volatile int count1;
uint8_t logic;
uint8_t length;
uint8_t yyy;
typedef enum {Not_Connected = 0,Charging = 2,Discharging = 1} Status;
typedef struct{
      Status Battery_Status;
   // volatile unsigned long Voltage[10];
      uint8_t Position;
    volatile unsigned long Voltage;
}Battery;

Battery Vl2020;
Battery Capacitor;

extern void Init_Clock(void);
extern void Init_Gpio(void);
extern void Init_Timer0(void);
extern void Init_Timer1(void);
extern void Init_ADC(void);
extern void Init_UART(void);
extern void Init_Battery(Battery*, Status , uint8_t/*, uint16_t*/);
extern uint8_t Circuit_Logic(Battery*);
extern void uartSend(unsigned char *pucData, unsigned char ucLength);
extern void Switch(Battery*);
volatile unsigned long Result;
volatile unsigned long ADC_ResultA7;   /* ADC_ResultA7, ADC_ResultA10 must be long to avoid overflow when converting into mV*/
volatile unsigned long ADC_ResultA10;
volatile unsigned long ADC_ResultA3;
uint8_t test;
#endif /* MAIN_H_ */
