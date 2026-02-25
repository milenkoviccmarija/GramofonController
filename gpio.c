#include "gpio.h"

void init_gpio(void)
{
    P2 = 0x00;
    P0 = 0xFF;
    MOTOR = 0;
}

void motor_on(void)
{
    MOTOR = 1;
}

void motor_off(void)
{
    MOTOR = 0;
}