/*RM - rm0008, also ImBdang*/
#include "m_uart.h"
#include "h_rcc.h"
#include "h_gpio.h"
#include "h_usart.h"

/*
 * USART2 tren PA2 (TX) va PA3 (RX)
 * APB1 = 36 MHz
 * USARTDIV = 36000000 / (16 * 115200) = 19.5
 * BRR mantissa = 19, fraction = 0.5 * 16 = 8
 * BRR = (19 << 4) | 8 = 0x138  =>  actual 115384 baud (sai so 0.16%)
 */
#define UART_BRR_115200  0x138U

void M_UART_Init(void)
{
    /* Enable clock USART2 (APB1 bit 17) va GPIOA */
    RCC->APB1ENR |= (1U << 17);
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* PA2: TX - alternate function push-pull, 50 MHz */
    GPIO_ConfigPin(GPIOA, GPIO_PIN_2, GPIO_MODE_OUTPUT_50MHz, GPIO_CNF_OUT_AF_PP);

    /* PA3: RX - floating input */
    GPIO_ConfigPin(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_CNF_IN_FLOATING);

    /* Reset control registers */
    USART2->CR1 = 0;
    USART2->CR2 = 0;
    USART2->CR3 = 0;

    /* Baudrate 115200 */
    USART2->BRR = UART_BRR_115200;

    /* Enable TX, RX, USART (8N1 mac dinh) */
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void M_UART_SendByte(uint8_t byte)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = byte;
}

void M_UART_SendString(const char *str)
{
    while (*str) {
        M_UART_SendByte((uint8_t)*str++);
    }
}

uint8_t M_UART_DataReady(void)
{
    return (USART2->SR & USART_SR_RXNE) ? 1U : 0U;
}

uint8_t M_UART_Receive(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
    return (uint8_t)USART2->DR;
}
