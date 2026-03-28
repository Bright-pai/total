
#include "Delay.h"
void Delay_s(uint32_t s)
{
    delay_cycles(32000000*s);
}

void delay_ms(uint32_t ms)
{
    delay_cycles(32000*ms);
}

void Delay_us(uint32_t us)
{
    delay_cycles(32000*us);
}