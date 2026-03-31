#include "state_machine.h"
#include "app.h"
#include "config.h"
#include "systick.h"

Color_table target_colors[STATE_MACHINE_MAX_TARGETS];
uint8_t target_color_count;
uint8_t target_color_index;

Color_table current_target;
Color_table target_color;

State_car current_state;
Checkpoint current_checkpoint;
CheckpointLog checkpoint_log;
static uint8_t entry_turn_used_for_next_target;
static uint8_t checkpoint_entry_armed;
static uint8_t checkpoint_entry_arm_frames;
static uint8_t checkpoint_entry_target_streak;
static uint32_t checkpoint_cooldown_until_ms;
static uint8_t lost_backed_up;

static void state_machine_advance_checkpoint(void)
{
    if (current_checkpoint < CHECK_POINT_4)
    {
        current_checkpoint = (Checkpoint)(current_checkpoint + 1U);
    }
}

static void state_machine_log_entry_sensor_snapshot(void)
{
    checkpoint_log.entries[checkpoint_log.next_index] = sensor_result;

    checkpoint_log.next_index++;
    if (checkpoint_log.next_index >= STATE_MACHINE_ENTRY_LOG_SIZE)
    {
        checkpoint_log.next_index = 0U;
    }

    if (checkpoint_log.count < STATE_MACHINE_ENTRY_LOG_SIZE)
    {
        checkpoint_log.count++;
    }
}

static void state_machine_apply_checkpoint_entry_action(void)
{
    switch (current_checkpoint)
    {
    case CHECK_POINT_1:
        Motor_Forward(-400);
        delay_ms(350);
        Motor_Stop();
        delay_ms(100);
        MotorB_Set(400);
        MotorA_Set(-400);
        delay_ms(CFG_CHECKPOINT1_TURN);
        break;

    case CHECK_POINT_2:
        Motor_Stop();
        delay_ms(100);
        MotorB_Set(400);
        MotorA_Set(-400);
        delay_ms(CFG_CHECKPOINT2_TURN);
        break;

    case CHECK_POINT_3:
        Motor_Stop();
        delay_ms(100);
        MotorB_Set(400);
        MotorA_Set(-400);
        delay_ms(CFG_CHECKPOINT3_TURN);
        break;

    case CHECK_POINT_4:
        Motor_Stop();
        delay_ms(100);
        MotorB_Set(400);
        MotorA_Set(-400);
        delay_ms(CFG_CHECKPOINT4_TURN);
        current_checkpoint = CHECK_POINT_5;
        StateMachine_AdvanceTarget();
        break;

    case CHECK_POINT_5:
        Motor_Stop();
        delay_ms(500);
        MotorB_Set(300);
        MotorA_Set(-300);
        while (1)
        {
        }
        break;

    default:
        break;
    }
}

static void state_machine_refresh_targets(void)
{
    if ((target_color_count == 0U) || (target_color_index >= target_color_count))
    {
        current_target = C_NONE;
        target_color = C_NONE;
        return;
    }

    current_target = target_colors[target_color_index];

    if (target_color_index + 1U < target_color_count)
    {
        target_color = target_colors[target_color_index + 1U];
    }
    else
    {
        target_color = C_NONE;
    }
}

static State_car state_machine_decide(Color_table left, Color_table right)
{
    uint32_t now_ms = sys_get_tick();
    uint8_t cooldown_active = ((int32_t)(now_ms - checkpoint_cooldown_until_ms) < 0) ? 1U : 0U;
    uint8_t sees_target = ((left == target_color) || (right == target_color)) ? 1U : 0U;
    uint8_t entry_ready = ((target_color != C_NONE) &&
                           (current_checkpoint != NONE_CHECK) &&
                           (checkpoint_entry_armed != 0U) &&
                           (cooldown_active == 0U) &&
                           (entry_turn_used_for_next_target == 0U))
                              ? 1U
                              : 0U;

    if ((entry_ready != 0U) && (sees_target != 0U))
    {
        if (checkpoint_entry_target_streak < CFG_SM_ENTRY_DETECT_STREAK)
        {
            checkpoint_entry_target_streak++;
        }
    }
    else
    {
        checkpoint_entry_target_streak = 0U;
    }

    if ((entry_ready != 0U) &&
        (checkpoint_entry_target_streak >= CFG_SM_ENTRY_DETECT_STREAK))
    {
        checkpoint_entry_target_streak = 0U;
        entry_turn_used_for_next_target = 1U;
        return STATE_ENTRY_CHECKPOINT;
    }

    if ((left == current_target) && (right == current_target))
    {
        if (checkpoint_entry_arm_frames < CFG_SM_MIN_ARM_FRAMES)
        {
            checkpoint_entry_arm_frames++;
        }
        else
        {
            checkpoint_entry_armed = 1U;
        }
        return STATE_FORWARD;
    }
    else
    {
        checkpoint_entry_arm_frames = 0U;
    }
    if ((left == current_target) && (right != current_target))
        return STATE_TURN_LEFT;
    if ((right == current_target) && (left != current_target))
        return STATE_TURN_RIGHT;

    return STATE_LOST;
}

static void state_machine_apply(State_car state)
{
    switch (state)
    {
    case STATE_FORWARD:
        lost_backed_up = 0U;
        Motor_Forward(CFG_SM_FORWARD_SPEED);
        break;

    case STATE_TURN_LEFT:
        lost_backed_up = 0U;
        Motor_TurnLeft(CFG_SM_TURN_INNER_SPEED, CFG_SM_TURN_OUTER_SPEED);
        break;

    case STATE_TURN_RIGHT:
        lost_backed_up = 0U;
        Motor_TurnRight(CFG_SM_TURN_OUTER_SPEED, CFG_SM_TURN_INNER_SPEED);
        break;

    case STATE_ENTRY_CHECKPOINT:
        state_machine_log_entry_sensor_snapshot();
        Motor_Stop();
        delay_ms(100);
        state_machine_apply_checkpoint_entry_action();
        state_machine_advance_checkpoint();
        StateMachine_AdvanceTarget();
        checkpoint_cooldown_until_ms = sys_get_tick() + CFG_SM_CHECKPOINT_COOLDOWN_MS;
        break;

    case STATE_LOST:
        // if (lost_backed_up == 0U)
        // {
        //     Motor_Stop();
        //     delay_ms(10);
        //     Motor_Forward(220);
        //     delay_ms(250);
        //     Motor_Stop();
        //     lost_backed_up = 1U;
        // }
        Motor_TurnRight(CFG_SM_LOST_LEFT_SPEED, CFG_SM_LOST_RIGHT_SPEED);
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
    if (index < target_color_count)
    {
        target_color_index = index;
        entry_turn_used_for_next_target = 0U;
        checkpoint_entry_armed = 0U;
        checkpoint_entry_arm_frames = 0U;
        checkpoint_entry_target_streak = 0U;
        checkpoint_cooldown_until_ms = 0U;
        if (index < (uint8_t)CHECK_POINT_4)
        {
            current_checkpoint = (Checkpoint)(index + 1U);
        }
        else
        {
            current_checkpoint = CHECK_POINT_4;
        }
        state_machine_refresh_targets();
    }
}

void StateMachine_AdvanceTarget(void)
{
    if ((target_color_count == 0U) || (target_color_index + 1U >= target_color_count))
    {
        return;
    }

    target_color_index++;
    entry_turn_used_for_next_target = 0U;
    checkpoint_entry_armed = 0U;
    checkpoint_entry_arm_frames = 0U;
    checkpoint_entry_target_streak = 0U;
    checkpoint_cooldown_until_ms = 0U;
    state_machine_refresh_targets();
}

void StateMachine_Init(const Color_table *targets, uint8_t count)
{
    uint8_t i;
    Sensor_result empty_result = {0};

    target_color_count = (uint8_t)((count > STATE_MACHINE_MAX_TARGETS) ? STATE_MACHINE_MAX_TARGETS : count);
    target_color_index = 0U;
    current_checkpoint = CHECK_POINT_1;
    checkpoint_log.next_index = 0U;
    checkpoint_log.count = 0U;
    entry_turn_used_for_next_target = 0U;
    checkpoint_entry_armed = 0U;
    checkpoint_entry_arm_frames = 0U;
    checkpoint_entry_target_streak = 0U;
    checkpoint_cooldown_until_ms = 0U;

    for (i = 0U; i < STATE_MACHINE_ENTRY_LOG_SIZE; i++)
    {
        checkpoint_log.entries[i] = empty_result;
    }

    for (i = 0U; i < target_color_count; i++)
    {
        target_colors[i] = targets[i];
    }

    for (; i < STATE_MACHINE_MAX_TARGETS; i++)
    {
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
