#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>
#include "m_sensor.h"

typedef enum {
    STATE_IDLE = 0,
    STATE_FORWARD,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_LOST
} State_car;

extern Color_table target_color;
extern State_car current_state;

void StateMachine_Init(Color_table target);
void StateMachine_Update(void);

#endif /* __STATE_MACHINE_H__ */
