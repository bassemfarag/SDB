// First_Battery is connected to Pin A7.
// Second_Battery is connected to Pin A10.
#include <functions.h>
void main(void)
{
    WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    ADC_ResultA7 = 0;
    ADC_ResultA10 = 0;
    Charge_Bat_Num = Discharge_Bat_Num = logic = length = 0;
    Button4_Pressed = 0;
    Button1_Pressed = 0;
    counter = counter1 = count = count1 = 0;
    Init_Clock();
    Init_Gpio();
    Init_Timer0();
    Init_Timer1();
    Init_ADC();
    Init_UART();
    /*printf("status is ssss %d\n", Discharging);
    printf("status is ssss %d\n", Charging);
    printf("status is ssss %d\n", Not_Connected);
    printf("status das %d\n", Vl2020.Battery_Status);
    printf("voltage das %d\n", Vl2020.Voltage);*/

    Init_Battery(&Vl2020,Discharging ,Position_One);
    Init_Battery(&Capacitor,Charging ,Position_Two);

  while(1){

  }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(!Button1_Pressed){
        Button1_Pressed = 1;
        __delay_cycles(100000);
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
                     printf("First Battery Voltage = %d mV \n",ADC_ResultA7*3600/4095);
                     printf("Second Battery Voltage = %d mV\n",ADC_ResultA10*3600/4095);              sprintf(outbuffer, "\r\nCharging battery number %d",Charge_Bat_Num);
                    // length= sizeof(outbuffer);
                     //uartSend(outbuffer,length);                   // Load data onto buffer
                     sprintf(outbuffer, "\r\nVoltage = %d mV",ADC_ResultA7*3600/4095);
                     length= sizeof(outbuffer);
                     uartSend(outbuffer,17);
                     Charge_Bat_Num++;
                     if(Charge_Bat_Num>3)
                         Charge_Bat_Num = 0;
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
        default:break;
        }
        Button1_Pressed = 0;
    }
}

            /********************************************************************************************/

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    if(!Button4_Pressed){
        Button4_Pressed = 1;
        __delay_cycles(100000);
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
             sprintf(outbuffer, "\r\nDischarging battery number %d",Discharge_Bat_Num);
             length= sizeof(outbuffer);
             uartSend(outbuffer,length);                   // Load data onto buffer
             sprintf(outbuffer, "\r\nVoltage = %d mV",ADC_ResultA7*3000/4095);
             length= sizeof(outbuffer);
             uartSend(outbuffer,17);
             Charge_Bat_Num++;
             if(Charge_Bat_Num>3)
                 Charge_Bat_Num = 0;
             P4IFG &= ~BIT5;           // P1.1 interrupt flag cleared
        break;
        case P4IV_P4IFG6:                        /* P1.6 */
        break;
        case P4IV_P4IFG7:                       /* P1.7 */
        break;
        default:break;
        }
        Button4_Pressed = 0;
    }
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        if (UCA1RXBUF == 'a'){ // 'u' received?
         // uartSend(outbuffer,17);                   // Load data onto buffer
         test = 1;
         printf("Test = 1\n");
         logic=Circuit_Logic(&Vl2020);
         if(logic == 1){
             printf("ERROR OCCURED at Position %d \n",Vl2020.Position);
         }
         logic=Circuit_Logic(&Capacitor);
         if(logic == 1){
             printf("ERROR OCCURED at position %d \n", Capacitor.Position);
         }

        }
        if (UCA1RXBUF == 'b'){
           // uartSend(outbuffer,17);                   // Load data onto buffer
         test = 2;
         printf("Test = 2\n");
         logic=Circuit_Logic(&Vl2020);
         logic=Circuit_Logic(&Capacitor);
        }
        if (UCA1RXBUF == 'c'){
           // uartSend(outbuffer,17);                   // Load data onto buffer
         test = 3;
         printf("Test = 3\n");
         logic=Circuit_Logic(&Vl2020);
         logic=Circuit_Logic(&Capacitor);
        }
      break;
    case USCI_UART_UCTXIFG:break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
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
 // P1OUT ^= BIT0;
  counter = 0;
  ++count;
 // printf("%d minute(s) passed \n", count);
  }
 // TA0CCR0 += 500000;                         // Add Offset to TA0CCR0
}

//Timer1A0 interrupt service routine

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  ++counter1;
  if(counter1 == 2){
     // ADC12CTL0 |= ADC12ENC | ADC12SC;                    // Enable and start conversions
     // while (ADC12CTL1 & ADC12BUSY);
     // ADC_ResultA7 = ADC12MEM0;
     // ADC_ResultA10 = ADC12MEM1;
      logic=Circuit_Logic(&Vl2020);
      if(logic==1){
          printf("ERROR TOOK PLACE!!!");
          logic=0;
      }
      logic=Circuit_Logic(&Capacitor);
      if(logic==1){
          printf("ERROR TOOK PLACE!!!");
          logic=0;
      }

     /*
      sprintf(outbuffer, "%d:",ADC_ResultA7*2750/4095);
      uartSend(outbuffer,5);
      sprintf(outbuffer, "%d \r\n",ADC_ResultA10*2750/4095);
      uartSend(outbuffer,7);
      ++count1;
     // printf("%d second(s) passed \n", count1);
      */
      P4OUT ^= BIT6;
      counter1 = 0;
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








