/*
 * File:   buttons.c
 * Author: kater
 *
 * Created on 7. února 2020, 19:09
 */


#include "PICF18LF46K22_ConfigSFR.h"

#define BTN_A_P PORTBbits.RB0  //SW1
#define BTN_A_N PORTBbits.RB1  //SW2
#define BTN_B_P PORTBbits.RB2  //SW3
#define BTN_B_N PORTBbits.RB3  //SW5

#define LED_A PORTDbits.RD0
#define LED_B PORTDbits.RD1
#define LED_C PORTDbits.RD2
#define LED_D PORTDbits.RD3

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    
    INTCON2bits.NOT_RBPU=1; //potrB pullup enable
    WPUBbits.WPUB0=1;
    WPUBbits.WPUB1=1;
    WPUBbits.WPUB2=1;
    WPUBbits.WPUB3=1;
    
    ANSELD=0; //portD to digital
    TRISD=0;
    
    ANSELB=0; //portB to digital
    TRISBbits.RB0=1;  
    TRISBbits.RB1=1;
    TRISBbits.RB2=1;
    TRISBbits.RB3=1;
    
}

void ClearDevice(){
    PORTD=0x00;
}

void TestDevice(){
    LATD=0xFF;
    delay_ms(1000);
    LATD=0x00;
}

void CheckButtons(){
        if(!BTN_A_P){
            __delay_ms(10);
            if(!BTN_A_P)
                LED_A=1;
        }else LED_A=0;
        
        if(!BTN_A_N){
            __delay_ms(10);
            if(!BTN_A_N)
                LED_B=1;
        }else LED_B=0;
        
        if(!BTN_B_P){
            __delay_ms(10);
            if(!BTN_B_P)
                LED_C=1;
        }else LED_C=0;
        
        if(!BTN_B_N){
            __delay_ms(10);
            if(!BTN_B_N)
                LED_D=1;
        }else LED_D=0;
        if (BTN_A_P&&BTN_A_N&&BTN_B_P&&BTN_B_N)PORTD=0x00;
        if (!(BTN_A_P||BTN_A_N||BTN_B_P||BTN_B_N))PORTD=0xFF;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
    
    while(1){      
        asm("NOP");
        asm("CLRWDT");
        CheckButtons();
    }
}

void interrupt IRS(void){
    return;
}
