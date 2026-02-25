#ifndef GRAMOFON_H
#define GRAMOFON_H

#include "stdint.h"
#include "stdbool.h"

typedef enum {
    STANJE_STOP,
    STANJE_READY,      
    STANJE_BROJANJE,
    STANJE_POMERANJE
} stanje_t;

extern volatile stanje_t stanje;
extern volatile uint8_t broj_nitni;
extern volatile uint8_t pomeraj;

void gramofon_init(void);
void gramofon_update(void);
void detekcija_senzora(void);
void reset_brojanje(void);

#endif