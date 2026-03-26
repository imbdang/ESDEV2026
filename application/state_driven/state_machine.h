#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>
#include "m_sensor.h"

#define STATE_MACHINE_MAX_TARGETS    8U

typedef enum {
    STATE_IDLE = 0,
    STATE_FORWARD,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_ENTER_LEFT,
    STATE_ENTER_RIGHT,
    STATE_LOST
} State_car;

extern Color_table target_colors[STATE_MACHINE_MAX_TARGETS];
extern uint8_t target_color_count;
extern uint8_t target_color_index;
extern Color_table current_target;
extern Color_table target_color;
extern State_car current_state;

void StateMachine_Init(const Color_table *targets, uint8_t count);
Color_table StateMachine_GetCurrentTarget(void);
Color_table StateMachine_GetNextTarget(void);
void StateMachine_SetTargetIndex(uint8_t index);
void StateMachine_AdvanceTarget(void);
void StateMachine_Update(void);

#endif /* __STATE_MACHINE_H__ */
