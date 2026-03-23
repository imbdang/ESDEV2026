#include "app.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "m_sensor.h"
#include "m_mpu.h"
#include "systick.h"

static uint8_t      tcs_id_1;
static TCS34_Data   tcs_1;

static uint8_t      tcs_id_2;
static TCS34_Data   tcs_2;

static uint8_t      mpu_id;
static MPU6050_Data mpu;

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

    TCS34_ReadID(I2C1, &tcs_id_1);  
    TCS34_Init(I2C1, 0xF6, TCS34_GAIN_1X);
    delay_ms(30);

    TCS34_ReadID(I2C2, &tcs_id_2);  
    TCS34_Init(I2C2, 0xF6, TCS34_GAIN_1X);
    delay_ms(30);
}

void app_run(void)
{
    app_init();

    while (1)
    {
        TCS34_Read(I2C1, &tcs_1);
        TCS34_Read(I2C2, &tcs_2);
        delay_ms(500);
    }
}