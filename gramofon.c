#include "gramofon.h"
#include "gpio.h"
#include "display.h"
#include "timer1.h"
#include <stdio.h>

volatile stanje_t stanje = STANJE_STOP;
volatile uint8_t broj_nitni = 0;
volatile uint8_t pomeraj = 0;

static stanje_t prev_stanje_motor = STANJE_STOP;
static stanje_t prev_stanje_lcd   = STANJE_STOP;

static bit prev_nitna = 0;
static bit prev_krug  = 0;

static char lcd_buf[17];
static uint8_t prev_broj_nitni = 0;
static uint32_t prev_timer = 0;


static uint8_t poslednji_broj_nitni = 0;
static uint32_t poslednje_vreme = 0;

void gramofon_init(void)
{
    stanje = STANJE_STOP;
    broj_nitni = 0;
    pomeraj = 0;
    reset_timer1();
}


void reset_brojanje(void)
{
    broj_nitni = 0;
    reset_timer1();
}


void detekcija_senzora(void)
{
    bit nitna = P0_1;
    bit krug  = P0_2;

    if(nitna && !prev_nitna)
        broj_nitni++;

    if(krug && !prev_krug && stanje == STANJE_BROJANJE)
    {
        poslednji_broj_nitni = broj_nitni;
        poslednje_vreme = timer1_s;

        reset_brojanje();
        start_timer1();
    }

    prev_nitna = nitna;
    prev_krug  = krug;
}


static void updateDisplay(void)
{
    clearDisplay();

    switch(stanje)
    {
        case STANJE_STOP:
            writeLine((uint8_t*)"STOP");
            newLine();
            sprintf(lcd_buf, "T:%lu", (unsigned long)poslednje_vreme);
            writeLine((uint8_t*)lcd_buf);
            break;

        case STANJE_READY:
            writeLine((uint8_t*)"ON");
            newLine();
            writeLine((uint8_t*)"READY");
            break;

        case STANJE_BROJANJE:
            writeLine((uint8_t*)"RUN");
            newLine();
            sprintf(lcd_buf, "N:%u T:%lu",
                    (unsigned int)broj_nitni,
                    (unsigned long)timer1_s);
            writeLine((uint8_t*)lcd_buf);
            break;

        case STANJE_POMERANJE:
            writeLine((uint8_t*)"MOVE");
            newLine();
            sprintf(lcd_buf, "N:%u T:%lu",
                    (unsigned int)broj_nitni,
                    (unsigned long)timer1_s);
            writeLine((uint8_t*)lcd_buf);
            break;
    }
}

void gramofon_update(void)
{

    if(stanje != prev_stanje_motor)
    {
        switch(stanje)
        {
            case STANJE_STOP:
                motor_off();
                stop_timer1();
                break;

            case STANJE_READY:
                motor_off();
                stop_timer1();
                break;

            case STANJE_BROJANJE:
                reset_brojanje();
                start_timer1();
                motor_on();
                break;

            case STANJE_POMERANJE:
                reset_brojanje();
                start_timer1();
                motor_on();
                break;
        }

        prev_stanje_motor = stanje;
    }

    if(stanje == STANJE_BROJANJE || stanje == STANJE_POMERANJE)
    {
        detekcija_senzora();
    }

    if(stanje == STANJE_POMERANJE)
    {
        if(broj_nitni >= pomeraj)
        {
            motor_off();
            stop_timer1();

            poslednji_broj_nitni = broj_nitni;
            poslednje_vreme = timer1_s;

            stanje = STANJE_STOP;
        }
    }


    if(stanje != prev_stanje_lcd ||
       broj_nitni != prev_broj_nitni ||
       timer1_s != prev_timer)
    {
        updateDisplay();

        prev_stanje_lcd = stanje;
        prev_broj_nitni = broj_nitni;
        prev_timer      = timer1_s;
    }
}
