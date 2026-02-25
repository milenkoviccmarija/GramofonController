#include "serijska.h"
#include "gramofon.h"
#include "stdint.h"
#include "timer1.h"
#include "gpio.h"

#define POCETNI_KARAKTER '('
#define KRAJNJI_KARAKTER ')'

uint8_t* bafer_za_slanje;
uint8_t bafer_za_prijem[KAPACITET];
uint8_t indeks_za_upisivanje = 0;

uint8_t poruka_greska[]   = "GRESKA\r\n";
uint8_t poruka_ispravna[] = "OK\r\n";


void inicijalizacija_serijska(void)
{

    SCON = 0x50; 

    PCON |= 0x80;  
    BDRCON = 0x1C; 
    BRL = 253;

    TI = 1;       
    RI = 0;        

    REN = 1;
}

void omoguci_prekid_serijske(void)
{
    ES = 1;
}

void posalji_poruku(uint8_t* poruka)
{
    bafer_za_slanje = poruka;
    SBUF = *bafer_za_slanje;
}

void parsiraj_poruku(void)
{
 
    if(bafer_za_prijem[0] == '(' &&
       bafer_za_prijem[1] == 'S' &&
       bafer_za_prijem[2] == 'T' &&
       bafer_za_prijem[3] == 'O' &&
       bafer_za_prijem[4] == 'P' &&
       bafer_za_prijem[5] == ')')
    {
        stanje = STANJE_STOP;
        motor_off();
        stop_timer1();
        posalji_poruku(poruka_ispravna);
        indeks_za_upisivanje = 0;
        return;
    }

    if(stanje == STANJE_BROJANJE || stanje == STANJE_POMERANJE)
    {
        posalji_poruku(poruka_greska);
        indeks_za_upisivanje = 0;
        return;
    }

    if(stanje == STANJE_STOP)
    {
        if(bafer_za_prijem[0] == '(' &&
           bafer_za_prijem[1] == 'S' &&
           bafer_za_prijem[2] == 'T' &&
           bafer_za_prijem[3] == 'A' &&
           bafer_za_prijem[4] == 'R' &&
           bafer_za_prijem[5] == 'T' &&
           bafer_za_prijem[6] == ')')
        {
            stanje = STANJE_READY;
            posalji_poruku(poruka_ispravna);
        }
        else
        {
            posalji_poruku(poruka_greska);
        }

        indeks_za_upisivanje = 0;
        return;
    }

    if(stanje == STANJE_READY)
    {
 
        if(bafer_za_prijem[0] == '(' &&
           bafer_za_prijem[1] == 'B' &&
           bafer_za_prijem[2] == 'R' &&
           bafer_za_prijem[3] == 'O' &&
           bafer_za_prijem[4] == 'J' &&
           bafer_za_prijem[5] == 'I' &&
           bafer_za_prijem[6] == ')')
        {
            stanje = STANJE_BROJANJE;
            posalji_poruku(poruka_ispravna);
        }


        else if(bafer_za_prijem[0] == '(' && bafer_za_prijem[1] == 'M')
        {
            uint16_t vrednost = 0;
            uint8_t i = 2;

            while(i < KAPACITET && bafer_za_prijem[i] != ')')
            {
                if(bafer_za_prijem[i] < '0' || bafer_za_prijem[i] > '9')
                {
                    posalji_poruku(poruka_greska);
                    indeks_za_upisivanje = 0;
                    return;
                }

                vrednost = vrednost * 10 + (bafer_za_prijem[i] - '0');
                i++;
            }

            if(bafer_za_prijem[i] == ')' && vrednost > 0 && vrednost <= 255)
            {
                pomeraj = (uint8_t)vrednost;
                broj_nitni = 0;
                reset_timer1();
                stanje = STANJE_POMERANJE;
                posalji_poruku(poruka_ispravna);
            }
            else
            {
                posalji_poruku(poruka_greska);
            }
        }
        else
        {
            posalji_poruku(poruka_greska);
        }

        indeks_za_upisivanje = 0;
        return;
    }

    indeks_za_upisivanje = 0;
}


void prekid_serijske(void) interrupt 4
{
    if(TI)
    {
        TI = 0;
        if(*(++bafer_za_slanje))
            SBUF = *bafer_za_slanje;
    }

    if(RI)
    {
        uint8_t prijem = SBUF;
        RI = 0;

        if(prijem == POCETNI_KARAKTER)
            indeks_za_upisivanje = 0;

        if(indeks_za_upisivanje < KAPACITET-1)
            bafer_za_prijem[indeks_za_upisivanje++] = prijem;

        if(prijem == KRAJNJI_KARAKTER)
        {
            bafer_za_prijem[indeks_za_upisivanje] = 0;
            parsiraj_poruku();
        }
    }
}