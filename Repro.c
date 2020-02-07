/*
 * File:   Repro.c
 * Author: kater
 *
 * Created on 18. prosince 2019, 16:13
 */

#include "PICF18LF46K22_ConfigSFR.h"

#define MAX_VOLTAGE_LEVEL 32
#define DAC_ON     VREFCON1bits.DACEN
#define LED PORTDbits.RD0

unsigned char Timer0Preloader;//set Timer0 to 100uS
unsigned char voltageLevel=0;

unsigned int calcTimerValue(unsigned int t_uS, unsigned char prescaller){
    unsigned int result=0;
    float T=0,Tmc=0;
    
    T=(float)t_uS/1000000;
    Tmc=((float)(4*prescaller)/_XTAL_FREQ);
    result=(unsigned int)(T/Tmc);
    
    return result;
}

void setSample(unsigned char value){
    if(value > MAX_VOLTAGE_LEVEL)value=32;
    DAC_ON=0;
    VREFCON2=value;
    DAC_ON=1;
}

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11100000; //GIE, PIE, TMR0
    T0CON=0b01000011;   //Off, 8b, INTOSC, 1:16 Fost/4
    Timer0Preloader=(0xFF-calcTimerValue(80, 16)); ; //set Timer0 to preload value
    
    //Init LEDs
    ANSELD=0;
    TRISD=0;

    //Init AUDIO OUT
    ANSELAbits.ANSA2=1;
    TRISAbits.RA2=0;
    
    ANSELCbits.ANSC2=1;
    TRISCbits.RC2=1;
    
    //Set DA FVR
    VREFCON0=0b10100000; //FVR set to 2,048 V ADC
    
    //Set DA
    VREFCON1=0b01100000; //!DAC enable, DAOUT (RA2), Vdd + , Vss - (MAX 32 VOLTAGE LEVELS!)
    VREFCON2=0b00011111;
}
void ClearDevice(){
    PORTD=0;
}
void TestDevice(){
    PORTD=0xFF;
    while(!VREFCON0bits.FVRS);
    __delay_ms(500);
    PORTD=0;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
    
    T0CONbits.TMR0ON=1;
    while(1);
}

void interrupt IRS(void){
    if(INTCONbits.TMR0IF){
        LED=!LED;
        setSample(voltageLevel++);
        if(voltageLevel > MAX_VOLTAGE_LEVEL)voltageLevel=0;
        
        TMR0L=Timer0Preloader; //set Timer0 to preload value
        INTCONbits.TMR0IF=0;
    }
}
