#include "m_mpu.h"

/* ============================== Registers =================================== */
#define MPU6050_REG_SMPLRT_DIV 0x19U   /* Sample rate divider          */
#define MPU6050_REG_CONFIG 0x1AU       /* DLPF config                  */
#define MPU6050_REG_GYRO_CONFIG 0x1BU  /* Gyroscope config             */
#define MPU6050_REG_ACCEL_CONFIG 0x1CU /* Accelerometer config         */
#define MPU6050_REG_ACCEL_XOUT_H 0x3BU /* Accel X high byte (burst start) */
#define MPU6050_REG_GYRO_XOUT_H 0x43U  /* Gyro X high byte             */
#define MPU6050_REG_PWR_MGMT_1 0x6BU   /* Power management 1           */
#define MPU6050_REG_WHO_AM_I 0x75U     /* Device ID (expect 0x68)      */

/* ============================== Public API ================================== */

I2C_Status MPU6050_Init(volatile I2C_TypeDef *i2c,
                        MPU6050_AccelRange accel,
                        MPU6050_GyroRange gyro)
{
    I2C_Status s;

    /* Wake up: clear SLEEP bit, use internal 8MHz oscillator */
    s = I2C_WriteReg(i2c, MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 0x00);
    if (s)
        return s;

    /* Sample rate = 1kHz / (1+3) = 250 Hz */
    s = I2C_WriteReg(i2c, MPU6050_ADDR, MPU6050_REG_SMPLRT_DIV, 0x03);
    if (s)
        return s;

    /* DLPF: bandwidth ~44Hz accel, ~42Hz gyro */
    s = I2C_WriteReg(i2c, MPU6050_ADDR, MPU6050_REG_CONFIG, 0x03);
    if (s)
        return s;

    /* Gyroscope range */
    s = I2C_WriteReg(i2c, MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG, (uint8_t)gyro);
    if (s)
        return s;

    /* Accelerometer range */
    s = I2C_WriteReg(i2c, MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG, (uint8_t)accel);
    return s;
}

I2C_Status MPU6050_ReadID(volatile I2C_TypeDef *i2c, uint8_t *id)
{
    return I2C_ReadReg(i2c, MPU6050_ADDR, MPU6050_REG_WHO_AM_I, id);
}

I2C_Status MPU6050_Read(volatile I2C_TypeDef *i2c, MPU6050_Data *out)
{
    uint8_t raw[14];
    I2C_Status s;

    /* Burst read 14 bytes: ACCEL(6) + TEMP(2) + GYRO(6) */
    s = I2C_ReadRegs(i2c, MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, raw, 14);
    if (s)
        return s;

    /* MPU6050 data is big-endian: HIGH byte first */
    out->ax = (int16_t)((raw[0] << 8) | raw[1]);
    out->ay = (int16_t)((raw[2] << 8) | raw[3]);
    out->az = (int16_t)((raw[4] << 8) | raw[5]);
    out->temp = (int16_t)((raw[6] << 8) | raw[7]);
    out->gx = (int16_t)((raw[8] << 8) | raw[9]);
    out->gy = (int16_t)((raw[10] << 8) | raw[11]);
    out->gz = (int16_t)((raw[12] << 8) | raw[13]);

    return I2C_OK;
}

I2C_Status MPU6050_ReadAccel(volatile I2C_TypeDef *i2c, MPU6050_Data *out)
{
    uint8_t raw[6];
    I2C_Status s;

    s = I2C_ReadRegs(i2c, MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, raw, 6);
    if (s)
        return s;

    out->ax = (int16_t)((raw[0] << 8) | raw[1]);
    out->ay = (int16_t)((raw[2] << 8) | raw[3]);
    out->az = (int16_t)((raw[4] << 8) | raw[5]);

    return I2C_OK;
}

I2C_Status MPU6050_ReadGyro(volatile I2C_TypeDef *i2c, MPU6050_Data *out)
{
    uint8_t raw[6];
    I2C_Status s;

    s = I2C_ReadRegs(i2c, MPU6050_ADDR, MPU6050_REG_GYRO_XOUT_H, raw, 6);
    if (s)
        return s;

    out->gx = (int16_t)((raw[0] << 8) | raw[1]);
    out->gy = (int16_t)((raw[2] << 8) | raw[3]);
    out->gz = (int16_t)((raw[4] << 8) | raw[5]);

    return I2C_OK;
}
