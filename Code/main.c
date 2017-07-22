#include "driverlib.h"
#include "msp430fr5969.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//#define   Num_of_Results   8
uint8_t Charge_Bat_Num = 0;
uint8_t Discharge_Bat_Num = 0;
uint16_t ADC_ResultA7 = 0;
uint16_t ADC_ResultA10 = 0;
//volatile unsigned char TXData[10]={0};
//volatile uint16_t TXData;
//volatile unsigned short indexT = 0;
//volatile unsigned short indexR = 0;
unsigned char outbuffer[50];
volatile int counter  = 0;
volatile int count = 0;
void initiate (void);
void circuit_logic(void);
void uartSend(unsigned char *pucData, unsigned char ucLength);
//unsigned char sendString[] = "I am working ";
uint8_t length=0;
void main(void)
{
    WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;// | DCORSEL;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
    initiate();
  //  unsigned char sendString[] = "I am working ";
    //unsigned char length=13;

 // P1OUT |= 0x01;
  while(1){

    /*  ADC12CTL0 |= ADC12ENC | ADC12SC;                    // Enable and start conversions
      while (ADC12CTL1 & ADC12BUSY);
      ADC_ResultA7 = ADC12MEM0;
      ADC_ResultA10 = ADC12MEM1;
      printf("A7 = %d\n",ADC_ResultA7);
      printf("A10 = %d\n",ADC_ResultA10);*/
      //  P1OUT ^= 0x01;
      //__delay_cycles(100000);
     //    while(!(UCA1IFG & UCTXIFG));
       //  sprintf(outbuffer, "%d \n \n \n \n \n ",TXData);
     //  uartSend(sendString,length);                   // Load data onto buffer
     //  __delay_cycles(5000);

  }
}



#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    switch (__even_in_range(P1IV,P1IV_P1IFG7))
    {
    case P1IV_NONE:                                 /*NONE  */
    break;
    case P1IV_P1IFG0:                              /* P1.0 */
    break;
    case P1IV_P1IFG1:                             /* P1.1 */
            ADC12CTL0 |= ADC12ENC | ADC12SC;                    // Enable and start conversions
            while (ADC12CTL1 & ADC12BUSY);
            ADC_ResultA7 = ADC12MEM0;
            ADC_ResultA10 = ADC12MEM1;
            P1OUT ^= BIT0;/* + BIT3 + BIT4 + BIT5; */             // Toggle P1.0
            if(Charge_Bat_Num == 0){
                P1OUT &= ~BIT4;
                P1OUT &= ~BIT5;
                printf("Charging Battery number %d \n", Charge_Bat_Num);
            }
            else if(Charge_Bat_Num == 1){
                P1OUT |= BIT4;
                P1OUT &= ~BIT5;
                printf("Charging Battery number %d \n", Charge_Bat_Num);
            }
            else if(Charge_Bat_Num == 2){
                P1OUT |= BIT5;
                P1OUT &= ~BIT4;
                printf("Charging Battery number %d \n", Charge_Bat_Num);
            }
            else if(Charge_Bat_Num == 3){
                P1OUT |= BIT5;
                P1OUT |= BIT4;
                printf("Charging Battery number %d \n", Charge_Bat_Num);
            }

                 printf("A7 = %d mV \n",ADC_ResultA7*3610/4095);
                 printf("A10 = %d mV\n",ADC_ResultA10*3610/4095);

                 sprintf(outbuffer, "Charging battery number %d \r\n",Charge_Bat_Num);
                 length= sizeof(outbuffer);
                 uartSend(outbuffer,length);                   // Load data onto buffer
                 sprintf(outbuffer, "Voltage = %d mV \r\n",ADC_ResultA7*3610/4095);
                 length= sizeof(outbuffer);
                 uartSend(outbuffer,18);
                 Charge_Bat_Num++;
                 if(Charge_Bat_Num>3)
                     Charge_Bat_Num = 0;

            /*if(UCA1IFG & UCTXIFG){
           //sprintf(outbuffer, "A7 is %d \n \n \n \n ",ADC_ResultA7);
            UCA1TXBUF = outbuffer;                     // Load data onto buffer
            printf(outbuffer);
           // __delay_cycles(5000);
            }*/
           // P1OUT |= BIT5;
         //   uartSend(sendString,length);                   // Load data onto buffer
            P1IFG &= ~BIT1;           // P1.1 interrupt flag cleared
            break;
    case P1IV_P1IFG2:                          /* P1.2 */
    break;
    case P1IV_P1IFG3:                          /* P1.3 */
    break;
    case P1IV_P1IFG4:                          /* P1.4 */
    break;
    case P1IV_P1IFG5:                         /* P1.5 */
    break;
    case P1IV_P1IFG6:                        /* P1.6 */
    break;
    case P1IV_P1IFG7:                       /* P1.7 */
    break;
    default:
        break;
    }}

            /********************************************************************************************/

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    switch (__even_in_range(P4IV,P4IV_P4IFG7))
    {
    case P4IV_NONE:                                 /*NONE  */
    break;
    case P4IV_P4IFG0:                              /* P4.0 */
    break;
    case P4IV_P4IFG1:                             /* P4.1 */
    break;
    case P4IV_P4IFG2:                          /* P4.2 */
    break;
    case P4IV_P4IFG3:                          /* P4.3 */
    break;
    case P4IV_P4IFG4:                          /* P4.4 */
    break;
    case P4IV_P4IFG5:                         /* P4.5 */
        P4OUT ^= BIT6;/* + BIT3 + BIT4 + BIT5; */             // Toggle P4.6
        if(Discharge_Bat_Num == 0){
            P3OUT &= ~BIT4;
            P4OUT &= ~BIT3;
            printf("Discharging Battery number %d \n", Discharge_Bat_Num);
            Discharge_Bat_Num = 1;
        }
        else if(Discharge_Bat_Num == 1){
            P3OUT |= BIT4;
            P4OUT &= ~BIT3;
            printf("Discharging Battery number %d \n", Discharge_Bat_Num);
            Discharge_Bat_Num = 2;
        }
        else if(Discharge_Bat_Num == 2){
            P4OUT |= BIT3;
            P3OUT &= ~BIT4;
            printf("Discharging Battery number %d \n", Discharge_Bat_Num);
            Discharge_Bat_Num = 3;
        }
        else if(Discharge_Bat_Num == 3){
            P3OUT |= BIT4;
            P4OUT |= BIT3;
            printf("Discharging Battery number %d \n", Discharge_Bat_Num);
            Discharge_Bat_Num = 0;
        }
        P4IFG &= ~BIT5;           // P1.1 interrupt flag cleared
    break;
    case P4IV_P4IFG6:                        /* P1.6 */
    break;
    case P4IV_P4IFG7:                       /* P1.7 */
    break;
    default:
        break;
    }
}
// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  ++counter;
  if(counter == 1459){
  P1OUT ^= BIT0;
  counter = 0;
  ++count;
  printf("%d minute(s) passed \n", count);
  }
  TA0CCR0 += 500000;                         // Add Offset to TA0CCR0
}


void initiate(void){

    P1DIR |= BIT0 + BIT3 + BIT4 + BIT5;       // P1.0 = LED, P1.3 = MUX1 EN , P1.4 = MUX1 S0, P1.5 = MUX1 S1;
    P3DIR |= BIT0 + BIT4;                      //P3.0 = MUX1 EN , P3.4 = MUX1 S0;
    P4DIR |= BIT3 + BIT6;                             // P4.3 = MUX1 S1;
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT6;

    P1REN |= BIT1;                            // Enable Pull-up resistor.
    P1OUT |= BIT1;                               // Set as pull up.
    P1IES |= BIT1;                               // interrupt high to low
    P1IE |= BIT1;                                // P1.3 interrupt enabled
    P1IFG &= ~BIT1;                            // P1.3 interrupt flag cleared
    P4REN |= BIT5;                            // Enable Pull-up resistor.
   P4OUT |= BIT5;                               // Set as pull up.
   P4IES |= BIT5;                               // interrupt high to low
   P4IE |= BIT5;                                // P1.3 interrupt enabled
   P4IFG &= ~BIT5;                            // P1.3 interrupt flag cleared
    P1OUT &= ~BIT4;
    P1OUT &= ~BIT5;
    P3OUT &= ~BIT4;
    P4OUT &= ~BIT3;

     TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
     TA0CCR0 = 500000;
     TA0CTL = TASSEL__SMCLK | MC__CONTINOUS;   // SMCLK, continuous mode

    //P2SEL0 |= BIT4;                             // Enable A/D channel inputs (P2.4 channel A7) (Not needed)
   // P2SEL1 |= BIT4;
  /*  P4SEL0 |= BIT2;                             // Enable A/D channel inputs (P4.2 channel A10) (Not needed)
    P4SEL1 |= BIT2;*/
   // P2DIR |= BIT4;
   // P4DIR |= BIT2;
    while(REFCTL0 & REFGENBUSY);
    REFCTL0 |= REFVSEL_1 + REFON;
    ADC12CTL0 &= ~ADC12ENC;
    P2SEL0 &= ~(BIT5 | BIT6);
    P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation

    PM5CTL0 &= ~LOCKLPM5;

      ADC12CTL0 |= ADC12MSC + ADC12SHT0_2 + ADC12SHT1_2; // Turn on ADC12, one SHI trigger is needed, extend sampling time (256 ADC12CLK cycles) // to avoid overflow of results
      ADC12CTL1 |= ADC12SSEL_3;                   // clk source: SMCLK
      ADC12CTL1 |= ADC12SHP +ADC12CONSEQ_1;       // Use sampling timer, repeated sequence
      ADC12MCTL0 |= /*ADC12VRSEL_1 + */ ADC12INCH_7;                 // ref+=AVcc, channel = A7
      ADC12MCTL1 |= /*ADC12VRSEL_1 + */ ADC12INCH_10 + ADC12EOS;            // ref+=AVcc, channel = A10
      Charge_Bat_Num = 0;
      Discharge_Bat_Num = 0;
      //ADC12IER0 = 0x0480;
     //Configure USCI_A1 for UART mode
      UCA1CTL1 = UCSWRST;
     //Set BRCLK = SMCLK
     UCA1CTL1 |= UCSSEL_2;
     //Values found using table for 1Mhz and 9600 baudrate
     UCA1BR0=6;
     UCA1BR1=0;
     UCA1MCTLW = 0x20 << 8;
     UCA1MCTLW |= 8 << 4;
     UCA1MCTLW |= UCOS16;
     //UCA0 out of reset
     UCA1CTL1 &= ~UCSWRST;
      while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle
      ADC12CTL2 |= ADC12RES_2;                   //  12 bit resolution        // ADC12RES_x defines the conversion result resolution.
      ADC12CTL0 |= ADC12ON;                       // Enable ADC12_B. This can only be modified if ADC12ENC = 0.
      __bis_SR_register(GIE);// + LPM0_bits);       // Enter LPM0, Enable interrupts
  //  __no_operation();                         // For debugger

}


void uartSend(unsigned char *pucData, uint8_t ucLength)
{
  while(ucLength>0)
  {
    // Wait for TX buffer to be ready for new data
    while(!(UCA1IFG & UCTXIFG));
    // Push data to TX buffer
    UCA1TXBUF = *pucData;
    // Update variables
    ucLength--;
    pucData++;
  }
}
/*
void circuit_logic(void){

}
*/

/*
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
 static unsigned int index = 0;

 switch(__even_in_range(ADC12IV,34))
 {
 case  0: break;                           // Vector  0:  No interrupt
 case  2: break;                           // Vector  2:  ADC overflow
 case  4: break;                           // Vector  4:  ADC timing overflow
 case  6: break;                           // Vector  6:  ADC12IFG0
 case  8: break;                           // Vector  8:  ADC12IFG1
 case 10: break;                           // Vector 10:  ADC12IFG2
 case 12: break;                               // Vector 12:  ADC12IFG3
 case 14: break;                           // Vector 14:  ADC12IFG4
 case 16: break;                           // Vector 16:  ADC12IFG5
 case 18: break;                           // Vector 18:  ADC12IFG6
 case 20:                                 // Vector 20:  ADC12IFG7
     ADC = ADC12MEM7;           // Move A7 results, IFG is cleared
     index++;                                // Increment results index, modulo; Set Breakpoint1 here
     __delay_cycles(5000);
     P1OUT ^= BIT0;
     if (index == 7)
     {
       (index = 0);
     }
     __bis_SR_register_on_exit(LPM0_bits);
     break;
 case 22: break;                           // Vector 22:  ADC12IFG8
 case 24: break;                           // Vector 24:  ADC12IFG9
 case 26:                            // Vector 26:  ADC12IFG10
     A1results[index] = ADC12MEM10;           // Move A10 results, IFG is cleared
     index++;                                // Increment results index, modulo; Set Breakpoint1 here
     __delay_cycles(5000);
         P1OUT ^= BIT0;
     if (index == 7)
     {
       (index = 0);
     }
     __bis_SR_register_on_exit(LPM0_bits);
     break;
 case 28: break;                           // Vector 28:  ADC12IFG11
 case 30: break;                           // Vector 30:  ADC12IFG12
 case 32: break;                           // Vector 32:  ADC12IFG13
 case 34: break;                           // Vector 34:  ADC12IFG14
 default: break;
 }
}
*/








