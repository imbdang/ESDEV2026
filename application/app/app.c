#include "app.h"
#include "config.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "state_machine.h"
#include "systick.h"

Sensor_result sensor_result = {0};

Color_table  target_color;

State_car    current_state;

static void app_init(void)
{
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
    StateMachine_Init(C_RED);
}

void app_run(void)
{
    app_init();
    delay_ms(30);

    while (1)
    {
        a_read_sensor();
        StateMachine_Update();
        delay_ms(10);
    }
}
