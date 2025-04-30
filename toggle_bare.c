#include "stm32f0xx.h"

#define GPIOAEN    (1U << 17)
#define GPIOCEN    (1U << 19)
#define PIN5       (1U << 5)
#define PIN13      (1U << 13)
#define LED_PIN    PIN5
#define BTN_PIN    PIN13

int main(void) {
    // Enable clocks
    RCC->AHBENR |= GPIOAEN;
    RCC->AHBENR |= GPIOCEN;

    // Set PA5 to output
    GPIOA->MODER &= ~(3U << (2 * 5));
    GPIOA->MODER |=  (1U << (2 * 5));

    // Set PC13 to input (00)
    GPIOC->MODER &= ~(3U << (2 * 13));

    while (1) {
        if (!(GPIOC->IDR & BTN_PIN)) {  // Button pressed (active-low)
            GPIOA->BSRR = LED_PIN;      // Set PA5
        } else {
            GPIOA->BSRR = (1U << (5 + 16)); // Reset PA5
        }
    }
}
