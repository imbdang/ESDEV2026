#include "app.h"
#include "m_clock.h"
#include "m_gpio.h"
#include "systick.h"
#include "encoder.h"

static void app_init(void)
{
    M_CLOCK_Set72MHz_HSE_PLL9();
    SysTick_Init_1ms(72000000); /*<! 72MHz */
    Motor_Init(72000000, 10000);
    Encoder_Init();
}

void app_run(void)
{
    app_init();
    int32_t rpmA = 0;
    int32_t rpmB = 0;
    uint32_t last_enc_tick   = 0;
    uint32_t last_motor_tick = 0;
    uint8_t  motor_phase     = 0;  /* 0: forward | 1: stop1 | 2: reverse | 3: stop2 */

    while (1)
    {
        uint32_t now = sys_get_tick();

        /* ---- Cap nhat RPM moi 100ms (non-blocking) ---- */
        if ((uint32_t)(now - last_enc_tick) >= 100U)
        {
            last_enc_tick += 100U;
            Encoder_Update();

            rpmA = EncoderA_GetRPM();
            rpmB = EncoderB_GetRPM();
            /* -> gui UART, PID, hien thi, v.v */
        }

        /* ---- Dieu khien motor (non-blocking state machine) ---- */
        switch (motor_phase)
        {
            case 0:  /* Chay tien 500ms */
                MotorA_Set(600);
                MotorB_Set(-600);
                motor_phase      = 1;
                last_motor_tick  = now;
                break;

            case 1:  /* Cho 500ms roi dung */
                if ((uint32_t)(now - last_motor_tick) >= 500U)
                {
                    MotorA_Set(0);
                    MotorB_Set(0);
                    motor_phase      = 2;
                    last_motor_tick  = now;
                }
                break;

            case 2:  /* Cho 1500ms roi chay lui */
                if ((uint32_t)(now - last_motor_tick) >= 1500U)
                {
                    MotorA_Set(-600);
                    MotorB_Set(600);
                    motor_phase      = 3;
                    last_motor_tick  = now;
                }
                break;

            case 3:  /* Cho 500ms roi dung va lap lai */
                if ((uint32_t)(now - last_motor_tick) >= 500U)
                {
                    MotorA_Set(0);
                    MotorB_Set(0);
                    motor_phase      = 0;
                    last_motor_tick  = now;
                }
                break;

            default:
                motor_phase = 0;
                break;
        }
    }
}