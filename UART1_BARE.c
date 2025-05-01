#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#define GPIOAEN (1U << 17)
#define UART2EN (1U << 17)
#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200
#define CR1_TE (1U << 3)
#define CR1_UE (1U << 0)
#define ISR_TXE (1U << 7)


void uart2tx_init(void);
void uart2_write(int ch);

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphClk , uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk , uint32_t BaudRate);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}
int main(void){
	uart2tx_init();

	while(1){
		printf("hello from raunaq");

	}
}
void uart2tx_init(void){
	// gpioa enable
	RCC->AHBENR |= GPIOAEN;
	// alternate mode enabled
	GPIOA->MODER &=~(1U << 4);
	GPIOA->MODER |= (1U << 5);
	//Alternate function on GPIOA FOR UART2
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] &=~ (1U << 9);
	GPIOA->AFR[0] &=~ (1U << 10);
	GPIOA->AFR[0] &=~ (1U << 11);

	//clock for UART2
	RCC->APB1ENR |= UART2EN;
	//CONFIGURE BAUDRATE
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	USART2->CR1 = CR1_TE;
	USART2->CR1 |= CR1_UE;


}
void uart2_write(int ch){
	while (!(USART2->ISR & ISR_TXE));
	USART2->TDR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphClk , uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk , uint32_t BaudRate){
	return ((PeriphClk + (BaudRate/2U))/BaudRate);

}