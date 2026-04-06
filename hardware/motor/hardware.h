#ifndef __HARDWARE_H__
#define __HARDWARE_H__

// ===================== TB6612FNG =====================
#define TB6612_PWMA_PORT    GPIOA
#define TB6612_PWMA_PIN     GPIO_PIN_9   // TIM1_CH2

#define TB6612_PWMB_PORT    GPIOA
#define TB6612_PWMB_PIN     GPIO_PIN_8   // TIM1_CH1

#define TB6612_AIN1_PORT    GPIOB
#define TB6612_AIN1_PIN     GPIO_PIN_0

#define TB6612_AIN2_PORT    GPIOB
#define TB6612_AIN2_PIN     GPIO_PIN_1

// Moved from PA2/PA3 → PB13/PB14 to free USART2
#define TB6612_BIN1_PORT    GPIOB
#define TB6612_BIN1_PIN     GPIO_PIN_13

#define TB6612_BIN2_PORT    GPIOB
#define TB6612_BIN2_PIN     GPIO_PIN_14

#define TB6612_STBY_PORT    GPIOB
#define TB6612_STBY_PIN     GPIO_PIN_12

// ===================== USART2 (freed up) =====================
// TX = PA2, RX = PA3  ← dùng cho Zigbee/module ngoài
#define UART2_TX_PORT       GPIOA
#define UART2_TX_PIN        GPIO_PIN_2

#define UART2_RX_PORT       GPIOA
#define UART2_RX_PIN        GPIO_PIN_3

// ===================== I2C1 (MPU6050 + Color Sensor) =====================
#define I2C1_SCL_PORT       GPIOB
#define I2C1_SCL_PIN        GPIO_PIN_6

#define I2C1_SDA_PORT       GPIOB
#define I2C1_SDA_PIN        GPIO_PIN_7

// ===================== I2C2 (Color Sensor 2) =====================
#define I2C2_SCL_PORT       GPIOB
#define I2C2_SCL_PIN        GPIO_PIN_10

#define I2C2_SDA_PORT       GPIOB
#define I2C2_SDA_PIN        GPIO_PIN_11

#endif /* __HARDWARE_H__ */