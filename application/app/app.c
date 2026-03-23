#include "app.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "m_sensor.h"
#include "m_mpu.h"
#include "systick.h"

static uint8_t      tcs_id;
static TCS34_Data   tcs;

static uint8_t      mpu_id;
static MPU6050_Data mpu;

static void app_init(void)
{
    M_CLOCK_Set72MHz_HSE_PLL9();
    SysTick_Init_1ms(72000000);
    Motor_Init(72000000, 10000);

    // I2C_Init(I2C1, 36, 400);

    // MPU6050_ReadID(I2C1, &mpu_id);      
    // MPU6050_Init(I2C1, MPU6050_ACCEL_2G, MPU6050_GYRO_250);
    // delay_ms(30);

    /*
     * TCS34 is not connected — skip to avoid corrupting I2C bus.
     * Uncomment when sensor is wired up.
     */
    // TCS34_ReadID(I2C1, &tcs_id);  
    // TCS34_Init(I2C1, 0xF6, TCS34_GAIN_1X);
    // delay_ms(30);
}

void app_run(void)
{
    app_init();

    while (1)
    {
        MotorA_Set(500);
        MotorB_Set(500);
    }
}