#include "app.h"
#include "config.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "state_machine.h"
#include "systick.h"

Sensor_result sensor_result = {0};

static void app_init(void)
{
    static const Color_table target_order[] = {
        C_RED,
        C_BLUE,
        C_GREEN,
        C_BLUE,
        C_GREEN,
        C_RED,
        C_YEALLOW,
    };

    M_CLOCK_Set72MHz_HSE_PLL9();
    SysTick_Init_1ms(72000000);
    Motor_Init(72000000, 10000);

    I2C_Init(I2C1, 36, 400);
    I2C_Init(I2C2, 36, 400);

    // MPU6050_ReadID(I2C1, &mpu_id);
    // MPU6050_Init(I2C1, MPU6050_ACCEL_2G, MPU6050_GYRO_250);
    // delay_ms(30);

    TCS34_Init(I2C1, CFG_TCS34_ATIME, CFG_TCS34_GAIN);
    TCS34_Init(I2C2, CFG_TCS34_ATIME, CFG_TCS34_GAIN);
    delay_ms(30);

    TCS34_ReadID(I2C1, I2C2, &sensor_result);
    StateMachine_Init(target_order,
                      (uint8_t)(sizeof(target_order) / sizeof(target_order[0])));
}

void app_run(void)
{
    app_init();
    delay_ms(1000);
    // MotorA_Set(1000);
    // MotorB_Set(400);
    // delay_ms(4500);
    while (1)
    {
        // Motor_Stop();
        // Motor_Forward(1000);
        a_read_sensor();
        StateMachine_Update();
    }
}
