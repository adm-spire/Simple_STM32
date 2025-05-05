#include "stm32f0xx.h"
#include "exti.h"

#define GPIOCEN (1U << 19)
#define SYSCFGEN (1U << 0)

void pc13_exti_init(void){
	//disable global interrupts
	__disable_irq();
	//enable clock for GPIOC
	RCC->AHBENR |= GPIOCEN;
	//set PC13 as input
	GPIOC->MODER &=~ (1U << 26);
	GPIOC->MODER &=~ (1U << 27);

	//enable clock access to SYSCFG
	RCC->APB2ENR |= SYSCFGEN;
	//select PC13 for EXTI
	SYSCFG->EXTICR[3] |= (1U << 5);
	//UNMASK pc13 interrupt
	EXTI->IMR |= (1U << 13);
	//falling edge
	EXTI->FTSR |= (1U << 13);
	//Enable Interrupt
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	//enable interrupts
	__enable_irq();



}