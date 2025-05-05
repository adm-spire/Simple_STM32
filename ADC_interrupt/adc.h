#ifndef UART_H_
#define UART_H_

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

void uart2txrx_init(void);
char uart2_read(void);
#define ADC_START   (1U << 2)

#endif