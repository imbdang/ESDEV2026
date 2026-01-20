#include "m_gpio.h"

#include "h_gpio.h"
#include "h_rcc.h"
#include "systick.h"

void M_BlinkLED_PC13(void)
{
    RCC_APB2_Enable(RCC_APB2ENR_IOPCEN);

    GPIO_ConfigOutputPP(GPIOC, 13);

    while (1) 
    {
        GPIO_TogglePin(GPIOC, 13);
        delay_ms(1000);
    }
}
