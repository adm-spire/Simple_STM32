#include "uart.h"
#include "stm32f072xb.h"
#define GPIOAEN (1U << 17)
#define UART2EN (1U << 17)
#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUDRATE 115200
#define CR1_TE (1U << 3)
#define CR1_RE (1U << 2)
#define CR1_UE (1U << 0)
#define ISR_TXE (1U << 7)
#define ISR_RXNE (1U << 5)
#define CR1_RXNEIE (1U << 5)
#define DMA_EN (1U << 0)

#define DMA_MINC (1U << 7)
#define DMA_DIR (1U << 4)
#define DMA_TCIE (1U << 1)
#define USART_DMAT (1U << 7)

void uart2_write(int ch);

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphClk , uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk , uint32_t BaudRate);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}
void dma_init(uint32_t src , uint32_t dst , uint32_t len){

	//enable clock to dma
		RCC->AHBENR |= DMA_EN;
		//disable buffer
		DMA1_Channel4->CCR &=~DMA_CCR_EN;
		//peripheral address
		DMA1_Channel4->CPAR = dst;
		//memory address
		DMA1_Channel4->CMAR = src;
		//length
		DMA1_Channel4->CNDTR = len;
		//increment
		DMA1_Channel4->CCR |= DMA_MINC;
		//direction
		DMA1_Channel4->CCR |= DMA_DIR;
		//transfer complete interrupt
		DMA1_Channel4->CCR |= DMA_TCIE;
		//enable DMA for USART
		USART2->CR3 |= USART_DMAT;
		//NVIC enable
		NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
		//enable buffer
		DMA1_Channel4->CCR |= DMA_CCR_EN;





}

void uart2txrx_init(void){
	// gpioa enable
	RCC->AHBENR |= GPIOAEN;
	// alternate mode enabled for PA2
	GPIOA->MODER &=~(1U << 4);
	GPIOA->MODER |= (1U << 5);
	//Alternate function on GPIOA FOR UART2
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] &=~ (1U << 9);
	GPIOA->AFR[0] &=~ (1U << 10);
	GPIOA->AFR[0] &=~ (1U << 11);

	//Alternate function for PA3
	GPIOA->MODER &=~(1U << 6);
	GPIOA->MODER |= (1U << 7);

		//Alternate function on GPIOA for UART2 RX
	GPIOA->AFR[0] |= (1U << 12);
	GPIOA->AFR[0] &=~ (1U << 13);
	GPIOA->AFR[0] &=~ (1U << 14);
	GPIOA->AFR[0] &=~ (1U << 15);

	//clock for UART2
	RCC->APB1ENR |= UART2EN;
	//CONFIGURE BAUDRATE
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	USART2->CR1 = (CR1_TE | CR1_RE);

	/* ************* Interrupt part ************/
	//Enable interrupt for USART
	USART2->CR1 |= CR1_RXNEIE;

	//Enable NVIC for USART2
	NVIC_EnableIRQ(USART2_IRQn);

	/* ************* Interrupt part ************/


	USART2->CR1 |= CR1_UE;






}
char uart2_read(void){
	while (!(USART2->ISR & ISR_RXNE)){}
	return USART2->RDR;


}
void uart2_write(int ch){
	while (!(USART2->ISR & ISR_TXE)){}
	USART2->TDR = (ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphClk , uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk,BaudRate);
}
static uint16_t compute_uart_bd(uint32_t PeriphClk , uint32_t BaudRate){
	return ((PeriphClk + (BaudRate/2U))/BaudRate);

}
