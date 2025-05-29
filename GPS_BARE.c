#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>




#define GPS_BUFFER_SIZE 100
volatile char gps_buffer[GPS_BUFFER_SIZE];
volatile int gps_index = 0;
volatile int gps_data_ready = 0;
volatile uint8_t rx_byte = 0;


void uart_init(void);
void USART1_IRQHandler(void);
void uart2_transmit(const char *str);

int main(void) {
    uart_init();

    while (1) {
        if (gps_data_ready) {
            uart2_transmit("Received GPS:\r\n");
            uart2_transmit((char*)gps_buffer);
            uart2_transmit("\r\n");
            gps_data_ready = 0;
        }
    }
}

void uart_init(void) {
    // Enable clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // USART1 - PA9 (TX), PA10 (RX)
    GPIOA->MODER |= (2 << (9 * 2)) | (2 << (10 * 2));    // Alternate function
    GPIOA->AFR[1] |= (1 << ((9 - 8) * 4)) | (1 << ((10 - 8) * 4)); // AF1 for USART1

    // USART2 - PA2 (TX), PA3 (RX)
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));    // Alternate function
    GPIOA->AFR[0] |= (1 << (2 * 4)) | (1 << (3 * 4));   // AF1 for USART2

    // USART1 config: 9600 baud, 8N1
    USART1->BRR = 8000000 / 9600;
    USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;

    // USART2 config: 115200 baud, 8N1
    USART2->BRR = 8000000 / 38400;
    USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

    // Enable USART1 interrupt
    NVIC_EnableIRQ(USART1_IRQn);
}


void USART1_IRQHandler(void){
	if(USART1->ISR & USART_ISR_RXNE){
		rx_byte = USART1->RDR; //Read data

		if(rx_byte == '\n'){
			gps_buffer[gps_index] = '\0';
			gps_data_ready = 1;
			gps_index = 0;
		}
		else{
			if(gps_index < GPS_BUFFER_SIZE - 1){
				gps_buffer[gps_index++] = rx_byte;
			}
		}
	}
}

void uart2_transmit(const char *str) {
    while (*str) {
        while (!(USART2->ISR & USART_ISR_TXE)); // Wait for TX buffer empty
        USART2->TDR = *str++;
    }
}
