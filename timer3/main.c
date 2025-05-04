#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


int timestamp = 0;

int main(void){

    tim2_pa5_init();
    pa6_init_timer();
    uart2txrx_init();

    while(1){
    	while(!(TIM3->SR & SR_CC1IF));
    	timestamp = TIM3->CCR1;
    	printf("timestamp %d\n\r",timestamp);



    }
}