#ifndef TIMER1_H
#define TIMER1_H

#include "stdint.h"

extern volatile uint32_t timer1_ms;
extern volatile uint32_t timer1_s;
void init_timer1(void);
void start_timer1(void);
void stop_timer1(void);
void reset_timer1(void);

#endif