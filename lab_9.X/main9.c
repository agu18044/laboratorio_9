/*
 Diego Aguilar
 * 18044
 * Lab 9
 
 */


#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 8000000

//

void setup(void);

void __interrupt() isr(void) { 
    if(ADCON0bits.CHS == 0){
            CCPR1L = (ADRESH >> 1) + 124;
    }    
    else if(ADCON0bits.CHS == 1){ 
            CCPR2L = (ADRESH >> 1) + 124;
    }
    PIR1bits.ADIF = 0;     
}

void main (void){
    setup();
    while (1){
        if (ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 1){
                ADCON0bits.CHS = 0;
               __delay_us(50);
            }
            else{
                ADCON0bits.CHS = 1;}
                __delay_us(50);
                ADCON0bits.GO =1;   
        }
    }

}

void setup(void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    TRISA = 0XFF;
    
    //config oscilador
    OSCCONbits.IRCF = 0b0111; //8Mhz
    OSCCONbits.SCS = 1;
    
    //config ADC
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b10; //FOSC/32
    ADCON0bits.CHS = 5;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    
    //config PWM
    TRISCbits.TRISC2 = 1;   // RC2/CCP1 como entrada
    TRISCbits.TRISC1 = 1;
    PR2 = 255;              // config del periodo
    CCP1CONbits.P1M = 0;    // config modo PWM
    CCP1CONbits.CCP1M = 0b1100; //
    CCPR1L = 0x0f;          //ciclo de trabajo incial
    CCP2CONbits.CCP2M = 0b1100;
    CCPR2L = 0x0f;
    
    CCP1CONbits.DC1B = 0;
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    
    PIR1bits.TMR2IF = 0;     //apagar la bandera
    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
    
    while(PIR1bits.TMR2IF == 0);    
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;  // Salida del PWM        
    TRISCbits.TRISC1 = 0; 
    
    
    //config inter
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;        
    INTCONbits.GIE = 1;
 

    return; 
}
