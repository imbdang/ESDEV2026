#include <stdint.h>

void app_run(void);

void Motor_Init(uint32_t core_clk_hz, uint32_t pwm_hz);
void MotorA_Set(int16_t speed);
void MotorB_Set(int16_t speed);
void Motor_RunPulses(int32_t pulses, int16_t speed);