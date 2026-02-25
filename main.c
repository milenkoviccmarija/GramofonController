#include "gpio.h"
#include "serijska.h"
#include "timer1.h"
#include "gramofon.h"
#include "display.h"

void main(void)
{
    init_gpio();
    initDisplay();
    inicijalizacija_serijska();
    omoguci_prekid_serijske();
    init_timer1();
    gramofon_init();

    EA = 1;
    ET1 = 1;

    while(1)
    {
        gramofon_update();
    }
}
