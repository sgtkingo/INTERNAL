/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM_FUNCTIONLIB_H
#define	EEPROM_FUNCTIONLIB_H
#include <xc.h> // include processor files - each processor file is guarded. 

#define WRITE EECON1bits.WR
#define READ EECON1bits.RD

#define WRITE_ENABLE EECON1bits.WREN
#define WRITE_ERROR EECON1bits.WRERR

void Init_EEPROM();
void Write_EEPROM(unsigned int addr,unsigned char data);
unsigned char Read_EEPROM(unsigned int addr);

void Init_EEPROM(){
    //EEDATA eeprom data register
    //EECON2 catch register
    //EEADR + EEADRH = 10bit eemprom address (0-1023)
    EECON1=0b00000000; //Access EEPROM, deny Write
    EEADRH=0x00; //will use only down 8bit of addr (EEADR)
    //PIE2bits.EEIE=1; //intterupt enable
}

void Write_EEPROM(unsigned int addr,unsigned char data){
    EEADR=(addr & 0xFF); //Set EEPROM address
    EEADRH=((addr >> 8)&0x03); //Set EEPROM H address
    EEDATA=data; //save data to eeprom register
    WRITE_ENABLE=1;
    
    ///NEED by proceser - INIT WRITE operation
    EECON2=0x55; 
    EECON2=0x0AA;
    ///
    WRITE=1; //Set WRITE operation
    while(WRITE && !WRITE_ERROR)NOP(); //till to complete
    WRITE_ENABLE=0;    
}

unsigned char Read_EEPROM(unsigned int addr){
    unsigned char data=0;
    EEADR=(addr & 0xFF); //Set EEPROM address
    EEADRH=((addr >> 8)&0x03); //Set EEPROM H address
    READ=1; //Set READ operation
    
    while(READ)NOP(); //till to complete
    data=EEDATA; //store data  
    
    return data;
}

#endif	/* EEPROM_FUNCTIONLIB_H */

