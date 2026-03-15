/*
 * PA0 -> EXTI0       -> Motor A encoder
 * PA5 -> EXTI9_5     -> Motor B encoder
 *
 * Disk: 20 holes per revolution
 * Speed formula: RPM = (pulse_in_100ms / 20) * 600
 *                    =  pulse_in_100ms * 30
 */

#include <stdint.h>
#include "m_encoder.h"

#define ENCODER_HOLES_PER_REV   20u
#define RPM_SCALE               30u


#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     (*(volatile uint32_t*)(RCC_BASE + 0x18))
#define RCC_APB2ENR_AFIOEN  (1u << 0)
#define RCC_APB2ENR_IOPAEN  (1u << 2)


#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRL       (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_ODR       (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))


#define AFIO_BASE       0x40010000UL
#define AFIO_EXTICR1    (*(volatile uint32_t*)(AFIO_BASE + 0x08))  /* EXTI0..3 */
#define AFIO_EXTICR2    (*(volatile uint32_t*)(AFIO_BASE + 0x0C))  /* EXTI4..7 */


#define EXTI_BASE       0x40010400UL
#define EXTI_IMR        (*(volatile uint32_t*)(EXTI_BASE + 0x00))
#define EXTI_RTSR       (*(volatile uint32_t*)(EXTI_BASE + 0x08))  /* Rising edge */
#define EXTI_PR         (*(volatile uint32_t*)(EXTI_BASE + 0x14))  /* Pending */

#define NVIC_ISER0      (*(volatile uint32_t*)0xE000E100)
#define NVIC_ISER1      (*(volatile uint32_t*)0xE000E104)
#define IRQ_EXTI0       6u   
#define IRQ_EXTI9_5     23u   


static volatile int32_t s_countA = 0; 
static volatile int32_t s_countB = 0; 

static volatile int32_t s_pulseA_100ms = 0;  
static volatile int32_t s_pulseB_100ms = 0;

static int32_t s_rpmA = 0;
static int32_t s_rpmB = 0;

void Encoder_Init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    /* 2. Cau hinh PA0, PA5 la input pull-up
     *    CRL: moi pin 4 bit -> PA0: bits[3:0], PA5: bits[23:20]
     *    Input pull-up: CNF=10, MODE=00 -> 0x8
     */
    /* PA0: xoa bits [3:0], dat 0x8 */
    GPIOA_CRL &= ~(0xFu << 0);
    GPIOA_CRL |=  (0x8u << 0);

    /* PA5: xoa bits [23:20], dat 0x8 */
    GPIOA_CRL &= ~(0xFu << 20);
    GPIOA_CRL |=  (0x8u << 20);

    /* Kich hoat pull-up bang ODR */
    GPIOA_ODR |= (1u << 0) | (1u << 5);

    /* 3. AFIO: chon EXTI0 va EXTI5 tu GPIOA (gia tri 0000 = PA) */
    /* EXTICR1 bits[3:0]  = 0000 -> EXTI0 = PA0 */
    AFIO_EXTICR1 &= ~(0xFu << 0);

    /* EXTICR2 bits[7:4]  = 0000 -> EXTI5 = PA5 */
    AFIO_EXTICR2 &= ~(0xFu << 4);

    /* 4. EXTI: bat mask va rising edge cho line 0 va 5 */
    EXTI_IMR  |= (1u << 0) | (1u << 5);
    EXTI_RTSR |= (1u << 0) | (1u << 5);

    /* 5. NVIC: bat EXTI0 (IRQ6) va EXTI9_5 (IRQ23) */
    NVIC_ISER0 |= (1u << IRQ_EXTI0);
    NVIC_ISER0 |= (1u << IRQ_EXTI9_5);
}

/* ===================== Goi moi 100ms ===================== */
void Encoder_Update(void)
{
    __asm volatile ("cpsid i");
    int32_t pA = s_pulseA_100ms;
    int32_t pB = s_pulseB_100ms;
    s_pulseA_100ms = 0;
    s_pulseB_100ms = 0;
    __asm volatile ("cpsie i");

    s_rpmA = pA * (int32_t)RPM_SCALE;
    s_rpmB = pB * (int32_t)RPM_SCALE;
}

int32_t EncoderA_GetCount(void) { return s_countA; }
int32_t EncoderB_GetCount(void) { return s_countB; }
int32_t EncoderA_GetRPM(void)   { return s_rpmA;   }
int32_t EncoderB_GetRPM(void)   { return s_rpmB;   }

void EncoderA_ResetCount(void) { s_countA = 0; }
void EncoderB_ResetCount(void) { s_countB = 0; }

void EXTI0_IRQHandler(void)
{
    if (EXTI_PR & (1u << 0))
    {
        s_countA++;
        s_pulseA_100ms++;
        EXTI_PR = (1u << 0);  
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_PR & (1u << 5))
    {
        s_countB++;
        s_pulseB_100ms++;
        EXTI_PR = (1u << 5);
    }
}