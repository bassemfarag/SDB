#include <functions.h>





void Init_Battery(Battery* Battery_Init, Status Bat_Status, uint8_t Position/*, uint16_t voltage*/){
    Battery_Init->Battery_Status = Bat_Status;
    Battery_Init->Position = Position;
    Result = 0;
    test = 0;
   // Battery_Init->Voltage = voltage;
    //printf("voltage is %d\n",Battery_Init->Voltage);
    switch(Battery_Init->Position){
    case Position_One:
      /*  ADC12CTL0 |= ADC12ENC | ADC12SC;      // Enable and start conversions
        while (ADC12CTL1 & ADC12BUSY);
        Result = ADC12MEM7*3000/4095;
        Battery_Init->Voltage = Result;*/
        Battery_Init->Voltage = 1900;
        printf("status is %d \n", Battery_Init->Battery_Status);
        if(Battery_Init->Battery_Status == Charging)
            printf("charging \n");
        if(Battery_Init->Battery_Status == Discharging)
            printf("Discharging\n");
        break;
    case Position_Two:
       /* ADC12CTL0 |= ADC12ENC | ADC12SC;     // Enable and start conversions
        while (ADC12CTL1 & ADC12BUSY);
        Result = ADC12MEM10*3000/4095;
        Battery_Init->Voltage = Result;*/
        Battery_Init->Voltage = 2950;
        printf("status second is %d \n", Battery_Init->Battery_Status);
        if(Battery_Init->Battery_Status == Charging)
            printf("charging \n");
        if(Battery_Init->Battery_Status == Discharging)
            printf("Discharging\n");
        break;
    default:
        break;

    }

}



 void Init_Clock(void){
           CSCTL0_H = CSKEY >> 8;
           CSCTL1 = DCOFSEL_0;// | DCORSEL;
           CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
           CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
           CSCTL0_H = 0;
    }

void Init_Gpio(void){
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
    P4IFG &= ~BIT5;                            // P4.5 interrupt flag cleared
    P1OUT &= ~BIT4;
    P1OUT &= ~BIT5;
    P3OUT &= ~BIT4;
    P4OUT &= ~BIT3;
    P2SEL0 &= ~(BIT5 | BIT6);
    P2SEL1 |= BIT5 | BIT6;                    // USCI_A1 UART operation
    PM5CTL0 &= ~LOCKLPM5;
}
void Init_Timer0(void){
        TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
     //   TA0CCR0 = 500000;
        TA0CTL = TASSEL__SMCLK | MC__CONTINOUS;   // SMCLK, continuous mode
}

void Init_Timer1(void){
        TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
     //   TA1CCR0 = 5000;
        TA1CTL = TASSEL__SMCLK | MC__CONTINOUS | ID_3;   // SMCLK, up mode // divided by 8 = 1.9 Hz
}




void Init_ADC(void){
    while(REFCTL0 & REFGENBUSY);
    REFCTL0 |= REFVSEL_1 + REFON;
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 |= ADC12MSC + ADC12SHT0_2 + ADC12SHT1_2; // Turn on ADC12, one SHI trigger is needed, extend sampling time (256 ADC12CLK cycles) // to avoid overflow of results
    ADC12CTL1 |= ADC12SSEL_3;                   // clk source: SMCLK
    ADC12CTL1 |= ADC12SHP +ADC12CONSEQ_1;       // Use sampling timer, repeated sequence
    ADC12MCTL0 |= /*ADC12VRSEL_1 + */ ADC12INCH_7;                 // ref+=AVcc, channel = A7
    ADC12MCTL1 |= /*ADC12VRSEL_1 + */ ADC12INCH_10 + ADC12EOS;            // ref+=AVcc, channel = A10
    while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator to settle
    ADC12CTL2 |= ADC12RES_2;                   //  12 bit resolution        // ADC12RES_x defines the conversion result resolution.
    ADC12CTL0 |= ADC12ON;                       // Enable ADC12_B. This can only be modified if ADC12ENC = 0.
    __bis_SR_register(GIE);// + LPM0_bits);       // Enter LPM0, Enable interrupts
      //  __no_operation();                         // For debugger
}

void Init_UART(void){
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
     UCA1IE |= UCRXIE;
}

void uartSend(unsigned char *pucData, uint8_t ucLength)
{
  while(ucLength>0){
    // Wait for TX buffer to be ready for new data
    while(!(UCA1IFG & UCTXIFG));
    // Push data to TX buffer
    UCA1TXBUF = *pucData;
    ucLength--;
    pucData++;
  }
}

uint8_t Circuit_Logic(Battery* Battery_Check){

    switch(Battery_Check->Position){
        case Position_One:
          /* ADC12CTL0 |= ADC12ENC | ADC12SC;                    // Enable and start conversions
            while (ADC12CTL1 & ADC12BUSY);
            Result = ADC12MEM10*3000/4095;
            Battery_Check->Voltage = Result;*/
            if(test ==1){
            Battery_Check->Voltage = 3200;
            printf("Position One Voltage = %d \n",Battery_Check->Voltage);
           //printf("status is %d \n", Battery_Check->Battery_Status);

            }
            else if(test ==2){
            Battery_Check->Voltage = 900;
            printf("Position one Voltage = %d \n",Battery_Check->Voltage);
            }
            else if(test ==3){
            Battery_Check->Voltage = 1100;
            printf("Position one voltage = %d \n",Battery_Check->Voltage);
            }
            break;
        case Position_Two:
          /*  ADC12CTL0 |= ADC12ENC | ADC12SC;                    // Enable and start conversions
            while (ADC12CTL1 & ADC12BUSY);
            Result = ADC12MEM7*3000/4095;
            Battery_Check->Voltage = Result;*/
            if(test ==1){
            Battery_Check->Voltage = 899;
            printf("Position two voltage = %d \n",Battery_Check->Voltage);
            }
            else if(test ==2){
            Battery_Check->Voltage = 2900;
            printf("Position two voltage = %d \n",Battery_Check->Voltage);
            }
            else if(test ==3){
            Battery_Check->Voltage = 3100;
            printf("Position two voltage = %d \n",Battery_Check->Voltage);
            }
            break;
        default:
            break;
        }

    if(Battery_Check->Battery_Status == Not_Connected){
        return 0;
    }
    else if(Battery_Check->Battery_Status == Charging){
        if(Battery_Check->Voltage /*+ Battery_Check->Voltage[0])/2 */>= Charging_Threshold){
            printf("Charging threshold reached, Switching\n");
            Switch(Battery_Check);
            return 0;
        }
        else if(Battery_Check->Voltage/*[0] + Battery_Check->Voltage[0])/2*/ < (Discharging_Threshold - 100) ){
            return 1;
            }
        else{
            return 0;
        }
    }
    else if (Battery_Check->Battery_Status == Discharging){
        if(Battery_Check->Voltage /*[0] + Battery_Check->Voltage[0])/2*/ <= Discharging_Threshold){
            Switch(Battery_Check);
            printf("Discharging threshold reached, Switching\n");
            return 0;
        }
        else if(Battery_Check->Voltage/*[0] + Battery_Check->Voltage[0])/2*/ >= (Charging_Threshold + 100 ) ){
            return 1;
            }
        else {
            return 0;
        }
    }
    printf("\n\n\n\n\n\n");
    return 0;
}

void Switch(Battery* Battery_Check){
    if(Battery_Check->Position == Position_One){
        P3OUT |= BIT4;
        P4OUT &= ~BIT3;
        printf("Switching to Position Two\n");
    }
    else if(Battery_Check->Position == Position_Two){
        P3OUT &= ~BIT4;
        P4OUT &= ~BIT3;
        printf("Switching to Position one\n");

    }
}

