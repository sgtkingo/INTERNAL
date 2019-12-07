/*
 * File:   Audio.c
 * Author: Jiri Konecny
 *
 * Created on 7. prosince 2019, 17:47
 */


#include "PICF18LF46K22_ConfigSFR.h"

#define AD_GODONE  ADCON0bits.GO_DONE 
#define BUFFMEM 1024

unsigned char indexer;
unsigned char Buffer[BUFFMEM];

void ClearBuff(){
    for(int i=0; i<BUFFMEM; i++)Buffer[i]=0x00;
    indexer=0;
}

bit isBufferFull(){
    if(indexer>=BUFFMEM)return true;
    else false;
}

unsigned char getSample(){
    AD_GODONE=1;
    NOP();
    while(AD_GODONE);
    NOP();
    return ADRESL;
}

unsigned int calcTimerValue(unsigned int t_uS, unsigned char prescaller){
    unsigned int result=0;
    float T,Tmc;
    
    T=(float)t_uS/1000000;
    Tmc=((4*(float)prescaller)/_XTAL_FREQ);
    result=(unsigned int)(T/Tmc);
    
    return result;
}

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11100000; //GIE, PIE, TMR0
    T0CON=0b01000100;   //Off, 8b, INTOSC, 1:32 Fost/4
    TMR0L=(0xFF-calcTimerValue(100, 32)); //set Timer0 to 100uS
    
    //Init LEDs
    ANSELD=0;
    TRISD=0;

    //Init AUDIO IN
    ANSELEbits.ANSE2=1;
    TRISEbits.RE2=1;
    //Init AUDIO OUT
    ANSELCbits.ANSC2=1;
    TRISCbits.RC2=0;
    
    //Set AD
    VREFCON0=0b10010000; //FVR set to 1,024 V
    
    ADCON0=0b00011101; //AN7 (RE2) chanel, Enable AD
    ADCON1=0b00001000; //Vref + to FVR BUF2, Vref - to Vss
    ADCON2=0b10100001; //ADFM=1 , ACQT 8 TAD , ADCS Fosc/8
    
    //result ADRESH, ADRESL
    
}
void ClearDevice(){
    PORTD=0;
    ClearBuff();
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
        Buffer[indexer++]=getSample();
        if(isBufferFull()){
            T0CONbits.TMR0ON=0;
        }
        if(indexer%4)++PORTD;
        INTCONbits.TMR0IF=0;
    }
}
