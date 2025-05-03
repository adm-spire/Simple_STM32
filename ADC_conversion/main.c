#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;



int main(void){
    uart2txrx_init();
    pa1_adc_init();

    while(1){
        sensor_value = adc_read();
        printf("sensor value : %d\n\r", (int)sensor_value);
        for(volatile int i = 0; i < 100000; i++);  // small delay
    }
}