#include "app.h"
#include "m_sensor.h"

void a_read_sensor(void)
{
    Sensor_result tmp = sensor_result;
    if (TCS34_Read(I2C1, I2C2, &tmp) == I2C_OK) {
        sensor_result = tmp;
        TCS34_DetectColor(&sensor_result);
    }
}

