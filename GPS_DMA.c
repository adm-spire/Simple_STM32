#include "stm32f0xx.h"
#include <string.h>

#define GPS_BUFFER_SIZE 100

volatile char gps_buffer[GPS_BUFFER_SIZE];
volatile int gps_data_ready = 0;
volatile int last_pos = 0;

void uart_dma_init(void);
void uart2_init(void);
void uart2_transmit(const char *str);

int main(){
	uart_dma_init();
	uart2_init();
	while(1){
		int cur_pos = GPS_BUFFER_SIZE - DMA1_Channel3->CNDTR;
        while (last_pos != cur_pos) {
        	char c = gps_buffer[last_pos++];
        	if (last_pos >= GPS_BUFFER_SIZE) last_pos = 0;

             static char sentence[GPS_BUFFER_SIZE];
        	 static int sentence_idx = 0;
        	 if (c == '\n') {
        	   sentence[sentence_idx] = '\0';
        	  uart2_transmit("Received GPS:\r\n");
        	  uart2_transmit(sentence);
        	  uart2_transmit("\r\n");
        	   sentence_idx = 0;
        	   } else if (sentence_idx < GPS_BUFFER_SIZE - 1) {
        	        sentence[sentence_idx++] = c;
        	    }
        }

	}

}

void uart_dma_init(void){
	//Enable clocks
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	// USART1 PA9 (TX), PA10 (RX)
	GPIOA->MODER |= (2 << (9 * 2)) | (2 << (10 * 2)); // AF
	GPIOA->AFR[1] |= (1 << ((9 - 8) * 4)) | (1 << ((10 - 8) * 4)); // AF1

	// USART1 config
	 USART1->BRR = 8000000 / 9600; // Assuming 8 MHz clock
	 USART1->CR3 |= USART_CR3_DMAR; // Enable DMA receiver
	 USART1->CR1 |= USART_CR1_RE | USART_CR1_UE;

	 // DMA config (Channel 3 for USART1_RX on STM32F072)
	 DMA1_Channel3->CNDTR = GPS_BUFFER_SIZE;
	 DMA1_Channel3->CPAR = (uint32_t)&USART1->RDR;
	 DMA1_Channel3->CMAR = (uint32_t)gps_buffer;
	 DMA1_Channel3->CCR =
	         DMA_CCR_MINC |     // Memory increment
	         DMA_CCR_CIRC |     // Circular mode
	         DMA_CCR_EN;        // Enable DMA
}

void uart2_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 (TX), PA3 (RX)
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));
    GPIOA->AFR[0] |= (1 << (2 * 4)) | (1 << (3 * 4));

    USART2->BRR = 8000000 / 38400;
    USART2->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void uart2_transmit(const char *str) {
    while (*str) {
        while (!(USART2->ISR & USART_ISR_TXE));
        USART2->TDR = *str++;
    }
}

