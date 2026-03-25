#include "app.h"
#include "m_sensor.h"

void a_read_sensor(void)
{
    TCS34_Read(I2C1, I2C2, &sensor_result);
    TCS34_DetectColor(&sensor_result);
}

