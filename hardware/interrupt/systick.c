#include "interrupt.h"
#include "stdint.h"

extern volatile uint32_t ms_ticks;

void SysTick_Handler(void)
{
    ms_ticks++;
}
