#include "h_timer.h"

void TIM1_SetPSC(uint16_t psc)
{
    TIM1->PSC = psc;
}

void TIM1_SetARR(uint16_t arr)
{
    TIM1->ARR = arr;
}

void TIM1_EnableARRPreload(void)
{
    TIM1->CR1 |= (1U << 7);
}

void TIM1_GenerateUpdate(void)
{
    TIM1->EGR = 1U;
}

uint16_t TIM1_GetARR(void)
{
    return (uint16_t)TIM1->ARR;
}

void TIM1_MOE_Enable(void)
{
    TIM1->BDTR |= (1U << 15);
}

void TIM1_MOE_Disable(void)
{
    TIM1->BDTR &= ~(1U << 15);
}

void TIM1_Start(void)
{
    TIM1->CR1 |= (1U << 0);
}

void TIM1_Stop(void)
{
    TIM1->CR1 &= ~(1U << 0);
}

void TIM1_CH1_Enable(void)
{
    TIM1->CCER |= (1U << 0);
}

void TIM1_CH1_Disable(void)
{
    TIM1->CCER &= ~(1U << 0);
}

void TIM1_CH2_Enable(void)
{
    TIM1->CCER |= (1U << 4);
}

void TIM1_CH2_Disable(void)
{
    TIM1->CCER &= ~(1U << 4);
}

void TIM1_CH1_SetCCR(uint16_t ccr)
{
    TIM1->CCR1 = ccr;
}

void TIM1_CH2_SetCCR(uint16_t ccr)
{
    TIM1->CCR2 = ccr;
}

void TIM1_CH1_PWM1_Config(uint8_t preload_en)
{
    TIM1->CCMR1 &= ~((7U << 4) | (1U << 3));
    TIM1->CCMR1 |= (6U << 4);
    if (preload_en)
        TIM1->CCMR1 |= (1U << 3);
}

void TIM1_CH2_PWM1_Config(uint8_t preload_en)
{
    TIM1->CCMR1 &= ~((7U << 12) | (1U << 11));
    TIM1->CCMR1 |= (6U << 12);
    if (preload_en)
        TIM1->CCMR1 |= (1U << 11);
}

void TIM1_CH1_SetPolarity(uint8_t active_low)
{
    if (active_low)
        TIM1->CCER |= (1U << 1);
    else
        TIM1->CCER &= ~(1U << 1);
}

void TIM1_CH2_SetPolarity(uint8_t active_low)
{
    if (active_low)
        TIM1->CCER |= (1U << 5);
    else
        TIM1->CCER &= ~(1U << 5);
}
