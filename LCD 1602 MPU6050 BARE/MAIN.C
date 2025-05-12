#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

#define LCD_ADDR  0x27   // or 0x3F depending on your module
#define MPU6050_ADDR     0x68
#define PWR_MGMT_1       0x6B
#define ACCEL_XOUT_H     0x3B
void LCD_Init(void);
void LCD_SendString(char *str);
void LCD_I2C_WriteNibble(uint8_t nibble, uint8_t rs);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void MPU6050_Init(void);
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
volatile uint32_t sysTickCounter = 0;

void SysTick_Handler(void) {
    sysTickCounter++;
}

void delay_ms(uint32_t ms) {
    uint32_t target = sysTickCounter + ms;
    while (sysTickCounter < target);
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

void LCD_Init(void) {
    delay_ms(50); // Wait after power on
    LCD_I2C_WriteNibble(0x03, 0);
    delay_ms(5);
    LCD_I2C_WriteNibble(0x03, 0);
    delay_ms(5);
    LCD_I2C_WriteNibble(0x03, 0);
    delay_ms(5);
    LCD_I2C_WriteNibble(0x02, 0); // Set 4-bit mode

    LCD_SendCmd(0x28); // 4-bit, 2 lines, 5x8 font
    LCD_SendCmd(0x0C); // Display ON, Cursor OFF
    LCD_SendCmd(0x01); // Clear display
    delay_ms(2);
    LCD_SendCmd(0x06); // Entry mode set
}
void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_I2C_WriteNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = 0;

    data |= (nibble & 0x0F) << 4; // Correct shift to D4-D7
    if (rs) data |= (1 << 0);     // RS = P0
    data |= (1 << 2);             // EN = P2
    data |= (1 << 3);             // Backlight = P3

    I2C1_WriteByte(LCD_ADDR, 0x00, data);
    delay_ms(1);
    I2C1_WriteByte(LCD_ADDR, 0x00, data & ~(1 << 2)); // EN low
    delay_ms(1);
}

void LCD_I2C_SendByte(uint8_t byte, uint8_t rs) {
    LCD_I2C_WriteNibble(byte >> 4, rs);  // High nibble
    LCD_I2C_WriteNibble(byte & 0x0F, rs);  // Low nibble
}

void LCD_SendCmd(uint8_t cmd) {
    LCD_I2C_SendByte(cmd, 0);
}

void LCD_SendData(uint8_t data) {
    LCD_I2C_SendByte(data, 1);
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
    float ax_ms2, ay_ms2, az_ms2;
    char buffer[64];

    SysTick_Config(8000);

    I2C1_Init();
    LCD_Init();
    MPU6050_Init();

    while (1){
        MPU6050_ReadAccel(&ax, &ay, &az);

        // Convert to m/s² (1g = 9.81 m/s², and 1g = 16384 raw units)
        ax_ms2 = (ax / 16384.0f) * 9.81f;
        ay_ms2 = (ay / 16384.0f) * 9.81f;
        az_ms2 = (az / 16384.0f) * 9.81f;

        LCD_SendCmd(0x01);  // Clear
        delay_ms(2);

        LCD_SendCmd(0x80);  // Line 1
        sprintf(buffer, "X:%.2f Y:%.2f", ax_ms2, ay_ms2);
        LCD_SendString(buffer);

        LCD_SendCmd(0xC0);  // Line 2
        sprintf(buffer, "Z:%.2f", az_ms2);
        LCD_SendString(buffer);

        delay_ms(1000);
    }
}