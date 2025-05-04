#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define GPIOAEN (1U << 17)
#define PIN5 (1U << 5)
#define LED_PIN PIN5







int main(void){
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~ (1U << 11);
    uart2txrx_init();


    while(1){
    	printf("a second passed\n");
    	GPIOA->ODR ^= LED_PIN;
    	systickDelayms(1000);

    }
}