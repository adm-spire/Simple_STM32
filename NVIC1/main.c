#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"


static void exti_callback(void);

int main(void){

	pc13_exti_init();
	uart2txrx_init();

    while(1){


    }
}
static void exti_callback(void){
	printf("btn pressed\n\r");
}

void EXTI4_15_IRQHandler(void){
	if((EXTI->PR & LINE13) != 0){
		//CLEAR flag
		EXTI->PR |= LINE13;
		//do something
		exti_callback();

	}

}