#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

#define MPU6050_ADDR     0x68
#define PWR_MGMT_1       0x6B
#define ACCEL_XOUT_H     0x3B

volatile uint32_t sysTickCounter = 0;

void SysTick_Handler(void) {
    sysTickCounter++;
}

void delay_ms(uint32_t ms) {
    uint32_t target = sysTickCounter + ms;
    while (sysTickCounter < target);
}


// UART2 TX on PA2
void UART2_Init(void) {
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER  |= (2 << (2 * 2));   // PA2 = AF
    GPIOA->AFR[0] |= (1 << (4 * 2));   // AF1 = USART2

    USART2->BRR = 8000000 / 38400;      // 9600 baud
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void UART2_SendChar(char c) {
    while (!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = c;
}

void UART2_SendString(char *str) {
    while (*str) UART2_SendChar(*str++);
}

// I2C1 on PB8=SCL, PB9=SDA
void I2C1_Init(void) {
    RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOB->MODER  |= (2 << (2 * 8)) | (2 << (2 * 9));  // PB8, PB9 = AF
    GPIOB->AFR[1] |= (1 << (0)) | (1 << (4));          // AF1 = I2C1

    I2C1->TIMINGR = 0x2000090E; // 100kHz I2C @ 8MHz
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_WriteByte(uint8_t addr, uint8_t reg, uint8_t data) {
    I2C1->CR2 = (addr << 1) | (2 << 16) | I2C_CR2_START;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = reg;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = data;
    while (!(I2C1->ISR & I2C_ISR_TC));
    I2C1->CR2 |= I2C_CR2_STOP;
}

void I2C1_ReadBytes(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len) {
    // Write register address
    I2C1->CR2 = (addr << 1) | (1 << 16) | I2C_CR2_START;
    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = reg;
    while (!(I2C1->ISR & I2C_ISR_TC));

    // Read 'len' bytes
    I2C1->CR2 = (addr << 1) | (len << 16) | I2C_CR2_RD_WRN | I2C_CR2_START;
    for (uint8_t i = 0; i < len; i++) {
        while (!(I2C1->ISR & I2C_ISR_RXNE));
        data[i] = I2C1->RXDR;
    }
    while (!(I2C1->ISR & I2C_ISR_TC));
    I2C1->CR2 |= I2C_CR2_STOP;
}

void MPU6050_Init(void) {
    delay_ms(100);
    I2C1_WriteByte(MPU6050_ADDR, PWR_MGMT_1, 0x00); // Wake up
    delay_ms(100);
}

void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t raw[6];
    I2C1_ReadBytes(MPU6050_ADDR, ACCEL_XOUT_H, raw, 6);

    *ax = (int16_t)(raw[0] << 8 | raw[1]);
    *ay = (int16_t)(raw[2] << 8 | raw[3]);
    *az = (int16_t)(raw[4] << 8 | raw[5]);
}

int main(void) {
    int16_t ax, ay, az;
    char buffer[64];
    SysTick_Config(8000);
    UART2_Init();
    I2C1_Init();
    MPU6050_Init();

    while (1) {
        MPU6050_ReadAccel(&ax, &ay, &az);
        snprintf(buffer, sizeof(buffer), "AX:%d AY:%d AZ:%d\r\n", ax, ay, az);
        UART2_SendString(buffer);
        delay_ms(1000);
    }
}