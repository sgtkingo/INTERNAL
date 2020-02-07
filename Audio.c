/*
 * File:   Audio.c
 * Author: Jiri Konecny
 *
 * Created on 7. prosince 2019, 17:47
 */


#include "PICF18LF46K22_ConfigSFR.h"

#define AD_GODONE  ADCON0bits.GO_DONE 
#define DAC_ON     VREFCON1bits.DACEN
#define LED PORTDbits.RD0

#define MAX_VOLTAGE_LEVEL 32
#define BUFFMEM 512
#define REF_LEVEL 29
#define MULTIPLE_LEVEL 3

#define BIT5 32
#define BIT10 1024



unsigned int indexer;
unsigned int SAMPLE=0;
unsigned int Buffer[BUFFMEM];
unsigned int FiltredBuffer[BUFFMEM];

void ClearBuff(){
    for(int i=0; i<BUFFMEM; i++)Buffer[i]=0x00;
    for(int i=0; i<BUFFMEM; i++)FiltredBuffer[i]=0x00;
    indexer=0;
}

bit isBufferFull(){
    if(indexer>=BUFFMEM)return true;
    else false;
}

unsigned int getSample(){
    AD_GODONE=1;
    NOP();
    while(AD_GODONE);
    NOP();
    return (ADRESH<<8 | ADRESL);
}

unsigned int formatSample(unsigned int from,unsigned int to,unsigned int sample){
    unsigned int result=0x0000;
    float fromResult=(float)sample/from;
    float toResult=to*fromResult;
    
    result=(unsigned int)toResult;
    return result;
}

void setSample(unsigned char value){
    if(value > MAX_VOLTAGE_LEVEL)value=32;
    /*for(char i=0;i<value;i++){
        VREFCON2=i;
    }*/
    VREFCON2=value;
}

char calcSample(char sample){
    if(sample < REF_LEVEL)sample=REF_LEVEL; //
    char multipler=sample-REF_LEVEL;
    return multipler*MULTIPLE_LEVEL;
}

unsigned int calcTimeUs(unsigned int freqHz){
    return ((float)1/(freqHz+1))*1000000;
}

unsigned int calcTimerValue(unsigned int t_uS, unsigned char prescaller){
    unsigned int result=0;
    float T=0,Tmc=0;
    
    T=(float)(t_uS-15)/1000000;
    Tmc=((float)(4*prescaller)/_XTAL_FREQ);
    result=(unsigned int)(T/Tmc);
    
    return result;
}

unsigned char Timer0Preloader;//set Timer0 to 100uS

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    INTCON=0b11100000; //GIE, PIE, TMR0
    T0CON=0b01000011;   //Off, 8b, INTOSC, 1:16 Fost/4
    unsigned int SampleFreq=16000; //16kHz
    Timer0Preloader=(0xFF-calcTimerValue(calcTimeUs(SampleFreq), 16)); ; //set Timer0 to preload value (+10%?)
    
    //Init LEDs
    ANSELD=0;
    TRISD=0;

    //Init AUDIO IN
    ANSELEbits.ANSE2=1;
    TRISEbits.RE2=1;
        //Init AUDIO OUT
    ANSELAbits.ANSA2=1;
    TRISAbits.RA2=0;
    
    ANSELCbits.ANSC2=1;
    TRISCbits.RC2=1;
    
    //Set AD
    VREFCON0=0b10100000; //FVR set to 2,048 V ADC
    //VREFCON0=0b10010000; //FVR set to 1,024 V ADC
    
    ADCON0=0b00011101; //AN7 (RE2) chanel, Enable AD
    ADCON1=0b00001000; //Vref + to FVR BUF2, Vref - to Vss
    //ADCON2=0b10110010; //ADFM=1 , ACQT 16 TAD , ADCS Fosc/32 (TAD=2us)
    ADCON2=0b10101101; //ADFM=1 , ACQT 12 TAD , ADCS Fosc/16 (TAD=1us)
    
    //result ADRESH, ADRESL
    
    //Set DA
    VREFCON1=0b01100000; //!DAC enable, DAOUT (RA2), Vdd + , Vss - (MAX 32 VOLTAGE LEVELS!)
    VREFCON2=0b00011111;
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
    
    DAC_ON=1;
    T0CONbits.TMR0ON=1;
    while(1);
}

void interrupt IRS(void){
    if(INTCONbits.TMR0IF){
        SAMPLE=getSample();
        Buffer[indexer]=SAMPLE;
        FiltredBuffer[indexer]=calcSample(SAMPLE);
        setSample(formatSample(BIT10,BIT5,FiltredBuffer[indexer]));
        indexer++;
        if(isBufferFull()){
            NOP();
            indexer=0;
            if(PORTD==0xFF)PORTD=0; 
            else ++PORTD;
        }
        //LED=!LED;
        TMR0L=Timer0Preloader; //set Timer0 to preload value
        INTCONbits.TMR0IF=0;
    }            
    /*if(INTCONbits.TMR0IF){
        LED=!LED;
        TMR0L=Timer0Preloader; //set Timer0 to preload value
        INTCONbits.TMR0IF=0;
    }*/
}
