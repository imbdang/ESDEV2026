#include <stdint.h>
#include "m_gpio.h"
#include "hardware.h"
#include "h_rcc.h"

static void GPIO_ConfigAFPP50MHz(volatile GPIO_TypeDef *gpio, uint8_t pin)
{
    volatile uint32_t *cr;
    uint8_t shift;

    if (pin < 8) { cr = &gpio->CRL; shift = pin * 4; }
    else         { cr = &gpio->CRH; shift = (pin - 8) * 4; }

    *cr &= ~(0xFU << shift);
    *cr |=  (0xBU << shift);
}

void TB6612_InitPins(void)
{
    RCC_APB2_Enable(RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);

    GPIO_ConfigOutputPP(TB6612_AIN1_PORT, 0);   // PB0
    GPIO_ConfigOutputPP(TB6612_AIN2_PORT, 1);   // PB1
    GPIO_ConfigOutputPP(TB6612_BIN1_PORT, 10);  // PB10
    GPIO_ConfigOutputPP(TB6612_BIN2_PORT, 11);  // PB11
    GPIO_ConfigOutputPP(TB6612_STBY_PORT, 12);  // PB12

    GPIO_ConfigAFPP50MHz(TB6612_PWMA_PORT, 8);  // PA8 TIM1_CH1
    GPIO_ConfigAFPP50MHz(TB6612_PWMB_PORT, 9);  // PA9 TIM1_CH2

    GPIO_ClearPin(TB6612_AIN1_PORT, 0);
    GPIO_ClearPin(TB6612_AIN2_PORT, 1);
    GPIO_ClearPin(TB6612_BIN1_PORT, 10);
    GPIO_ClearPin(TB6612_BIN2_PORT, 11);

    GPIO_SetPin(TB6612_STBY_PORT, 12);
}
