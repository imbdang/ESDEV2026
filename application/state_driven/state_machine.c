#include "config.h"
#include "state_machine.h"
#include "app.h"

Color_table target_colors[STATE_MACHINE_MAX_TARGETS];
uint8_t target_color_count;
uint8_t target_color_index;
Color_table current_target;
Color_table target_color;
State_car current_state;
static uint8_t entry_turn_used_for_next_target;

static void state_machine_refresh_targets(void)
{
    if ((target_color_count == 0U) || (target_color_index >= target_color_count)) {
        current_target = C_NONE;
        target_color = C_NONE;
        return;
    }

    current_target = target_colors[target_color_index];

    if (target_color_index + 1U < target_color_count) {
        target_color = target_colors[target_color_index + 1U];
    } else {
        target_color = C_NONE;
    }
}

static State_car state_machine_decide(Color_table left, Color_table right)
{
    if ((target_color != C_NONE) && (left == target_color) && (right == target_color)) {
        StateMachine_AdvanceTarget();
        return STATE_FORWARD;
    }

    if ((target_color != C_NONE) &&
        (entry_turn_used_for_next_target == 0U) &&
        (left == target_color) &&
        (right != target_color)) {
        entry_turn_used_for_next_target = 1U;
        return STATE_ENTER_LEFT;
    }

    if ((target_color != C_NONE) &&
        (entry_turn_used_for_next_target == 0U) &&
        (right == target_color) &&
        (left != target_color)) {
        entry_turn_used_for_next_target = 1U;
        return STATE_ENTER_RIGHT;
    }

    if ((left == current_target) && (right == current_target)) return STATE_FORWARD;
    if ((left == current_target) && (right != current_target)) return STATE_TURN_LEFT;
    if ((right == current_target) && (left != current_target)) return STATE_TURN_RIGHT;

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

    case STATE_ENTER_LEFT:
        Motor_TurnLeft(CFG_SM_TURN_INNER_SPEED, CFG_SM_ENTRY_TURN_OUTER_SPEED);
        break;

    case STATE_ENTER_RIGHT:
        Motor_TurnRight(CFG_SM_ENTRY_TURN_OUTER_SPEED, CFG_SM_TURN_INNER_SPEED);
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

Color_table StateMachine_GetCurrentTarget(void)
{
    return current_target;
}

Color_table StateMachine_GetNextTarget(void)
{
    return target_color;
}

void StateMachine_SetTargetIndex(uint8_t index)
{
    if (index < target_color_count) {
        target_color_index = index;
        entry_turn_used_for_next_target = 0U;
        state_machine_refresh_targets();
    }
}

void StateMachine_AdvanceTarget(void)
{
    if ((target_color_count == 0U) || (target_color_index + 1U >= target_color_count)) {
        return;
    }

    target_color_index++;
    entry_turn_used_for_next_target = 0U;
    state_machine_refresh_targets();
}

void StateMachine_Init(const Color_table *targets, uint8_t count)
{
    uint8_t i;

    target_color_count = (uint8_t)((count > STATE_MACHINE_MAX_TARGETS) ?
                                   STATE_MACHINE_MAX_TARGETS : count);
    target_color_index = 0U;
    entry_turn_used_for_next_target = 0U;

    for (i = 0U; i < target_color_count; i++) {
        target_colors[i] = targets[i];
    }

    for (; i < STATE_MACHINE_MAX_TARGETS; i++) {
        target_colors[i] = C_NONE;
    }

    state_machine_refresh_targets();
    current_state = STATE_IDLE;

    Motor_Stop();
}

void StateMachine_Update(void)
{
    Color_table left = sensor_result.Color_SS_1;
    Color_table right = sensor_result.Color_SS_2;

    current_state = state_machine_decide(left, right);
    state_machine_apply(current_state);
}
