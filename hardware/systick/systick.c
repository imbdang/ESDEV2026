#include "systick.h"

volatile uint32_t ms_ticks = 0;

uint32_t sys_get_tick(void)
{
    return ms_ticks;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = sys_get_tick();
    while ((uint32_t)(sys_get_tick() - start) < ms)
    {
        __asm volatile("nop");
    }
}

void SysTick_Init_1ms(uint32_t core_clk_hz)
{
    SYST_CSR = 0;
    SYST_RVR = (core_clk_hz / 1000U) - 1U;
    SYST_CVR = 0;
    SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
}
