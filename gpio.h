#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "at89c51rc2.h"

#define MOTOR P2_0

void init_gpio(void);
void motor_on(void);
void motor_off(void);

#endif