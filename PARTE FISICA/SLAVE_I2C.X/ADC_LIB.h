/*
 * Archivo:   ADC_LIB.h
 * Dispositivo: PIC16F887
 * Autor: Brandon Garrido 
 * 
 * Programa: Comunicación esclavo - maestro SPI / ADC
 * 
 * Creado: Agosto 02, 2021
 * Última modificación: Agosto 03, 2021
 */

#ifndef ADC_LIB_H
#define	ADC_LIB_H
#define _XTAL_FREQ 4000000

void ADC_INIT(unsigned int canal){
    //CONFIGURACION DEL ADC
    ADCON0bits.ADON = 0X01;     //ENCENDEMOS EL MODULO
    ADCON0bits.CHS = canal;      //ESCOGEMOS EL CANAL 0
    __delay_us(100);
    ADCON0bits.ADCS = 0X01;     //ADC CLOCK FOSC/8  
    ADCON1bits.ADFM = 0;        //JUSTIFICADO A LA IZQUIERDA
    ADCON1bits.VCFG0 = 0;       //VOLTAGE DE REFERENCIA EN VDD
    ADCON1bits.VCFG1 = 0;       //VOLTAGE DE REFERENCIA EN VSS
    PIE1bits.ADIE = 1;          //HABILITAMOS LA INTERRUPCION DEL ADC
    PIR1bits.ADIF = 0;
}
unsigned int ADC_READ(void){
    unsigned int LECT;
    LECT = ADRESH;
    return LECT;
}
void ADC_CHANNELS(unsigned int canal, unsigned int valor_ADC, unsigned int* val){
    
    switch (ADCON0bits.CHS){
        case 0:
            *val = valor_ADC;
            ADCON0bits.CHS = canal;
            __delay_us(50);
            ADCON0bits.GO = 1;
            break;
        case 1:
            *val = valor_ADC;
            ADCON0bits.CHS = canal;
            __delay_us(50);
            ADCON0bits.GO = 1;
            break;
    }
    
    
}

#endif	/* ADC_LIB_H */

