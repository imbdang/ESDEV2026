/*RM - rm0008, also ImBdang*/
#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

/* ====================================== GPIO REGISTER ======================================= */
typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

#define GPIOA ((volatile GPIO_TypeDef *)0x40010800)
#define GPIOB ((volatile GPIO_TypeDef *)0x40010C00)
#define GPIOC ((volatile GPIO_TypeDef *)0x40011000)
#define GPIOD ((volatile GPIO_TypeDef *)0x40011400)
#define GPIOE ((volatile GPIO_TypeDef *)0x40011800)
#define GPIOF ((volatile GPIO_TypeDef *)0x40011C00)
#define GPIOG ((volatile GPIO_TypeDef *)0x40012000)

typedef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_10MHz,
    GPIO_MODE_OUTPUT_2MHz,
    GPIO_MODE_OUTPUT_50MHz
} GPIO_Mode;

typedef enum
{
    GPIO_CNF_IN_ANALOG = 0,
    GPIO_CNF_IN_FLOATING = 1,
    GPIO_CNF_IN_PUPD = 2,

    GPIO_CNF_OUT_PP = 0,
    GPIO_CNF_OUT_OD = 1,
    GPIO_CNF_OUT_AF_PP = 2,
    GPIO_CNF_OUT_AF_OD = 3
} GPIO_Cnf;

void GPIO_ConfigPin(volatile GPIO_TypeDef *gpio, uint8_t pin, GPIO_Mode mode, GPIO_Cnf cnf);

void GPIO_ConfigOutputPP(volatile GPIO_TypeDef *gpio, uint8_t pin);
void GPIO_ConfigInputPullUp(volatile GPIO_TypeDef *gpio, uint8_t pin);
void GPIO_ConfigInputPullDown(volatile GPIO_TypeDef *gpio, uint8_t pin);

void GPIO_SetPin(volatile GPIO_TypeDef *gpio, uint8_t pin);
void GPIO_ClearPin(volatile GPIO_TypeDef *gpio, uint8_t pin);
void GPIO_WritePin(volatile GPIO_TypeDef *gpio, uint8_t pin, uint8_t value);
void GPIO_TogglePin(volatile GPIO_TypeDef *gpio, uint8_t pin);

void GPIO_SetMask(volatile GPIO_TypeDef *gpio, uint16_t mask);
void GPIO_ClearMask(volatile GPIO_TypeDef *gpio, uint16_t mask);

#endif /*<! __GPIO_H__ */