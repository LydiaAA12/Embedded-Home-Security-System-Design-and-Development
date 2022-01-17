#include <INITIALIZATION.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

void timer_init(){

    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    SysTick->LOAD |= TICKS-1;
    SysTick->VAL = 0X01;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //Local Enable to generate SysTick interrupt at the rollover of the SysTick counter
    __enable_irq(); //Enable Global Interrupt

}


void init_ports(){
    P5->DIR &= ~(BIT1);   // Make all pins of Port 1 inputs pins. (only Pins 1 and 4 will be used)
    P5->REN |= BIT1;   // Enable pull resistors on Port 5 Pins 1
    P5->OUT |= BIT1;   // Make the enabled pull resistors on Pin 1 to be Pull-up resistor
    P5->DIR |= BIT6;   // Make Pin 1 of Port 2 (Green LED) an output pin
    P2->DIR |= BIT7;

    P5->OUT &= ~BIT6;
    P2->OUT &= ~BIT7;

}


void init_UART(){
    // Configure UART pins
  P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function
  P1->DIR |= BIT0;                        // Make Red LED an output

    // Configure UART
  EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset

  EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |  EUSCI_A_CTLW0_SSEL__SMCLK;
    //          |EUSCI_A_CTLW0_PEN|EUSCI_A_CTLW0_PAR;      // Configure eUSCI clock source for SMCLK

  EUSCI_A0->BRW = 19;                     // 9,600 Baud =>  3,000,000/16*9,600 = 19.51..
  EUSCI_A0->MCTLW = (0xAA << EUSCI_A_MCTLW_BRS_OFS) | (8 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;
  EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI

  // Enable eUSCIA0 interrupt in NVIC module
   NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);

}

