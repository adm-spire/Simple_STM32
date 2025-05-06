#include "stm32f0xx.h"
#include "stm32f072xb.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"


#define GPIOAEN (1U << 17)
#define GPIOA_5 (1U << 5)

#define LED_PIN GPIOA_5

char key;

int main(void){
	char message[31] = "hello from stm32 dma transfer\n\r";
	uart2txrx_init();
	dma_init((uint32_t)message,(uint32_t)&USART2->TDR,31);

	RCC->AHBENR |= GPIOAEN;
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &=~ (1U << 11);

	while(1){

	}
}
void DMA1_Channel4_5_IRQHandler(void){
	if(DMA1->ISR & DMA_TCIF4){
		//CLEAR FLAG for interrupt
		DMA1->IFCR |= DMA_CTCIF4;
		//do something
		GPIOA-> ODR ^= LED_PIN;
		DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	}

}