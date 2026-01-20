#include <stdint.h>

#include "m_gpio.h"
#include "m_timer1.h"

void Motor_Init(uint32_t core_clk_hz, uint32_t pwm_hz)
{
    TB6612_InitPins();           
    TIM1_PWM_Init(core_clk_hz, pwm_hz); 
}

void MotorA_Set(int16_t speed)
{
    if (speed > 0) { GPIO_SetPin(GPIOB,0); GPIO_ClearPin(GPIOB,1); }
    else if (speed < 0) { GPIO_ClearPin(GPIOB,0); GPIO_SetPin(GPIOB,1); speed = -speed; }
    else { GPIO_ClearPin(GPIOB,0); GPIO_ClearPin(GPIOB,1); }

    TIM1_CH1_SetDutyPermille((uint16_t)((speed > 1000) ? 1000 : speed));
}

void MotorB_Set(int16_t speed)
{
    if (speed > 0) { GPIO_SetPin(GPIOB,10); GPIO_ClearPin(GPIOB,11); }
    else if (speed < 0) { GPIO_ClearPin(GPIOB,10); GPIO_SetPin(GPIOB,11); speed = -speed; }
    else { GPIO_ClearPin(GPIOB,10); GPIO_ClearPin(GPIOB,11); }

    TIM1_CH2_SetDutyPermille((uint16_t)((speed > 1000) ? 1000 : speed));
}
