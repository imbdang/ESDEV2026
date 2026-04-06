#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "m_sensor.h"
#include <stdint.h>

#define STATE_MACHINE_MAX_TARGETS 8U
#define STATE_MACHINE_ENTRY_LOG_SIZE 6U

typedef enum
{
    STATE_IDLE = 0,
    STATE_FORWARD,
    STATE_TURN_LEFT,
    STATE_TURN_RIGHT,
    STATE_ENTRY_CHECKPOINT,
    STATE_LOST
} State_car;

/* ============================== State Log =================================== */
#define SM_LOG_ENABLE               /* comment out de tat log */
#define SM_LOG_SIZE (64U)           /* so entry luu, tang len neu can xem nhieu hon */

typedef struct
{
    uint32_t    timestamp_ms;
    State_car   state;
    uint16_t    c1, r1, g1, b1;  /* raw sensor 1 */
    uint16_t    c2, r2, g2, b2;  /* raw sensor 2 */
    Color_table color1;
    Color_table color2;
} SM_LogEntry;

typedef struct
{
    SM_LogEntry entries[SM_LOG_SIZE];
    uint16_t    next;   /* vi tri ghi tiep theo (ring buffer) */
    uint16_t    count;  /* tong so entry da ghi (toi da SM_LOG_SIZE) */
} SM_Log;

extern SM_Log sm_log;

typedef enum
{
    CHECK_POINT_1 = 0,
    CHECK_POINT_2,
    CHECK_POINT_3,
    CHECK_POINT_4,
    CHECK_POINT_5,
    NONE_CHECK,
} Checkpoint;

typedef struct
{
    Sensor_result entries[STATE_MACHINE_ENTRY_LOG_SIZE];
    uint8_t count;
    uint8_t next_index;
} CheckpointLog;

extern Color_table target_colors[STATE_MACHINE_MAX_TARGETS];
extern uint8_t target_color_count;
extern uint8_t target_color_index;
extern Color_table current_target;
extern Color_table target_color;
extern State_car current_state;
extern Checkpoint current_checkpoint;
extern CheckpointLog checkpoint_log;

void StateMachine_Init(const Color_table *targets, uint8_t count);
Color_table StateMachine_GetCurrentTarget(void);
Color_table StateMachine_GetNextTarget(void);
void StateMachine_SetTargetIndex(uint8_t index);
void StateMachine_AdvanceTarget(void);
void StateMachine_Update(void);

#endif /* __STATE_MACHINE_H__ */
