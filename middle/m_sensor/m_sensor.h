/* TCS34725 RGBC Color Sensor driver */
#ifndef __M_TCS34_H__
#define __M_TCS34_H__

#include <stdint.h>
#include "h_i2c.h"

/* ============================== Device ====================================== */
#define TCS34_ADDR      0x29U

/* ============================== Gain options ================================ */
typedef enum {
    TCS34_GAIN_1X  = 0x00,
    TCS34_GAIN_4X  = 0x01,
    TCS34_GAIN_16X = 0x02,
    TCS34_GAIN_60X = 0x03
} TCS34_Gain;

/* ============================== RGBC result ================================= */
typedef struct {
    uint16_t c;   /* clear  */
    uint16_t r;   /* red    */
    uint16_t g;   /* green  */
    uint16_t b;   /* blue   */
} TCS34_Data;

typedef enum 
{
    C_NONE = 0, //white maybe
    C_RED,
    C_GREEN,
    C_BLUE,
    C_YEALLOW,
} Color_table;

typedef struct {
    uint8_t     Tcs_ID_1;
    TCS34_Data  Data_SS_1;
    Color_table Color_SS_1;

    uint8_t     Tcs_ID_2;
    TCS34_Data  Data_SS_2;
    Color_table Color_SS_2;
} Sensor_result;

/* ============================== Public API ================================== */

/*
 * TCS34_Init  - power on, set integration time and gain
 *   i2c        : I2C1 or I2C2
 *   atime      : integration time register (0xFF=2.4ms .. 0x00=614ms)
 *                common values: 0xF6=24ms, 0xEB=50ms, 0xD5=101ms
 *   gain       : TCS34_GAIN_1X / 4X / 16X / 60X
 * Returns I2C_OK on success.
 */
I2C_Status TCS34_Init(volatile I2C_TypeDef *i2c, uint8_t atime, TCS34_Gain gain);

/*
 * TCS34_ReadID - read chip ID register (0x44 or 0x4D = OK)
 */
I2C_Status TCS34_ReadID(volatile I2C_TypeDef *i2c_1,
                        volatile I2C_TypeDef *i2c_2,
                        Sensor_result *result);

/*
 * TCS34_Read  - burst read all 4 channels into TCS34_Data
 */
I2C_Status TCS34_Read(volatile I2C_TypeDef *i2c_1,
                      volatile I2C_TypeDef *i2c_2,
                      Sensor_result *result);

void TCS34_DetectColor(Sensor_result *result);
void TCS34_DetectColorPostCP4(Sensor_result *result);

#endif /*<! __M_TCS34_H__ */
