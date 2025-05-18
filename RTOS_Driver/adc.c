#include "stm32f0xx.h"
#include "adc.h"

#define GPIOAEN     (1U << 17)
#define ADCEN       (1U << 9)
#define ADC_CH1     (1U << 1)
#define ADC_EN      (1U << 0)
#define ADC_START   (1U << 2)
#define ADC_EOC     (1U << 2)
void adc_init(void) {
    // 1. Enable GPIOA clock
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // 2. Set PA1 to analog mode
    GPIOA->MODER |= (3U << 2);  // MODER1[1:0] = 11

    // 3. Enable ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    // 4. Calibration
    if (ADC1->CR & ADC_CR_ADEN) {
        ADC1->CR |= ADC_CR_ADDIS;
    }
    while (ADC1->CR & ADC_CR_ADEN);
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    // 5. Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    // 6. Select channel 1 (PA1)
    ADC1->CHSELR = ADC_CHSELR_CHSEL1;

    // 7. 12-bit resolution (default)
    ADC1->CFGR1 &= ~ADC_CFGR1_RES;

    // 8. Enable continuous conversion mode (optional)
    ADC1->CFGR1 |= ADC_CFGR1_CONT;

    // 9. Start ADC
    ADC1->CR |= ADC_CR_ADSTART;
}


uint32_t adc_read(void) {
    ADC1->CR |= ADC_START;
    while (!(ADC1->ISR & ADC_ISR_EOC));

    return ADC1->DR;

}