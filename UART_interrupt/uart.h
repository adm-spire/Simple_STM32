#ifndef UART_H_
#define UART_H_

#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>

void uart2txrx_init(void);
char uart2_read(void);

#define ISR_RXNE (1U << 5)
#define CR1_RXNEIE (1U << 5)

#endif