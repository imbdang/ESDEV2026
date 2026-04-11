/*RM - rm0008, also ImBdang*/
#ifndef __M_UART_H__
#define __M_UART_H__

#include <stdint.h>

/*
 * USART2: PA2 (TX), PA3 (RX)
 * APB1 = 36 MHz, 115200 baud, 8N1
 */
void M_UART_Init(void);
void M_UART_SendByte(uint8_t byte);
void M_UART_SendString(const char *str);
uint8_t M_UART_Receive(void);
uint8_t M_UART_DataReady(void);

#endif /*<! __M_UART_H__ */
