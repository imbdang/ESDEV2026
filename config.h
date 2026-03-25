#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "m_sensor.h"

/* ============================== TCS34 ====================================== */
#define CFG_TCS34_ATIME              0xF6U
#define CFG_TCS34_GAIN               TCS34_GAIN_1X

/* ============================== Motor speed ================================ */
#define CFG_SM_FORWARD_SPEED         (-320)
#define CFG_SM_TURN_INNER_SPEED      (-180)
#define CFG_SM_TURN_OUTER_SPEED      (-360)
#define CFG_SM_LOST_LEFT_SPEED       (220)
#define CFG_SM_LOST_RIGHT_SPEED      (-220)

#endif /* __CONFIG_H__ */
