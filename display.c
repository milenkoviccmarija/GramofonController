#include "at89c51rc2.h"
#include "display.h"
#include "intrins.h"

void initP1P3(void) {          
    P1 = 0xE0;
    P3 = 0xF3;
}

void wait1s(void) {            
    uint8_t i;
    TMOD = (TMOD&0xF0) | 0x01;  
    ET0 = 0;
    for(i=0;i<200;i++) {
        TH0 = 0xEE;            
        TL0 = 0x00;            
        TR0 = 1;                    
        while(!TF0);
        TF0 = 0;                
    }
    TR0=0;                        
}

void wait50micro(void) {
    TMOD = (TMOD&0xF0) | 0x01;
    ET0 = 0;
    TH0=0xFF;
    TL0=0xD2;
    TR0=1;                    
    while(!TF0);
    TF0=0;
    TR0=0;  
}

void wait2ms(void) {
    TMOD = (TMOD&0xF0) | 0x01;
    ET0 = 0;
    TH0=0xF8;          
    TL0=0xCD;
    TR0=1;
    while(!TF0);
    TF0=0;
    TR0=0;  
}

void entry_setDisplay(void){
    LCD_RS=0;      
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=1;
    LCD_D5=1;
    LCD_D4=0;
    LCD_EN=0;
    wait50micro();
}

void function_setDisplay(void){
    LCD_RS=0;
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=1;
    LCD_D4=0;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=1;      
    LCD_D6=0;  
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    wait50micro();
}

void clearDisplay(void) {
    LCD_RS=0;
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=1;
    LCD_EN=0;
    wait2ms();
    wait2ms();
}

void on_offDisplay(void){
    LCD_RS=0;
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=1;
    LCD_D6=1;
    LCD_D5=1;
    LCD_D4=1;
    LCD_EN=0;
    wait50micro();  
}

void initDisplay(void) {    
    initP1P3();
    CKCON0 &= ~(1<<0);
    wait1s();
    function_setDisplay();
    on_offDisplay();
    clearDisplay();
    entry_setDisplay();
    wait2ms();
    LCD_BL=1;
}

void writeChar(uint8_t karakter) {
    LCD_RS=1;
    LCD_EN=1;                    
    LCD_D7=(karakter >> 7) & 0x01;
    LCD_D6=(karakter >> 6) & 0x01;
    LCD_D5=(karakter >> 5) & 0x01;
    LCD_D4=(karakter >> 4) & 0x01;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=(karakter >> 3) & 0x01;
    LCD_D6=(karakter >> 2) & 0x01;
    LCD_D5=(karakter >> 1) & 0x01;
    LCD_D4=(karakter >> 0) & 0x01;
    LCD_EN=0;
    wait50micro();
}

void writeLine(uint8_t* message) {  
    uint8_t n=0;
    while(message[n]!='\0') {
        writeChar(message[n]);
        n++;
    }
}

void newLine(void) {                        
    LCD_RS=0;
    LCD_EN=1;  
    LCD_D7=1;
    LCD_D6=1;  
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    _nop_();
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    wait2ms();
    wait2ms();
}