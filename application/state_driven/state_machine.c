#include "state_machine.h"
#include "app.h"
#include "config.h"
#include "m_uart.h"
#include "systick.h"

Color_table target_colors[STATE_MACHINE_MAX_TARGETS];
uint8_t target_color_count;
uint8_t target_color_index;

Color_table current_target;
Color_table target_color;

State_car current_state;
Checkpoint current_checkpoint;
#if DEBUG_ENABLE
CheckpointLog checkpoint_log;
#endif
static uint8_t entry_turn_used_for_next_target;
static uint8_t checkpoint_entry_armed;
static uint8_t checkpoint_entry_arm_frames;
static uint8_t checkpoint_entry_target_streak;
static uint32_t checkpoint_cooldown_until_ms;
static uint8_t lost_backed_up;
static uint8_t center_confirm_count;
static State_car last_turn_state;
static uint8_t lost_debounce_count;

#if DEBUG_ENABLE
SM_Log sm_log;

static void sm_log_record(State_car state, const Sensor_result *s)
{
    SM_LogEntry *e = &sm_log.entries[sm_log.next];

    e->timestamp_ms = sys_get_tick();
    e->state        = state;
    e->c1 = s->Data_SS_1.c;  e->r1 = s->Data_SS_1.r;
    e->g1 = s->Data_SS_1.g;  e->b1 = s->Data_SS_1.b;
    e->c2 = s->Data_SS_2.c;  e->r2 = s->Data_SS_2.r;
    e->g2 = s->Data_SS_2.g;  e->b2 = s->Data_SS_2.b;
    e->color1 = s->Color_SS_1;
    e->color2 = s->Color_SS_2;

    sm_log.next = (uint16_t)((sm_log.next + 1U) % SM_LOG_SIZE);
    if (sm_log.count < SM_LOG_SIZE)
        sm_log.count++;
}
#endif /* DEBUG_ENABLE */

static void state_machine_advance_checkpoint(void)
{
    if (current_checkpoint < CHECK_POINT_4)
    {
        current_checkpoint = (Checkpoint)(current_checkpoint + 1U);
    }
}

#if DEBUG_ENABLE
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
#endif /* DEBUG_ENABLE */

static void state_machine_apply_checkpoint_entry_action(void)
{
    switch (current_checkpoint)
    {
    case CHECK_POINT_1:
        
        Motor_Forward(MAIN_SPEED);
        delay_ms(30);
        Motor_Stop();
        delay_ms(50);
        MotorB_Set(-MAIN_SPEED);
        MotorA_Set(MAIN_SPEED);
        delay_ms(CFG_CHECKPOINT1_TURN);

        M_UART_SendString("ESDEV10:CK-LUNGCU\r\n");

        for (uint8_t i = 0; i < 2; i++)
        {
            M_UART_SendString("ESDEV10:FW-HOANGSA\r\n");
            delay_ms(5);
        }

        break;

    case CHECK_POINT_2:
        Motor_Stop();
        delay_ms(50);
        MotorB_Set(-MAIN_SPEED);
        MotorA_Set(MAIN_SPEED);
        delay_ms(CFG_CHECKPOINT2_TURN);

        M_UART_SendString("ESDEV10:CK-HOANGSA\r\n");

        for (uint8_t i = 0; i < 2; i++)
        {
            M_UART_SendString("ESDEV10:FW-TRUONGSA\r\n");
            delay_ms(5);
        }

        break;

    case CHECK_POINT_3:
        Motor_Stop();
        delay_ms(50);
        MotorB_Set(-MAIN_SPEED);
        MotorA_Set(MAIN_SPEED);
        delay_ms(CFG_CHECKPOINT3_TURN);

        M_UART_SendString("ESDEV10:CK-TRUONGSA\r\n");

        for (uint8_t i = 0; i < 2; i++)
        {
            M_UART_SendString("ESDEV10:FW-CAMAU\r\n");
            delay_ms(5);
        }

        break;

    case CHECK_POINT_4:
        Motor_Stop();
        delay_ms(50);
        MotorB_Set(-MAIN_SPEED);
        MotorA_Set(MAIN_SPEED);
        delay_ms(CFG_CHECKPOINT4_TURN);
        current_checkpoint = CHECK_POINT_5;
        StateMachine_AdvanceTarget();

        M_UART_SendString("ESDEV10:CK-CAMAU\r\n");

        for (uint8_t i = 0; i < 2; i++)
        {
            M_UART_SendString("ESDEV10:FW-HANOI\r\n");
            delay_ms(5);
        }

        break;

    case CHECK_POINT_5:
        Motor_Stop();
        delay_ms(50);
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

        center_confirm_count++;
        if (center_confirm_count < CFG_SM_CENTER_CONFIRM)
        {
            /* Ca 2 sensor thay line nhung chua du frame - giu nguyen correction
             * truoc do, khong nhat vot len 1000/1000 ngay, tranh xe vot qua giua */
            return last_turn_state;
        }
        return STATE_FORWARD;
    }
    else
    {
        center_confirm_count = 0U;
        checkpoint_entry_arm_frames = 0U;
    }
    if ((left == current_target) && (right != current_target))
    {
        lost_debounce_count = 0U;
        last_turn_state = STATE_TURN_LEFT;
        return STATE_TURN_LEFT;
    }
    if ((right == current_target) && (left != current_target))
    {
        lost_debounce_count = 0U;
        last_turn_state = STATE_TURN_RIGHT;
        return STATE_TURN_RIGHT;
    }

    /* Ca 2 sensor NONE: tiep tuc turn cu them vai frame truoc khi spin LOST
     * tranh sensor mep chap chorn 1 frame lam xe spin ngay lap tuc */
    lost_debounce_count++;
    if (lost_debounce_count <= CFG_SM_LOST_DEBOUNCE)
        return last_turn_state;

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
#if DEBUG_ENABLE
        state_machine_log_entry_sensor_snapshot();
#endif
        Motor_Stop();
        delay_ms(50);
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

        // Motor_Stop();
        // while (1)
        // {
        //     /* code */
        // }
        
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
#if DEBUG_ENABLE
    checkpoint_log.next_index = 0U;
    checkpoint_log.count = 0U;
#endif
    entry_turn_used_for_next_target = 0U;
    checkpoint_entry_armed = 0U;
    checkpoint_entry_arm_frames = 0U;
    checkpoint_entry_target_streak = 0U;
    checkpoint_cooldown_until_ms = 0U;
    center_confirm_count = 0U;
    last_turn_state = STATE_FORWARD;
    lost_debounce_count = 0U;
#if DEBUG_ENABLE
    sm_log.next  = 0U;
    sm_log.count = 0U;
#endif

#if DEBUG_ENABLE
    for (i = 0U; i < STATE_MACHINE_ENTRY_LOG_SIZE; i++)
    {
        checkpoint_log.entries[i] = empty_result;
    }
#endif

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
    State_car prev_state;
    Color_table left  = sensor_result.Color_SS_1;
    Color_table right = sensor_result.Color_SS_2;

    prev_state    = current_state;
    current_state = state_machine_decide(left, right);

#if DEBUG_ENABLE
    if (current_state != prev_state)
        sm_log_record(current_state, &sensor_result);
#endif

    state_machine_apply(current_state);
}
