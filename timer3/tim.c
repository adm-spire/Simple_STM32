#include "stm32f0xx.h"
#include "tim.h"


#define TIM2EN (1U << 0)
#define TIM3EN (1U << 1)
#define TIM2_CR_1 (1U << 0)
#define TIM3_CR_1 (1U << 0)
#define TOGGLE (1U << 4) | (1U << 5)
#define CCER_CC1E (1U << 0)
#define GPIOAEN (1U << 17)
#define CCMR1_CC1S (1U << 0)


void tim2_pa5_init(void){
	//clock access to gpioa
	RCC->AHBENR |= GPIOAEN;
	//PA5 In alternate function mode
	GPIOA->MODER &=~ (1U << 10);
	GPIOA->MODER |= (1U << 11);
	//setting AFR TO AF02
	GPIOA->AFR[0] &=~ (1U <<20);
	GPIOA->AFR[0] |= (1U <<21);
	GPIOA->AFR[0] &=~ (1U <<22);
	GPIOA->AFR[0] &=~ (1U <<23);
	//clock access to timer
	RCC->APB1ENR |= TIM2EN;
	//Set prescaler
	TIM2->PSC = 800 - 1;
	//Auto reload value
	TIM2->ARR = 10000-1;

	//set  toggle mode
	TIM2->CCMR1 = TOGGLE;
	//TIM2 ch1 in compare mode
	TIM2->CCER |= CCER_CC1E;

	// Counter clear
	TIM2->CNT = 0;
	//enable timer
	TIM2->CR1 |= TIM2_CR_1;


}
void pa6_init_timer(void){
	//clock access to gpio
	RCC->AHBENR |= GPIOAEN;
	//PA6 in alternate function
	GPIOA->MODER &=~ (1U << 12);
	GPIOA->MODER |= (1U << 13);
	//alternative type selection AF01
	GPIOA->AFR[0] |= (1U << 24);
	GPIOA->AFR[0] &= ~ (1U << 25);
	GPIOA->AFR[0] &= ~ (1U << 26);
	GPIOA->AFR[0] &= ~ (1U << 27);
	//timer3 clock enable
	RCC->APB1ENR |= TIM3EN;
	//set prescaler
	TIM3->PSC = 8000 - 1;

	//set input capture mode
	TIM3->CCMR1 |= CCMR1_CC1S;
	//input enabled
	TIM3->CCER |= CCER_CC1E;
	//enable timer
	TIM3->CR1 |= TIM3_CR_1;




}