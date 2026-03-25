#include "config.h"
#include "state_machine.h"
#include "app.h"

static State_car state_machine_decide(Color_table left, Color_table right)
{
    if ((left == target_color) && (right == target_color)) return STATE_FORWARD;
    if ((left != target_color) && (right == target_color)) return STATE_TURN_LEFT;
    if ((left == target_color) && (right != target_color)) return STATE_TURN_RIGHT;
    return STATE_LOST;
}

static void state_machine_apply(State_car state)
{
    switch (state) {
    case STATE_FORWARD:
        Motor_Forward(CFG_SM_FORWARD_SPEED);
        break;

    case STATE_TURN_LEFT:
        Motor_TurnLeft(CFG_SM_TURN_INNER_SPEED, CFG_SM_TURN_OUTER_SPEED);
        break;

    case STATE_TURN_RIGHT:
        Motor_TurnRight(CFG_SM_TURN_OUTER_SPEED, CFG_SM_TURN_INNER_SPEED);
        break;

    case STATE_LOST:
        Motor_TurnLeft(CFG_SM_LOST_LEFT_SPEED, CFG_SM_LOST_RIGHT_SPEED);
        break;

    case STATE_IDLE:
    default:
        Motor_Stop();
        break;
    }
}

void StateMachine_Init(Color_table target)
{
    target_color = target;
    current_state = STATE_IDLE;

    Motor_Stop();
}

void StateMachine_Update(void)
{
    current_state = state_machine_decide(sensor_result.Color_SS_1,
                                         sensor_result.Color_SS_2);
    state_machine_apply(current_state);
}
