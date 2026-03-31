/* MPU6050 6-axis IMU driver (I2C) */
#ifndef __M_MPU_H__
#define __M_MPU_H__

#include "h_i2c.h"
#include <stdint.h>

/* ============================== Device address ============================== */
#define MPU6050_ADDR 0x68U /* AD0 = GND -> 0x68, AD0 = VCC -> 0x69 */

/* ============================== Data structs ================================ */
typedef struct
{
    int16_t ax;   /* accelerometer X  (raw) */
    int16_t ay;   /* accelerometer Y  (raw) */
    int16_t az;   /* accelerometer Z  (raw) */
    int16_t temp; /* temperature      (raw) */
    int16_t gx;   /* gyroscope X      (raw) */
    int16_t gy;   /* gyroscope Y      (raw) */
    int16_t gz;   /* gyroscope Z      (raw) */
} MPU6050_Data;

/* ============================== Accel range ================================= */
typedef enum
{
    MPU6050_ACCEL_2G = 0x00, /* +/- 2g   (16384 LSB/g) */
    MPU6050_ACCEL_4G = 0x08, /* +/- 4g   (8192  LSB/g) */
    MPU6050_ACCEL_8G = 0x10, /* +/- 8g   (4096  LSB/g) */
    MPU6050_ACCEL_16G = 0x18 /* +/- 16g  (2048  LSB/g) */
} MPU6050_AccelRange;

/* ============================== Gyro range ================================== */
typedef enum
{
    MPU6050_GYRO_250 = 0x00,  /* +/- 250  deg/s  (131   LSB/dps) */
    MPU6050_GYRO_500 = 0x08,  /* +/- 500  deg/s  (65.5  LSB/dps) */
    MPU6050_GYRO_1000 = 0x10, /* +/- 1000 deg/s  (32.8  LSB/dps) */
    MPU6050_GYRO_2000 = 0x18  /* +/- 2000 deg/s  (16.4  LSB/dps) */
} MPU6050_GyroRange;

/* ============================== Public API ================================== */

/*
 * MPU6050_Init - wake up sensor, set accel/gyro range, DLPF ~44Hz
 *   i2c   : I2C1 or I2C2
 *   accel : MPU6050_ACCEL_2G / 4G / 8G / 16G
 *   gyro  : MPU6050_GYRO_250 / 500 / 1000 / 2000
 */
I2C_Status MPU6050_Init(volatile I2C_TypeDef *i2c,
                        MPU6050_AccelRange accel,
                        MPU6050_GyroRange gyro);

/*
 * MPU6050_ReadID - read WHO_AM_I register (expect 0x68)
 */
I2C_Status MPU6050_ReadID(volatile I2C_TypeDef *i2c, uint8_t *id);

/*
 * MPU6050_Read - burst read accel + temp + gyro (14 bytes)
 */
I2C_Status MPU6050_Read(volatile I2C_TypeDef *i2c, MPU6050_Data *out);

/*
 * MPU6050_ReadAccel - read only accelerometer (6 bytes)
 */
I2C_Status MPU6050_ReadAccel(volatile I2C_TypeDef *i2c, MPU6050_Data *out);

/*
 * MPU6050_ReadGyro - read only gyroscope (6 bytes)
 */
I2C_Status MPU6050_ReadGyro(volatile I2C_TypeDef *i2c, MPU6050_Data *out);

#endif /*<! __M_MPU_H__ */
