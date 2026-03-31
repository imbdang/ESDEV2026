#ifndef __HARDWARE_H__
#define __HARDWARE_H__

// ===================== TB6612FNG (Dual DC Motor Driver) =====================
#define TB6612_PWMA_PORT GPIOA
#define TB6612_PWMA_PIN GPIO_PIN_9 // TIM1_CH1

#define TB6612_PWMB_PORT GPIOA
#define TB6612_PWMB_PIN GPIO_PIN_8 // TIM1_CH2

#define TB6612_AIN1_PORT GPIOB
#define TB6612_AIN1_PIN GPIO_PIN_0

#define TB6612_AIN2_PORT GPIOB
#define TB6612_AIN2_PIN GPIO_PIN_1

#define TB6612_BIN1_PORT GPIOA
#define TB6612_BIN1_PIN GPIO_PIN_2

#define TB6612_BIN2_PORT GPIOA
#define TB6612_BIN2_PIN GPIO_PIN_3

#define TB6612_STBY_PORT GPIOB
#define TB6612_STBY_PIN GPIO_PIN_12

// ===================== Encoder H602 1 - TIM2 =====================
#define ENC1_A_PORT GPIOA
#define ENC1_A_PIN GPIO_PIN_0 // TIM2_CH1

#define ENC1_B_PORT GPIOA
#define ENC1_B_PIN GPIO_PIN_5 // TIM2_CH2

// A0 A5

// ===================== I2C1 (MPU + Color Sensor I2C) =====================
#define I2C1_SCL_PORT GPIOB
#define I2C1_SCL_PIN GPIO_PIN_6 // I2C1_SCL

#define I2C1_SDA_PORT GPIOB
#define I2C1_SDA_PIN GPIO_PIN_7 // I2C1_SDA

#define I2C2_SCL_PORT GPIOB
#define I2C2_SCL_PIN GPIO_PIN_10 // I2C2_SCL

#define I2C2_SDA_PORT GPIOB
#define I2C2_SDA_PIN GPIO_PIN_11 // I2C2_SDA

// ===================== Optional Interrupt Pins (if you want) =====================
// For MPU INT (optional):
#define MPU_INT_PORT GPIOB
#define MPU_INT_PIN GPIO_PIN_5 // free GPIO, EXTI capable

// For Color Sensor INT (optional, e.g., TCS34725 INT):
#define COLOR_INT_PORT GPIOB
#define COLOR_INT_PIN GPIO_PIN_4 // free GPIO, EXTI capable

#endif /*<! __HARDWARE_H__ */