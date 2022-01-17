#include "INITIALIZATION.h"
#include "AMBIENT_SENSOR_LCD.h"
#include "GCD.h"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <string.h>
#include <stdio.h>



// Greatest Common Divisor to Initialize Tick Period

unsigned long GCD;


/* Shared Variable between State Machines.*/
unsigned char mtn;

/* Global Variable for SysTick Interrupt.*/
unsigned char SM_Clk = 0;

/* Global Variable for Total Number of state_machines*/
const unsigned char sm_num = 3;

/* Counter for Room Light Delay After Motion Detection is Off*/
unsigned char cnt;

/*Push Notification Message to Send to Home Owner Via Serial Communication*/
char notification_message[18] ="Motion Detected: ";
char len = sizeof(notification_message) / sizeof(notification_message[0]);

/* Counter for printing Motion Detected only once while input stays on*/
unsigned char counter= 0;

/************ Ambient Sensor Input and LCD Display Declaration & Initialization ***************/

float lux;
char lux_string[8];

/*******************************----------------------------********************/


enum SM1_States { SM1_DMO, SM1_DM1, SM1_DM2 } SM1_State;
void TickFct_Detect_Motion_SM();

enum SM2_States { SM2_IL0, SM2_IL1, SM2_IL2 } SM2_State;
void TickFct_Switch_Room_Light_SM();

enum SM3_States { SM3_BL0, SM3_BL1 } SM3_State;
void TickFct_Sound_Alarm_SM();

unsigned char j,k;



/* ********* Initialize States for All Three State Machines ************/

void Motion_Det_State(){
    SM1_State = SM1_DMO;
}

void Room_Light_State(){
    SM2_State = SM2_IL0;
}


void Buzzer_Alarm_State(){
    SM3_State = SM3_BL0;
}
/************  -------------------------------------------  ************/



/* Building A Configurable State_machine Scheduler Depending On Varying State_Machine Period */

typedef struct task {
   void (*init_state)();                  // state_machine's state initialization
   unsigned long sm_period;       // state_machine period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   void (*TickFct)();        // task tick function
} task;


task state_machines[sm_num];

void init_tasks(){

    state_machines[0] = (task){ .init_state = &Motion_Det_State, .sm_period = 200, .elapsedTime = 0, .TickFct = &TickFct_Detect_Motion_SM};
    state_machines[1] =(task) { .init_state = &Room_Light_State, .sm_period = 200, .elapsedTime = 0, .TickFct = &TickFct_Switch_Room_Light_SM};
    state_machines[2] = (task) { .init_state = &Buzzer_Alarm_State, .sm_period = 200, .elapsedTime = 0, .TickFct = &TickFct_Sound_Alarm_SM};

    GCD = gcd_Num(state_machines[0].sm_period, state_machines[1].sm_period, state_machines[2].sm_period);

}



/* SysTick Interrupt Handler Function */
void SysTick_Handler(void) {
   unsigned char i = 0;
   SM_Clk = 1;
   for (i=0; i < sm_num; i++) {
     if (state_machines[i].elapsedTime >= state_machines[i].sm_period){
             // state_machine is ready to tick, so call its tick function
       state_machines[i].TickFct();
       state_machines[i].elapsedTime = 0; // Reset the elapsed time
     }
     state_machines[i].elapsedTime += GCD;
     printf(state_machines[i].elapsedTime);
   }
}

 int main() {

   init_LCD_Ambient();
   init_ports();
   init_tasks();
   init_UART();
   timer_init();



   while(1) {
      while (!SM_Clk){
          __sleep();
      }
      SM_Clk = 0;

     }
}




void TickFct_Detect_Motion_SM() {
    lux = get_lux();
    unsigned char A0 = lux<100;

   switch(SM1_State) { // Transitions

      case SM1_DMO:
         if (!(A0)) {
            SM1_State = SM1_DM1;
         }
         else if (A0) {
            SM1_State = SM1_DMO;
         }
         break;
      case SM1_DM1:
         if (A0) {
            SM1_State = SM1_DMO;
         }
         else if (!(A0)) {
            SM1_State = SM1_DM2;
         }
         break;
      case SM1_DM2:
         if (!(A0)) {
            SM1_State = SM1_DM2;
         }
         else if (A0) {
            SM1_State = SM1_DMO;
         }
         break;
      default:
         SM1_State = SM1_DMO;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_DMO:
         mtn =0;
         counter =0;
         display_basic_message();
         break;
      case SM1_DM1:
         break;
      case SM1_DM2:
         mtn =1;
         display_temperature_value(lux);

         if(counter == 0){
             counter = 1;

             for(j=0;j<len;j++){     //Transmit the string one character at a time
               while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
               EUSCI_A0->TXBUF = notification_message[j];
             }
          }

         break;
      default: // ADD default behaviour below
         break;
   } // State actions
   sprintf(lux_string, "%f", lux);
   for(k=0;k<8;k++){     //Transmit the string one character at a time
      while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
      EUSCI_A0->TXBUF = lux_string[k];
   }
   while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); //send CR sequence after temperature string
   EUSCI_A0->TXBUF = '\n';
   EUSCI_A0->TXBUF ='\r';

}


void TickFct_Switch_Room_Light_SM() {

   switch(SM2_State) { // Transitions

      case SM2_IL0:
         if (mtn) {
            SM2_State = SM2_IL1;
         }
         else if (!mtn) {
            SM2_State = SM2_IL0;
         }
         break;
      case SM2_IL1:
         if (!mtn) {
            SM2_State = SM2_IL2;
            cnt =0;
         }
         else if (mtn) {
            SM2_State = SM2_IL1;
         }
         break;
      case SM2_IL2:
         if (!mtn && cnt < 10) {
            SM2_State = SM2_IL2;
         }
         else if (mtn) {
            SM2_State = SM2_IL1;
         }
         else if (!mtn && !(cnt<10)) {
            SM2_State = SM2_IL0;
         }
         break;
      default:
         SM2_State = SM2_IL0;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_IL0:
         B1 &= ~BIT6;
         break;
      case SM2_IL1:
         B1 |= BIT6;
         break;
      case SM2_IL2:
         ++cnt;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions

}



void TickFct_Sound_Alarm_SM() {

   switch(SM3_State) { // Transitions

      case SM3_BL0:
         if (!mtn) {
            SM3_State = SM3_BL0;
         }
         else if (mtn) {
            SM3_State = SM3_BL1;
         }
         break;
      case SM3_BL1:
         if (1) {
            SM3_State = SM3_BL0;
         }
         break;
      default:
         SM3_State = SM3_BL0;
      } // Transitions

   switch(SM3_State) { // State actions
      case SM3_BL0:
         B0 &= ~BIT7 ;
         break;
      case SM3_BL1:
         B0 |= BIT7;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions

}
