#include "h_i2c.h"
#include "h_rcc.h"
#include "h_gpio.h"

/* --------------------------------- Timeout --------------------------------- */
#define I2C_TIMEOUT 100000U

/* ========================== Private helpers ================================= */

static I2C_Status _wait_flag(volatile uint32_t *reg, uint32_t flag, uint8_t set)
{
    uint32_t t = I2C_TIMEOUT;
    if (set) {
        while (!(*reg & flag)) { if (!--t) return I2C_ERR_TIMEOUT; }
    } else {
        while ( (*reg & flag)) { if (!--t) return I2C_ERR_TIMEOUT; }
    }
    return I2C_OK;
}

static I2C_Status _check_errors(volatile I2C_TypeDef *i2c)
{
    if (i2c->SR1 & I2C_SR1_AF)   { i2c->SR1 &= ~I2C_SR1_AF;   return I2C_ERR_NACK; }
    if (i2c->SR1 & I2C_SR1_BERR) { i2c->SR1 &= ~I2C_SR1_BERR; return I2C_ERR_BUS;  }
    if (i2c->SR1 & I2C_SR1_ARLO) { i2c->SR1 &= ~I2C_SR1_ARLO; return I2C_ERR_BUS;  }
    return I2C_OK;
}

static I2C_Status _start(volatile I2C_TypeDef *i2c, uint8_t addr_rw)
{
    I2C_Status s;

    s = _wait_flag(&i2c->SR2, I2C_SR2_BUSY, 0);
    if (s) return s;

    i2c->CR1 |= I2C_CR1_START;
    s = _wait_flag(&i2c->SR1, I2C_SR1_SB, 1);
    if (s) return s;

    i2c->DR = addr_rw;
    s = _wait_flag(&i2c->SR1, I2C_SR1_ADDR, 1);
    if (s) { _check_errors(i2c); return s; }

    (void)i2c->SR1;
    (void)i2c->SR2;

    return I2C_OK;
}

static I2C_Status _repeated_start(volatile I2C_TypeDef *i2c, uint8_t addr_rw)
{
    I2C_Status s;

    i2c->CR1 |= I2C_CR1_START;
    s = _wait_flag(&i2c->SR1, I2C_SR1_SB, 1);
    if (s) return s;

    i2c->DR = addr_rw;
    s = _wait_flag(&i2c->SR1, I2C_SR1_ADDR, 1);
    if (s) { _check_errors(i2c); return s; }

    (void)i2c->SR1;
    (void)i2c->SR2;

    return I2C_OK;
}

static void _stop(volatile I2C_TypeDef *i2c)
{
    i2c->CR1 |= I2C_CR1_STOP;
}

/* ========================== Public API ===================================== */

void I2C_Init(volatile I2C_TypeDef *i2c, uint32_t pclk1_MHz, uint32_t speed_kHz)
{
    RCC_APB2ENR |= (1U << 3);    /* GPIOB */
    RCC_APB1ENR |= (1U << 21);   /* I2C1  */

    i2c->CR1 |=  I2C_CR1_SWRST;
    i2c->CR1 &= ~I2C_CR1_SWRST;
    i2c->CR1 &= ~I2C_CR1_PE;

    i2c->CR2 = pclk1_MHz & 0x3FU;

    if (speed_kHz <= 100) {
        uint32_t ccr = (pclk1_MHz * 1000U) / (2U * speed_kHz);
        if (ccr < 4) ccr = 4;
        i2c->CCR   = ccr & 0xFFFU;
        i2c->TRISE = pclk1_MHz + 1U;
    } else {
        uint32_t ccr = (pclk1_MHz * 1000U) / (3U * speed_kHz);
        if (ccr < 1) ccr = 1;
        i2c->CCR   = I2C_CCR_FS | (ccr & 0xFFFU);
        i2c->TRISE = (pclk1_MHz * 3U) / 10U + 1U;
    }

    i2c->CR1 |= I2C_CR1_PE;

    GPIO_ConfigPin(GPIOB, 6, GPIO_MODE_OUTPUT_10MHz, GPIO_CNF_OUT_AF_OD);
    GPIO_ConfigPin(GPIOB, 7, GPIO_MODE_OUTPUT_10MHz, GPIO_CNF_OUT_AF_OD);
}

I2C_Status I2C_Write(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                     const uint8_t *data, uint8_t len)
{
    I2C_Status s;

    s = _start(i2c, (uint8_t)(dev_addr << 1));
    if (s) return s;

    for (uint8_t i = 0; i < len; i++) {
        s = _wait_flag(&i2c->SR1, I2C_SR1_TXE, 1);
        if (s) { _stop(i2c); return s; }

        s = _check_errors(i2c);
        if (s) { _stop(i2c); return s; }

        i2c->DR = data[i];
    }

    s = _wait_flag(&i2c->SR1, I2C_SR1_BTF, 1);
    if (s) { _stop(i2c); return s; }

    _stop(i2c);
    return I2C_OK;
}

I2C_Status I2C_Read(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                    uint8_t *data, uint8_t len)
{
    I2C_Status s;

    if (len == 0) return I2C_OK;

    i2c->CR1 |= I2C_CR1_ACK;

    s = _start(i2c, (uint8_t)((dev_addr << 1) | 0x01U));
    if (s) return s;

    for (uint8_t i = 0; i < len; i++) {
        if (i == len - 1U) {
            i2c->CR1 &= ~I2C_CR1_ACK;
            _stop(i2c);
        }
        s = _wait_flag(&i2c->SR1, I2C_SR1_RXNE, 1);
        if (s) return s;

        data[i] = (uint8_t)i2c->DR;
    }

    return I2C_OK;
}

I2C_Status I2C_WriteReg(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                        uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = { reg, value };
    return I2C_Write(i2c, dev_addr, buf, 2);
}

I2C_Status I2C_ReadReg(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                       uint8_t reg, uint8_t *value)
{
    I2C_Status s;

    s = _start(i2c, (uint8_t)(dev_addr << 1));
    if (s) return s;

    _wait_flag(&i2c->SR1, I2C_SR1_TXE, 1);
    i2c->DR = reg;
    _wait_flag(&i2c->SR1, I2C_SR1_BTF, 1);

    i2c->CR1 |= I2C_CR1_ACK;
    s = _repeated_start(i2c, (uint8_t)((dev_addr << 1) | 0x01U));
    if (s) return s;

    i2c->CR1 &= ~I2C_CR1_ACK;
    _stop(i2c);
    _wait_flag(&i2c->SR1, I2C_SR1_RXNE, 1);
    *value = (uint8_t)i2c->DR;

    return I2C_OK;
}

I2C_Status I2C_ReadRegs(volatile I2C_TypeDef *i2c, uint8_t dev_addr,
                        uint8_t reg, uint8_t *buf, uint8_t len)
{
    I2C_Status s;

    s = _start(i2c, (uint8_t)(dev_addr << 1));
    if (s) return s;

    _wait_flag(&i2c->SR1, I2C_SR1_TXE, 1);
    i2c->DR = reg;
    _wait_flag(&i2c->SR1, I2C_SR1_BTF, 1);

    i2c->CR1 |= I2C_CR1_ACK;
    s = _repeated_start(i2c, (uint8_t)((dev_addr << 1) | 0x01U));
    if (s) return s;

    for (uint8_t i = 0; i < len; i++) {
        if (i == len - 1U) {
            i2c->CR1 &= ~I2C_CR1_ACK;
            _stop(i2c);
        }
        _wait_flag(&i2c->SR1, I2C_SR1_RXNE, 1);
        buf[i] = (uint8_t)i2c->DR;
    }

    return I2C_OK;
}