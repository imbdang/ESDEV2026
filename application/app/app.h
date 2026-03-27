#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>
#include "m_sensor.h"
// #include "m_mpu.h"

extern Sensor_result sensor_result;

// extern uint8_t      mpu_id;
// extern MPU6050_Data mpu;

void app_run(void);

/* ============ a_motor.c ============ */
void Motor_Init(uint32_t core_clk_hz, uint32_t pwm_hz);
void MotorA_Set(int16_t speed);
void MotorB_Set(int16_t speed);
void Motor_Stop(void);
void Motor_Forward(int16_t speed);
void Motor_TurnLeft(int16_t left_speed, int16_t right_speed);
void Motor_TurnRight(int16_t left_speed, int16_t right_speed);
void Motor_RunPulses(int32_t pulses, int16_t speed);
void Motor_Left90(void);
void Motor_Right90(void);


/* ============ a_sensor.c ============ */
void a_read_sensor(void);

#endif /* __APP_H__ */
