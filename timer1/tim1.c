#include "stm32f0xx.h"
#include "tim.h"


#define TIM2EN (1U << 0)
#define TIM2_CR_1 (1U << 0)

void tim2_1hz_init(void){
	//clock access to timer
	RCC->APB1ENR |= TIM2EN;
	//Set prescaler
	TIM2->PSC = 800 - 1;
	//Auto reload value
	TIM2->ARR = 10000-1;
	// Counter clear
	TIM2->CNT = 0;
	//enable timer
	TIM2->CR1 |= TIM2_CR_1;


}