#include "app.h"
#include "m_clock.h"
#include "m_gpio.h"

#include "systick.h"

static void app_init(void)
{
    M_CLOCK_Set72MHz_HSE_PLL9();
    SysTick_Init_1ms(72000000); /*<! 72MHz */
    Motor_Init(72000000, 10000);
}

void app_run(void)
{
    app_init();
    while (1)
    {
        MotorA_Set(600);
        MotorB_Set(-600);
        delay_ms(500);
        MotorA_Set(0);
        MotorB_Set(0);
        delay_ms(1500);

        MotorA_Set(-600);
        MotorB_Set(600);
        delay_ms(500);
        MotorA_Set(0);
        MotorB_Set(0);
        delay_ms(1500);
    }
}