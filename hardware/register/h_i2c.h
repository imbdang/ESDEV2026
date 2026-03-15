/* RM - rm0008 */
#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

/* ====================================== I2C REGISTER ====================================== */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_TypeDef;

#define I2C1 ((volatile I2C_TypeDef*)0x40005400)
#define I2C2 ((volatile I2C_TypeDef*)0x40005800)

/* --------------------------------- CR1 bits --------------------------------- */
#define I2C_CR1_PE          (1U << 0)   /* Peripheral enable            */
#define I2C_CR1_START       (1U << 8)   /* Generate START               */
#define I2C_CR1_STOP        (1U << 9)   /* Generate STOP                */
#define I2C_CR1_ACK         (1U << 10)   /* Acknowledge enable           */
#define I2C_CR1_SWRST       (1U << 15)   /* Software reset               */

/* --------------------------------- SR1 bits --------------------------------- */
#define I2C_SR1_SB          (1U << 0)   /* Start bit generated          */
#define I2C_SR1_ADDR        (1U << 1)   /* Address sent / matched       */
#define I2C_SR1_BTF         (1U << 2)   /* Byte transfer finished       */
#define I2C_SR1_RXNE        (1U << 6)   /* Data register not empty      */
#define I2C_SR1_TXE         (1U << 7)   /* Data register empty          */
#define I2C_SR1_BERR        (1U << 8)   /* Bus error                    */
#define I2C_SR1_ARLO        (1U << 9)   /* Arbitration lost             */
#define I2C_SR1_AF          (1U << 10)   /* Acknowledge failure          */
#define I2C_SR1_OVR         (1U << 11)   /* Overrun / underrun           */

/* --------------------------------- SR2 bits --------------------------------- */
#define I2C_SR2_BUSY        (1U << 1)   /* Bus busy                     */

/* --------------------------------- CCR bits --------------------------------- */
#define I2C_CCR_FS          (1U << 15)   /* Fast mode select             */

/* ------------------------------- Return codes ------------------------------- */
typedef enum {
    I2C_OK      = 0,
    I2C_ERR_TIMEOUT,
    I2C_ERR_NACK,
    I2C_ERR_BUS
} I2C_Status;

/* ============================== PUBLIC API ================================== */

void       I2C_Init(volatile I2C_TypeDef *i2c, uint32_t pclk1_MHz, uint32_t speed_kHz);

I2C_Status I2C_Write(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                     const uint8_t *data, uint8_t len);

I2C_Status I2C_Read(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                    uint8_t *data, uint8_t len);

I2C_Status I2C_WriteReg(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                        uint8_t reg, uint8_t value);

I2C_Status I2C_ReadReg(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                       uint8_t reg, uint8_t *value);

I2C_Status I2C_ReadRegs(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                        uint8_t reg, uint8_t *buf, uint8_t len);

#endif /*<! __I2C_H__ */