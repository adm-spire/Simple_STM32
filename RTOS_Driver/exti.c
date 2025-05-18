#include "stm32f0xx.h"

void EXTI_Init(void) {
    // 1. Enable GPIOC clock (Button: PC13)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // 2. Set PC13 as input (reset state, but explicit for clarity)
    GPIOC->MODER &= ~(3 << (13 * 2));  // Input mode (00)

    // 3. Enable pull-up on PC13 (button is active-low)
    GPIOC->PUPDR &= ~(3 << (13 * 2));  // Clear pull settings
    GPIOC->PUPDR |=  (1 << (13 * 2));  // 01 = pull-up

    // 4. Enable SYSCFG clock (required for EXTI configuration)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    // 5. Connect EXTI13 to PC13 (EXTICR4 because EXTI13 is in EXTICR[3])
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;  // Clear bits
    SYSCFG->EXTICR[3] |=  (2 << 4);               // 0010 = PC13 (2 = GPIOC)

    // 6. Configure EXTI13: unmask, falling edge (button press)
    EXTI->IMR  |= (1 << 13);  // Unmask EXTI13
    EXTI->FTSR |= (1 << 13);  // Falling edge trigger

    // 7. Enable EXTI4_15 interrupt in NVIC
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void LED_Init(void) {
    // 1. Enable GPIOA clock (LED: PA5)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // 2. Set PA5 as output (01)
    GPIOA->MODER &= ~(3 << (5 * 2));  // Clear mode bits
    GPIOA->MODER |=  (1 << (5 * 2));  // Output mode (01)
}

// Interrupt Service Routine for EXTI4_15 (includes EXTI13)
void EXTI4_15_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {      // Check if EXTI13 triggered
        GPIOA->ODR ^= (1 << 5);      // Toggle LED (PA5)
        EXTI->PR |= (1 << 13);      // Clear pending bit
    }
}
