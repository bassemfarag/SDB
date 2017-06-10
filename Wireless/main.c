//#include "driverlib.h"
#include "msp430fr5969.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

//#define   Num_of_Results   8
volatile uint8_t test;
unsigned long ADC_ResultA7 = 0;
unsigned long ADC_ResultA10 = 0;
//volatile unsigned char TXData[10]={0};
volatile uint16_t TXData;
volatile unsigned short indexT = 0;
volatile unsigned short indexR = 0;
volatile unsigned char outbuffer[10];
void initiate (void);
void main(void)
{
    WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    FRCTL0 = 0xA500 | ((1) << 4);
    CSCTL0_H = 0xA5;
    CSCTL1 = DCOFSEL_6 | DCORSEL;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
    initiate();

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
      if(UCA1IFG & UCTXIFG){
      sprintf(outbuffer, "%d \n \n \n \n \n ",TXData);
      UCA1TXBUF = outbuffer;                     // Load data onto buffer
      __delay_cycles(5000);
      }
  }
}
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
            printf("A7 = %d\n",ADC_ResultA7);
            printf("A10 = %d\n",ADC_ResultA10);
            P1OUT ^= BIT0;/* + BIT3 + BIT4 + BIT5; */             // Toggle P1.0
            if(test == 0){
                P1OUT &= ~BIT4;
                P1OUT &= ~BIT5;
                P3OUT &= ~BIT4;
                P4OUT &= ~BIT3;
                printf("Charging Battery number %d \n", test);
                test = 1;
            }
            else if(test == 1){
                P1OUT |= BIT4;
                P1OUT &= ~BIT5;
                P3OUT |= BIT4;
                P4OUT &= ~BIT3;
                printf("Charging Battery number %d \n", test);
                test = 2;
            }
            else if(test == 2){
                P1OUT |= BIT5;
                P1OUT &= ~BIT4;
                P4OUT |= BIT3;
                P3OUT &= ~BIT4;
                printf("Charging Battery number %d \n", test);
                test = 3;
            }
            else if(test == 3){
                P1OUT |= BIT5;
                P1OUT |= BIT4;
                P3OUT |= BIT4;
                P4OUT |= BIT3;
                printf("Charging Battery number %d \n", test);
                test = 0;
            }
           // P1OUT |= BIT5;
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
    }
}

void initiate(void){

    P1DIR |= BIT0 + BIT3 + BIT4 + BIT5;       // P1.0 = LED, P1.3 = MUX1 EN , P1.4 = MUX1 S0, P1.5 = MUX1 S1;
    P3DIR |= BIT0 + BIT4;                      //P3.0 = MUX1 EN , P3.4 = MUX1 S0;
    P4DIR |= BIT3;                             // P4.3 = MUX1 S1;
    P1OUT &= ~BIT0;
    P1REN |= BIT1;                            // Enable Pull-up resistor.
    P1OUT |= BIT1;                               // Set as pull up.
    P1IES |= BIT1;                               // interrupt high to low
    P1IE |= BIT1;                                // P1.3 interrupt enabled
    P1IFG &= ~BIT1;                            // P1.3 interrupt flag cleared
    P1OUT &= ~BIT4;
    P1OUT &= ~BIT5;
    P3OUT &= ~BIT4;
    P4OUT &= ~BIT3;
    //P2SEL0 |= BIT4;                             // Enable A/D channel inputs (P2.4 channel A7) (Not needed)
   // P2SEL1 |= BIT4;
  /*  P4SEL0 |= BIT2;                             // Enable A/D channel inputs (P4.2 channel A10) (Not needed)
    P4SEL1 |= BIT2;*/
   // P2DIR |= BIT4;
   // P4DIR |= BIT2;
    while(REFCTL0 & REFGENBUSY);
    REFCTL0 |= REFVSEL_1 + REFON;
    ADC12CTL0 &= ~ADC12ENC;

    P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation
    P2SEL0 &= ~(BIT5 | BIT6);
    PM5CTL0 &= ~LOCKLPM5;

      ADC12CTL0 |= ADC12MSC + ADC12SHT0_2 + ADC12SHT1_2; // Turn on ADC12, one SHI trigger is needed, extend sampling time (256 ADC12CLK cycles) // to avoid overflow of results
      ADC12CTL1 |= ADC12SSEL_3;                   // clk source: SMCLK
      ADC12CTL1 |= ADC12SHP +ADC12CONSEQ_1;       // Use sampling timer, repeated sequence
      ADC12MCTL0 |= /*ADC12VRSEL_1 + */ ADC12INCH_7;                 // ref+=AVcc, channel = A7
      ADC12MCTL1 |= /*ADC12VRSEL_1 + */ ADC12INCH_10 + ADC12EOS;            // ref+=AVcc, channel = A10
      test = 0 ;
      //ADC12IER0 = 0x0480;                         // Enable ADC12IFG.7 & ADC12IFG.10
     //Configure USCI_A1 for UART mode
      /*UCA1CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA1CTL1 |= UCSSEL__SMCLK;                // CLK = SMCLK
    UCA1BR0 = 8;                              // 1000000/115200 = 8.68
    UCA1MCTLW = 0xD600;                       // 1000000/115200 - INT(1000000/115200)=0.68
    UCA1BR1 = 0;
    UCA1CTL1 &= ~UCSWRST;    */                 // release from reset
    while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle
       ADC12CTL2 |= ADC12RES_2;                   //  12 bit resolution        // ADC12RES_x defines the conversion result resolution.
       ADC12CTL0 |= ADC12ON;                       // Enable ADC12_B. This can only be modified if ADC12ENC = 0.
     __bis_SR_register(GIE);       // Enter LPM0, Enable interrupts
  //  __no_operation();                         // For debugger

}
