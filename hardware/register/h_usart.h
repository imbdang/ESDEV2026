/*RM - rm0008, also ImBdang*/
#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

/* ====================================== USART REGISTER ======================================= */
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART1 ((volatile USART_TypeDef *)0x40013800)
#define USART2 ((volatile USART_TypeDef *)0x40004400)
#define USART3 ((volatile USART_TypeDef *)0x40004800)
#define UART4  ((volatile USART_TypeDef *)0x40004C00)
#define UART5  ((volatile USART_TypeDef *)0x40005000)

/* SR bits */
#define USART_SR_RXNE  (1U << 5)  /*<! Read data register not empty */
#define USART_SR_TC    (1U << 6)  /*<! Transmission complete */
#define USART_SR_TXE   (1U << 7)  /*<! Transmit data register empty */

/* CR1 bits */
#define USART_CR1_RE   (1U << 2)  /*<! Receiver enable */
#define USART_CR1_TE   (1U << 3)  /*<! Transmitter enable */
#define USART_CR1_UE   (1U << 13) /*<! USART enable */

#endif /*<! __USART_H__ */
