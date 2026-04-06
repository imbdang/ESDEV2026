#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "m_sensor.h"

/* ============================== TCS34 ====================================== */
#define CFG_TCS34_ATIME 0xF4

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
#define MAIN_SPEED (1000)

#define CFG_SM_FORWARD_SPEED    (MAIN_SPEED) /* ca 2 banh bang nhau khi 2 sensor thay mau */
#define CFG_SM_TURN_INNER_SPEED (600)        /* banh ben sensor thay line - giam de cua vao */
#define CFG_SM_TURN_OUTER_SPEED (MAIN_SPEED) /* banh ben sensor mat line - giu full */
#define CFG_SM_LOST_LEFT_SPEED (1000)
#define CFG_SM_LOST_RIGHT_SPEED (-1000)
#define CFG_SM_CHECKPOINT_COOLDOWN_MS (3000U)
#define CFG_SM_ENTRY_DETECT_STREAK (3U)
#define CFG_SM_MIN_ARM_FRAMES (3U)
#define CFG_SM_CENTER_CONFIRM (3U) /* so frame lien tiep ca 2 sensor phai thay line truoc khi FORWARD */
#define CFG_SM_LOST_DEBOUNCE  (4U) /* so frame ca 2 None truoc khi spin LOST - tranh sensor mep chap 1 frame */

#define CFG_TIME_TURN (390)
#define CFG_TIME_TURN45 (25)

#define CFG_CHECKPOINT1_TURN (1200)
#define CFG_CHECKPOINT2_TURN (200)
#define CFG_CHECKPOINT3_TURN (800)
#define CFG_CHECKPOINT4_TURN (800)

#endif /* __CONFIG_H__ */
