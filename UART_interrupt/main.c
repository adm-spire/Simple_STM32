#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"


#define GPIOAEN (1U << 17)
#define GPIOA_5 (1U << 5)

#define LED_PIN GPIOA_5

char key;

int main(void){
	uart2txrx_init();

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~ (1U << 11);

	while(1){

	}
}

void USART2_IRQHandler(void){
	//check if RXNE Is set
	if((USART2->ISR & ISR_RXNE)){
		key = USART2->RDR;
		if(key == '1'){
					GPIOA->ODR |= LED_PIN;
				}
				else{
					GPIOA->ODR &= ~LED_PIN;
				}

	}
}