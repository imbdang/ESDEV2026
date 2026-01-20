#ifndef __M_TIMER_H__
#define __M_TIMER_H__

#include "h_timer1.h"

/* ====================================== TIMER1 ======================================= */
void TIM1_PWM_Init(uint32_t core_clk_hz, uint32_t pwm_hz);
void TIM1_CH1_SetDutyPermille(uint16_t duty);
void TIM1_CH2_SetDutyPermille(uint16_t duty);

#endif /*<! __M_TIMER_H__ */