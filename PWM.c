/*
 * File:   PWM.c
 * Author: kater
 *
 * Created on 7. února 2020, 15:41
 */

#include "PICF18LF46K22_ConfigSFR.h"

#define TP PORTDbits.RD0
#define PWM_PIN PORTDbits.RD1

#define TMR2_ON T2CONbits.TMR2ON
#define PWM_TMR2_F PIR1bits.TMR2IF

bit direction;

//declared init fce
void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11000000; //en gie, pie
    PIE1bits.TMR2IE=1; //Enables the TMR2 to PR2 match interrupt
    
    ANSELD=0x00;
    TRISD=0x00;
     
    //CCP4 out is RD1
    CCPTMRS1=0x0C; //set PWM source as CCP4, Timer2
    PR2=0xFF; //Timer2 Period Register, 255
    
    CCP4CON=0b00001100; //set CCP4 as PWM, LSB <4:5> = 0b11 
    CCP4CONbits.DC4B0=1;
    CCP4CONbits.DC4B1=1; 
    CCPR4L=0x40; // MSB = 64
     
    T2CON=0b00000011; //pre <0:1> 1:16, post <3:6> 1:1   
}
//declared clear fce
void ClearDevice(){
    PORTD=0;
    PORTBbits.RB0=0;
    direction=true;
}
//declared test fce
void TestDevice(){
    LATD=0xFF;
    delay_ms(1000);
    LATD=0x00;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
    
    TMR2_ON=1;
    while(1){ NOP();
       /*if(PWM_PIN){
           LATD=0xFF;
       }else LATD=0x00;*/
    }
    return;
}

void interrupt IRS(void){
    if(PWM_TMR2_F){
       PWM_TMR2_F=0;
       if(direction)CCPR4L++;
       else CCPR4L--;
       
       if((CCPR4L >= PR2) && direction){
           direction=false;
       }
       if((CCPR4L == 0) && !direction){
           direction=true;
       }
       TP=!TP;
    }
}
