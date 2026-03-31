#include "app.h"
#include "m_sensor.h"
#include "state_machine.h"

static uint8_t post_cp4_mode = 0U;

void a_read_sensor(void)
{
    Sensor_result tmp = sensor_result;
    if (TCS34_Read(I2C1, I2C2, &tmp) != I2C_OK)
        return;
    sensor_result = tmp;

    if (post_cp4_mode == 0U)
    {
        TCS34_DetectColor(&sensor_result);
        /* Switch khi: CP4 xong (CHECK_POINT_5) va ca 2 sensor dang thay line RED */
        if ((current_checkpoint == CHECK_POINT_5) &&
            (sensor_result.Color_SS_1 == C_RED) &&
            (sensor_result.Color_SS_2 == C_RED))
        {
            post_cp4_mode = 1U;
        }
    }
    else
    {
        TCS34_DetectColorPostCP4(&sensor_result);
    }
}
