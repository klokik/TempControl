#ifndef _DELAY_H_
#define _DELAY_H_

#include "stdint.h"

void Delay_Init(void);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);
void Delay_loop(uint32_t count);

#endif //_DELAY_H_
