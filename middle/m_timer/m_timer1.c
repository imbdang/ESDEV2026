#include "m_timer1.h"
#include "h_rcc.h"

static uint16_t clamp_u16(uint16_t x, uint16_t lo, uint16_t hi)
{
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}

void TIM1_PWM_Init(uint32_t core_clk_hz, uint32_t pwm_hz)
{
    RCC_APB2_Enable(RCC_APB2ENR_TIM1EN);

    if (pwm_hz == 0)
        pwm_hz = 1000;

    /* calc PSC/ARR in middle */
    uint32_t psc = (core_clk_hz / 1000000U);
    if (psc == 0)
        psc = 1;
    psc -= 1U;
    if (psc > 0xFFFF)
        psc = 0xFFFF;

    uint32_t tim_clk = core_clk_hz / (psc + 1U);
    uint32_t arr = tim_clk / pwm_hz;
    if (arr == 0)
        arr = 1;
    arr -= 1U;
    if (arr > 0xFFFF)
        arr = 0xFFFF;

    /* apply via hardware APIs */
    TIM1_Stop();

    TIM1_SetPSC((uint16_t)psc);
    TIM1_SetARR((uint16_t)arr);

    /* PWM mode 1 + preload */
    TIM1_CH1_PWM1_Config(1);
    TIM1_CH2_PWM1_Config(1);

    /* active-high */
    TIM1_CH1_SetPolarity(0);
    TIM1_CH2_SetPolarity(0);

    /* duty = 0 */
    TIM1_CH1_SetDutyPermille(0);
    TIM1_CH2_SetDutyPermille(0);

    /* preload ARR + force update */
    TIM1_EnableARRPreload();
    TIM1_GenerateUpdate();

    /* enable outputs */
    TIM1_CH1_Enable();
    TIM1_CH2_Enable();

    /* advanced timer output enable */
    TIM1_MOE_Enable();

    TIM1_Start();
}

void TIM1_CH1_SetDutyPermille(uint16_t duty)
{
    duty = clamp_u16(duty, 0, 1000);
    uint16_t arr = TIM1_GetARR();
    uint16_t ccr = (uint16_t)(((uint32_t)(arr + 1U) * duty) / 1000U);
    TIM1_CH1_SetCCR(ccr);
}

void TIM1_CH2_SetDutyPermille(uint16_t duty)
{
    duty = clamp_u16(duty, 0, 1000);
    uint16_t arr = TIM1_GetARR();
    uint16_t ccr = (uint16_t)(((uint32_t)(arr + 1U) * duty) / 1000U);
    TIM1_CH2_SetCCR(ccr);
}
