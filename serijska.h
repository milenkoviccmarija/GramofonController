#ifndef SERIJSKA_H
#define SERIJSKA_H

#include "stdint.h"

#define KAPACITET 20

void inicijalizacija_serijska(void);
void omoguci_prekid_serijske(void);
void posalji_poruku(uint8_t* poruka);
void prekid_serijske(void);      
void parsiraj_poruku(void);     

#endif