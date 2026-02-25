#include "timer1.h"
#include "at89c51rc2.h"

#define TH1_INIT 0xFC
#define TL1_INIT 0x67

volatile uint32_t timer1_ms = 0;
volatile uint32_t timer1_s = 0;

void init_timer1(void)
{
    TMOD = (TMOD & 0x0F) | 0x10;
    TH1 = TH1_INIT;
    TL1 = TL1_INIT;
    ET1 = 1;  
    TR1 = 0;  
}

void start_timer1(void)
{
    TH1 = TH1_INIT;
    TL1 = TL1_INIT;
    TR1 = 1;
}

void stop_timer1(void)
{
    TR1 = 0;
}

void reset_timer1(void)
{
    timer1_ms = 0;
	timer1_s=0;
}

void timer1_isr(void) interrupt 3
{
    TF1 = 0;
    TH1 = TH1_INIT;
    TL1 = TL1_INIT;
    timer1_ms++;
	if(timer1_ms>=1000){
		timer1_s++;
		timer1_ms=0;
	}
}
