#include <stdint.h>

#include "config.h"
#include "m_encoder.h"
#include "m_gpio.h"
#include "m_mpu.h"
#include "m_timer1.h"
#include "systick.h"

static uint16_t motor_limit_speed(int16_t speed)
{
    uint16_t magnitude;

    if (speed < 0)
        magnitude = (uint16_t)(-speed);
    else
        magnitude = (uint16_t)speed;

    return (uint16_t)((magnitude > 1000U) ? 1000U : magnitude);
}

void Motor_Init(uint32_t core_clk_hz, uint32_t pwm_hz)
{
    TB6612_InitPins();
    TIM1_PWM_Init(core_clk_hz, pwm_hz);
    Encoder_Init();
}

void MotorA_Set(int16_t speed)
{
    if (speed > 0)
    {
        GPIO_ClearPin(GPIOB, 0);
        GPIO_SetPin(GPIOB, 1);
    }
    else if (speed < 0)
    {
        GPIO_SetPin(GPIOB, 0);
        GPIO_ClearPin(GPIOB, 1);
        speed = -speed;
    }
    else
    {
        GPIO_ClearPin(GPIOB, 0);
        GPIO_ClearPin(GPIOB, 1);
    }

    TIM1_CH1_SetDutyPermille(motor_limit_speed(speed));
}

void MotorB_Set(int16_t speed)
{
    if (speed > 0)
    {
        GPIO_ClearPin(GPIOB, 13);
        GPIO_SetPin(GPIOB, 14);
    }
    else if (speed < 0)
    {
        GPIO_SetPin(GPIOB, 13);
        GPIO_ClearPin(GPIOB, 14);
        speed = -speed;
    }
    else
    {
        GPIO_ClearPin(GPIOB, 13);
        GPIO_ClearPin(GPIOB, 14);
    }

    TIM1_CH2_SetDutyPermille(motor_limit_speed(speed));
}

void Motor_Stop(void)
{
    MotorA_Set(0);
    MotorB_Set(0);
}

void Motor_Forward(int16_t speed)
{
    MotorA_Set(speed);
    MotorB_Set(speed);
}

void Motor_TurnLeft(int16_t left_speed, int16_t right_speed)
{
    MotorA_Set(right_speed);
    MotorB_Set(left_speed);
}

void Motor_TurnRight(int16_t left_speed, int16_t right_speed)
{
    MotorA_Set(right_speed);
    MotorB_Set(left_speed);
}

void Motor_Left90(void)
{
    MotorA_Set(-400);
    MotorB_Set(400);
    delay_ms(CFG_TIME_TURN);
    Motor_Stop();
}

void Motor_Right90(void)
{
    MotorB_Set(400);
    MotorA_Set(-400);
    delay_ms(CFG_TIME_TURN);
    Motor_Stop();
}

void Motor_Left45(void)
{
    MotorA_Set(-400);
    MotorB_Set(400);
    delay_ms(CFG_TIME_TURN45);
    Motor_Stop();
}

void Motor_Right45(void)
{
    MotorB_Set(400);
    MotorA_Set(-400);
    delay_ms(CFG_TIME_TURN45);
    Motor_Stop();
}

void Motor_RunPulses(int32_t pulses, int16_t speed)
{
    if (pulses <= 0 || speed == 0)
        return;

    EncoderA_ResetCount();
    EncoderB_ResetCount();

    MotorA_Set(speed);
    MotorB_Set(speed);

    while (EncoderA_GetCount() < pulses || EncoderB_GetCount() < pulses)
        ;

    Motor_Stop();
}
