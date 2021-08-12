/*
 * Archivo:   Lab04_main_master.c
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido 
 * 
 * Programa: Comunicación esclavo - maestro SPI / ADC
 * 
 * Creado: Agosto 02, 2021
 * Última modificación: Agosto 03, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "I2C.h"

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT//Oscillator Selection bits(INTOSCIO 
                              //oscillator: I/O function on RA6/OSC2/CLKOUT pin, 
                              //I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled and 
                          //can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR  
                                //pin function is digital input, MCLR internally 
                                //tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //Internal/External Switchover mode is disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         //Low Voltage Programming Enable bit(RB3/PGM pin 
                                //has PGM function, low voltage programming 
                                //enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 8000000

//------------------------------VARIABLES---------------------------------------
uint8_t z;
uint8_t CONTADOR = 0;
uint8_t flag1 = 0;
uint8_t flag2 = 0;
//-----------------------------PROTOTIPOS---------------------------------------
void setup (void);

//---------------------------INTERRUPCION--------------------------------------
void __interrupt()isr(void){
    di();                   //PUSH
    if (RBIF == 1){
            if (RB0 == 0){              //REVISAMOS QUE RB0 FUE PRESIONADO
                CONTADOR ++;
            }else if (RB1 == 0){              //REVISAMOS QUE RB1 FUE PRESIONADO
                CONTADOR --;
            }
        
     INTCONbits.RBIF = 0;           //LIMPIAMOS LA BANDERA DEL TMR0
    }
       if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // LEEMOS EL VALOR DEL BUFFER Y AGREGAMOS A UNA VARIABLE
            SSPCONbits.SSPOV = 0;       // LIMPIAMOS LA BANDERA DE OVERFLOW
            SSPCONbits.WCOL = 0;        // LIMPIAMOS EL BIT DE COLISION
            SSPCONbits.CKP = 1;         // HABILITAMOS SCL
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // LEEMOS EL VALOR DEL BUFFER Y AGREGAMOS A UNA VARIABLE
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // LIMPIAMOS BANDERA DE INTERUPCION RECEPCION/TRANSMISION SSP
            SSPCONbits.CKP = 1;         // HABILITA LOS PULSOS DEL RELOJ SCL
            while(!SSPSTATbits.BF);     // HASTA QUE LA RECEPCION SE REALICE
           // PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = CONTADOR;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
    ei();                           //POP
}
void main (void){
    setup(); 
    
    while(1){
        
        __delay_ms(10);
    }
}
//---------------------------CONFIGURACION--------------------------------------
void setup(void){
    //CONFIGURACION DE PUERTOS
    ANSEL = 0B0000000;          //PINES COMO DIGITALES
    ANSELH = 0X00;              //PINES COMO DIGITALES
    
    TRISA = 0B0000000;          //PORTA COMO OUTPUT
    TRISD = 0X00;               //PORTD COMO OUTPUT
    TRISE = 0X00;               //PORTE COMO OUTPUT
    TRISB = 0X03;               //RB0 Y RB1 COMO INPUT
    
    PORTA = 0X00;                //LIMPIAMOS EL PUERTOA
    PORTB = 0X00;                //LIMPIAMOS EL PUERTOB
    PORTC = 0X00;                //LIMPIAMOS EL PUERTOC
    PORTD = 0X00;                //LIMPIAMOS EL PUERTOD
    PORTE = 0X00;                //LIMPIAMOS EL PUERTOE
    
    //CONFIGURACION DEL OSCIALDOR
    OSCCONbits.IRCF2 = 1;        //OSCILADOR  DE 8 MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;         //RELOJ INTERNO 
    OPTION_REGbits.nRBPU = 0;   //RBPU HABILITADO, PULL UPS
    //CONFIGURACION DEL IOC
    WPUB0 = 1 ;                 //HABILITAMOS  LOS WEAK PULL UP RB0 RB1
    WPUB1 = 1 ;
    IOCB0 = 1 ;
    IOCB1 = 1 ;
    INTCONbits.GIE = 1;         //HABILITAMOS LAS INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        //HABILITAMOS LAS INTERRUPCIONES PERIFERICAS
    INTCONbits.RBIE = 1;        //HABILITAMOS LAS INTERRUPCIONES IOC
    INTCONbits.RBIF = 0;        //LIMPIAMOS LA BANDER DE IOC
    I2C_Slave_Init(0x80);       //DIRECCION DEL I2C ESCLAVO
}