#include "m_sensor.h"

/* ============================== Registers =================================== */
#define TCS34_CMD           0x80U   /* command bit - must be set on every access */
#define TCS34_CMD_AUTO      0x10U   /* auto-increment flag for burst reads        */

#define TCS34_REG_ENABLE    0x00U
#define TCS34_REG_ATIME     0x01U
#define TCS34_REG_CONTROL   0x0FU
#define TCS34_REG_ID        0x12U
#define TCS34_REG_CDATA     0x14U   /* C_L, C_H, R_L, R_H, G_L, G_H, B_L, B_H  */

#define TCS34_ENABLE_PON    0x01U   /* power on    */
#define TCS34_ENABLE_AEN    0x02U   /* RGBC enable */

/* ============================== Public API ================================== */

I2C_Status TCS34_Init(volatile I2C_TypeDef *i2c, uint8_t atime, TCS34_Gain gain)
{
    I2C_Status s;

    /* integration time */
    s = I2C_WriteReg(i2c, TCS34_ADDR, TCS34_CMD | TCS34_REG_ATIME, atime);
    if (s) return s;

    /* gain */
    s = I2C_WriteReg(i2c, TCS34_ADDR, TCS34_CMD | TCS34_REG_CONTROL, (uint8_t)gain);
    if (s) return s;

    /* power on first, wait for oscillator (~3 ms) */
    s = I2C_WriteReg(i2c, TCS34_ADDR, TCS34_CMD | TCS34_REG_ENABLE, TCS34_ENABLE_PON);
    if (s) return s;

    /* caller must delay >= 3 ms here before AEN */
    /* split into two writes so the caller controls the delay externally */
    /* enable RGBC ADC */
    s = I2C_WriteReg(i2c, TCS34_ADDR, TCS34_CMD | TCS34_REG_ENABLE,
                     TCS34_ENABLE_PON | TCS34_ENABLE_AEN);
    return s;
}

I2C_Status TCS34_ReadID(volatile I2C_TypeDef *i2c, uint8_t *id)
{
    return I2C_ReadReg(i2c, TCS34_ADDR, TCS34_CMD | TCS34_REG_ID, id);
}

I2C_Status TCS34_Read(volatile I2C_TypeDef *i2c, TCS34_Data *out)
{
    uint8_t raw[8];
    I2C_Status s;

    s = I2C_ReadRegs(i2c, TCS34_ADDR,
                     TCS34_CMD | TCS34_CMD_AUTO | TCS34_REG_CDATA,
                     raw, 8);
    if (s) return s;

    out->c = (uint16_t)((raw[1] << 8) | raw[0]);
    out->r = (uint16_t)((raw[3] << 8) | raw[2]);
    out->g = (uint16_t)((raw[5] << 8) | raw[4]);
    out->b = (uint16_t)((raw[7] << 8) | raw[6]);

    return I2C_OK;
}

uint8_t TCS34_DetectColor(TCS34_Data *d)
{
    if (d->c < 15) return 0;   /* too dark, unreliable */

    uint8_t r = (d->r * 100) / d->c;
    uint8_t g = (d->g * 100) / d->c;
    uint8_t b = (d->b * 100) / d->c;

    if (r > g && r > b && r > 40) return 1;   /* red   */
    if (g > r && g > b && g > 30) return 2;   /* green */
    if (b > r && b > g && b > 25) return 3;   /* blue  */

    return 0;   /* white, black, or ambiguous */
}