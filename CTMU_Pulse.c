<<<<<<< HEAD
/*
 * File:   CTMU Pulse.c
 * Author: kater
 *
 * Created on 18. prosince 2019, 16:50
 */


#include "PICF18LF46K22_ConfigSFR.h"

unsigned char Timer0Preloader;//set Timer0 to 100uS
#define LED PORTDbits.RD0
#define ENABLE_CTMU CTMUCONHbits.CTMUEN
void ConfigCTMU(){
    //1. Congfig CPP2
    
    //AI
    ANSELAbits.ANSA1=1;
    TRISAbits.RA1=1;
    
    CM2CON0=0b10001101; // Vref+, C12IN1 - (RA1), non-inverted, normal mode
    CM2CON1=0b00110000; //Vref to FVR Buff
    
    CTMUCONH=0b00011000;
    
}

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11100000; //GIE, PIE, TMR0
    T0CON=0b01000011;   //Off, 8b, INTOSC, 1:16 Fost/4
    Timer0Preloader=(0xFF-calcTimerValue(80, 16)); ; //set Timer0 to preload value
    
    //Init LEDs
    ANSELD=0;
    TRISD=0;
    
    //Set VFR
    VREFCON0=0b10100000; //FVR set to 2,048 V CPP2
    VREFCON1=0x00; 
    VREFCON2=0x00;

    //Init AUDIO OUT
    ANSELCbits.ANSC2=0;
    TRISCbits.RC2=0;   
    
    ConfigCTMU();
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
        TMR0L=Timer0Preloader; //set Timer0 to preload value
        INTCONbits.TMR0IF=0;
    }
}
=======
/*
 * File:   CTMU Pulse.c
 * Author: kater
 *
 * Created on 18. prosince 2019, 16:50
 */


#include "PICF18LF46K22_ConfigSFR.h"

unsigned char Timer0Preloader;//set Timer0 to 100uS
#define LED PORTDbits.RD0
#define ENABLE_CTMU CTMUCONHbits.CTMUEN
void ConfigCTMU(){
    //1. Congfig CPP2
    
    //AI
    ANSELAbits.ANSA1=1;
    TRISAbits.RA1=1;
    
    CM2CON0=0b10001101; // Vref+, C12IN1 - (RA1), non-inverted, normal mode
    CM2CON1=0b00110000; //Vref to FVR Buff
    
    CTMUCONH=0b00011000;
    
}

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11100000; //GIE, PIE, TMR0
    T0CON=0b01000011;   //Off, 8b, INTOSC, 1:16 Fost/4
    Timer0Preloader=(0xFF-calcTimerValue(80, 16)); ; //set Timer0 to preload value
    
    //Init LEDs
    ANSELD=0;
    TRISD=0;
    
    //Set VFR
    VREFCON0=0b10100000; //FVR set to 2,048 V CPP2
    VREFCON1=0x00; 
    VREFCON2=0x00;

    //Init AUDIO OUT
    ANSELCbits.ANSC2=0;
    TRISCbits.RC2=0;   
    
    ConfigCTMU();
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
        TMR0L=Timer0Preloader; //set Timer0 to preload value
        INTCONbits.TMR0IF=0;
    }
}
>>>>>>> c45a37996cf49d8c0b6de4972458d65d10158558
