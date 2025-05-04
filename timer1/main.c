#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN (1U << 17)
#define PIN5 (1U << 5)
#define LED_PIN PIN5


int main(void){
	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~ (1U << 11);
    uart2txrx_init();
    tim2_1hz_init();


    while(1){
    	//wait for UIF
    	while(!(TIM2->SR & SR_UIF));
    	//Clear UIF
    	TIM2->SR &= ~SR_UIF;
    	printf("a second passed\n");
    	GPIOA->ODR ^= LED_PIN;


    }
}