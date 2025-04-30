#include "stm32f0xx.h"

#define GPIOAEN    (1U << 17)  // AHBENR for GPIOA is bit 17 on STM32F0
#define PIN5       (1U << 5)
#define LED_PIN    PIN5

int main(void) {
    // Enable clock for GPIOA
    RCC->AHBENR |= GPIOAEN;

    // Set PA5 to output mode
    GPIOA->MODER &= ~(3U << 10);  // Clear mode bits first
    GPIOA->MODER |=  (1U << 10);  // Set MODER5[1:0] to 01 (output)

    while(1) {
        GPIOA->BSRR = LED_PIN;  // Toggle LED

        for(volatile int i = 0; i < 100000; ++i);  // crude delay

        GPIOA->BSRR = (1U << 21);  // Toggle LED

        for(volatile int i = 0; i < 100000; ++i);  // crude delay
    }
}