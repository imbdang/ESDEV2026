#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "m_sensor.h"

/* ============================== TCS34 ====================================== */
#define CFG_TCS34_ATIME 0xEB
// #define CFG_TCS34_ATIME              0xD5U
#define CFG_TCS34_GAIN TCS34_GAIN_1X

// ATIME   Thoi gian tich phan
// 0xFF    2.4 ms
// 0xFE    4.8 ms
// 0xFD    7.2 ms
// 0xFC    9.6 ms
// 0xFB    12.0 ms
// 0xFA    14.4 ms
// 0xF9    16.8 ms
// 0xF8    19.2 ms
// 0xF7    21.6 ms
// 0xF6    24.0 ms
// 0xF5    26.4 ms
// 0xF4    28.8 ms
// 0xF0    38.4 ms
// 0xEB    50.4 ms
// 0xE0    76.8 ms
// 0xD5    103.2 ms
// 0xC0    153.6 ms
// 0xB0    192.0 ms
// 0x80    307.2 ms
// 0x40    460.8 ms
// 0x00    614.4 ms

/* ============================== Motor speed ================================ */
#define CFG_SM_FORWARD_SPEED (-480)
#define CFG_SM_TURN_INNER_SPEED (-220)
#define CFG_SM_TURN_OUTER_SPEED (-320)
#define CFG_SM_LOST_LEFT_SPEED (220)
#define CFG_SM_LOST_RIGHT_SPEED (-220)
#define CFG_SM_CHECKPOINT_COOLDOWN_MS (3000U)
#define CFG_SM_ENTRY_DETECT_STREAK (5U)
#define CFG_SM_MIN_ARM_FRAMES (3U)

#define CFG_TIME_TURN (390)
#define CFG_TIME_TURN45 (30)

#endif /* __CONFIG_H__ */
