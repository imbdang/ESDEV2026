#include "app.h"
#include "config.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "m_uart.h"
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
    M_UART_Init();
    Motor_Init(72000000, 10000);

    I2C_Init(I2C1, 36, 400);
    I2C_Init(I2C2, 36, 400);

    // MPU6050_ReadID(I2C1, &mpu_id);
    // MPU6050_Init(I2C1, MPU6050_ACCEL_2G, MPU6050_GYRO_250);
    // delay_ms(30);

    while (TCS34_Init(I2C1, CFG_TCS34_ATIME, CFG_TCS34_GAIN) != I2C_OK)
        I2C_Recover(I2C1, 36, 400);
    while (TCS34_Init(I2C2, CFG_TCS34_ATIME, CFG_TCS34_GAIN) != I2C_OK)
        I2C_Recover(I2C2, 36, 400);
    delay_ms(30);

    TCS34_ReadID(I2C1, I2C2, &sensor_result);
    StateMachine_Init(target_order,
                      (uint8_t)(sizeof(target_order) / sizeof(target_order[0])));
}

void app_run(void)
{
    app_init();
    delay_ms(100);

        //     Motor_Forward(400);
        // delay_ms(200);
        // Motor_Stop();
        // delay_ms(50);
        // MotorB_Set(-400);
        // MotorA_Set(400);
        // delay_ms(CFG_CHECKPOINT1_TURN);
        // Motor_Stop();

    M_UART_SendString("ESDEV10:XP-HA NOI\r\n");
    delay_ms(10);
    for (uint8_t i = 0; i < 2; i++)
    {
        M_UART_SendString("ESDEV10:FW-LUNGCU\r\n");
        delay_ms(5);
    }
    while (1)
    {
        // a_read_sensor();
        // StateMachine_Update();
    }
}
