/*RM - rm0008, also ImBdang*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

/* ====================================== TIMER1 REGISTER ======================================= */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

#define TIM1 ((volatile TIM_TypeDef*)0x40012C00)


void TIM1_Start(void);
void TIM1_Stop(void);

void TIM1_SetPSC(uint16_t psc);
void TIM1_SetARR(uint16_t arr);
uint16_t TIM1_GetARR(void);

void TIM1_EnableARRPreload(void);
void TIM1_GenerateUpdate(void);

void TIM1_MOE_Enable(void);
void TIM1_MOE_Disable(void);

void TIM1_CH1_Enable(void);
void TIM1_CH1_Disable(void);
void TIM1_CH2_Enable(void);
void TIM1_CH2_Disable(void);

void TIM1_CH1_SetCCR(uint16_t ccr);
void TIM1_CH2_SetCCR(uint16_t ccr);

void TIM1_CH1_PWM1_Config(uint8_t preload_en);
void TIM1_CH2_PWM1_Config(uint8_t preload_en);

void TIM1_CH1_SetPolarity(uint8_t active_low);
void TIM1_CH2_SetPolarity(uint8_t active_low);

#endif /*<! __TIMER_H__ */