/*
 * File:   PWM.c
 * Author: kater
 *
 * Created on 7. �nora 2020, 15:41
 */

#include "PICF18LF46K22_ConfigSFR.h"

#define LED LATDbits.LATD0

//declared init fce
void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
<<<<<<< HEAD
=======
    OSCTUNE=0x00; //PLLx4 OFF
>>>>>>> c1189114aa668f4a18ed4c8277571b5af4be3043
    ANSELD=0x00;
    TRISD=0x00;
}
//declared clear fce
void ClearDevice(){
    PORTD=0;
    LATD=0;
}
//declared test fce
void TestDevice(){
    delay_ms(1000);
    LATD=0xFF;
    delay_ms(1000);
    LATD=0x00;
}

void main(void) {
    InitDevice();
    ClearDevice();
    TestDevice();
<<<<<<< HEAD
    
=======

>>>>>>> c1189114aa668f4a18ed4c8277571b5af4be3043
    while(1){
        LED=!LED;
        delay_ms(1000);
        if(LED){
            LATD<<=1;
            LATD|=0x01;
        }
        if(PORTD == 0xFF)LATD=0;
    }
    return;
<<<<<<< HEAD
}
=======
}
>>>>>>> c1189114aa668f4a18ed4c8277571b5af4be3043
