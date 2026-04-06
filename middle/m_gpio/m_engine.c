#include "h_rcc.h"
#include "hardware.h"
#include "m_gpio.h"
#include <stdint.h>

static void GPIO_ConfigAFPP50MHz(volatile GPIO_TypeDef *gpio, uint8_t pin)
{
    volatile uint32_t *cr;
    uint8_t shift;

    if (pin < 8)
    {
        cr = &gpio->CRL;
        shift = pin * 4;
    }
    else
    {
        cr = &gpio->CRH;
        shift = (pin - 8) * 4;
    }

    *cr &= ~(0xFU << shift);
    *cr |= (0xBU << shift);
}

void TB6612_InitPins(void)
{
    RCC_APB2_Enable(RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);

    GPIO_ConfigOutputPP(TB6612_AIN1_PORT, TB6612_AIN1_PIN); // PB0
    GPIO_ConfigOutputPP(TB6612_AIN2_PORT, TB6612_AIN2_PIN); // PB1
    GPIO_ConfigOutputPP(TB6612_BIN1_PORT, TB6612_BIN1_PIN); // PB13
    GPIO_ConfigOutputPP(TB6612_BIN2_PORT, TB6612_BIN2_PIN); // PB14
    GPIO_ConfigOutputPP(TB6612_STBY_PORT, TB6612_STBY_PIN); // PB12

    GPIO_ConfigAFPP50MHz(TB6612_PWMA_PORT, TB6612_PWMA_PIN); // PA9 TIM1_CH2
    GPIO_ConfigAFPP50MHz(TB6612_PWMB_PORT, TB6612_PWMB_PIN); // PA8 TIM1_CH1

    GPIO_ClearPin(TB6612_AIN1_PORT, TB6612_AIN1_PIN);
    GPIO_ClearPin(TB6612_AIN2_PORT, TB6612_AIN2_PIN);
    GPIO_ClearPin(TB6612_BIN1_PORT, TB6612_BIN1_PIN);
    GPIO_ClearPin(TB6612_BIN2_PORT, TB6612_BIN2_PIN);

    GPIO_SetPin(TB6612_STBY_PORT, TB6612_STBY_PIN);
}
