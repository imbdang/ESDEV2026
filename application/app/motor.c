#include <stdint.h>

#include "m_gpio.h"
#include "m_timer1.h"
#include "m_encoder.h"
#include "m_mpu.h"
#include "systick.h"

void Motor_Init(uint32_t core_clk_hz, uint32_t pwm_hz)
{
    TB6612_InitPins();           
    TIM1_PWM_Init(core_clk_hz, pwm_hz); 
    Encoder_Init();
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
    if (speed > 0) { GPIO_SetPin(GPIOA,2); GPIO_ClearPin(GPIOA,3); }
    else if (speed < 0) { GPIO_ClearPin(GPIOA,2); GPIO_SetPin(GPIOA,3); speed = -speed; }
    else { GPIO_ClearPin(GPIOA,2); GPIO_ClearPin(GPIOA,3); }

    TIM1_CH2_SetDutyPermille((uint16_t)((speed > 1000) ? 1000 : speed));
}

void Motor_RunPulses(int32_t pulses, int16_t speed)
{
    if (pulses <= 0 || speed == 0) return;

    EncoderA_ResetCount();
    EncoderB_ResetCount();

    MotorA_Set(speed);
    MotorB_Set(speed);

    while (EncoderA_GetCount() < pulses || EncoderB_GetCount() < pulses)
        ;

    MotorA_Set(0);
    MotorB_Set(0);
}
