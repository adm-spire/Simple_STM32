#include "stm32f0xx.h"
#include "spi.h"


#define SPI1EN (1U << 12)
#define GPIOAEN (1U << 17)
#define SPI_TXE (1U << 1)
#define SPI_BUSY (1U << 7)

void spi_gpio_init(void){
	//clocK access to gpioa
	RCC->AHBENR |= GPIOAEN;
	//alternate function
	//PA4 output
	//GPIOA->MODER &=~ (1U << 9);
	//GPIOA->MODER |= (1U << 8);
	//PA5 alternate
	GPIOA->MODER |= (1U << 11);
	GPIOA->MODER &= ~(1U << 10);
	//PA6 alternate
	GPIOA->MODER |= (1U << 13);
	GPIOA->MODER &= ~(1U << 12);
	//PA7 alternate
	GPIOA->MODER |= (1U << 15);
	GPIOA->MODER &= ~(1U << 14);

	//alternate function type
	GPIOA->AFR[0] &=~ (0xF << 20);
	GPIOA->AFR[0] &=~ (0xF << 24);
	GPIOA->AFR[0] &=~ (0xF << 28);

}
void spi_config(void){
	//enable clock to spi
	RCC->APB2ENR |= SPI1EN;
	//prescaler divide by 2
	SPI1->CR1 &= ~(0x7 << 3);
	//CPOL AND CPHA changed here currently set to 1 and 1
	SPI1->CR1 |= (1U << 0);
	SPI1->CR1 |= (1U << 1);
	//full duplex
	SPI1->CR1 &=~(1U << 10);
	//LSB FIRST
	SPI1->CR1 &=~ (1U << 7);
	//master config
	SPI1->CR1 |= (1U << 2);
	//software slave management
	SPI1->CR1 |= (1U << 9);
	SPI1->CR1 |= (1U << 8);
	//disable SPI
	SPI1->CR1 &=~(1U << 6);
	//8 bit data
	SPI1->CR2 &=~(0XF << 8);
	SPI1->CR2 |= (0X7 << 8);
	//Enable SPI
	SPI1->CR1 |= (1U << 6);


}

void spi_transmit(uint8_t *data , uint32_t size){
	uint32_t i=0;
	uint8_t temp;
	while(i<size){
		//wait till txe is set
		while(!(SPI1->SR & SPI_TXE));
		//WRITE DATA
		SPI1->DR = data[i];
		i++;

	}
	//wait till txe is set
	while(!(SPI1->SR & SPI_TXE));
	//wait for busy flag to clear

	while((SPI1->SR & SPI_BUSY));
	//clear OVR flag
	temp = SPI1->DR;
	temp = SPI1->SR;



}
void spi_receive(uint8_t *data, uint32_t size) {
    uint32_t i = 0;

    while (i < size) {
        // Wait until TXE is set (ready to transmit dummy byte)
        while (!(SPI1->SR & SPI_SR_TXE));

        // Send dummy byte to generate clock
        SPI1->DR = 0xFF;

        // Wait until RXNE is set (data received)
        while (!(SPI1->SR & SPI_SR_RXNE));

        // Read the received data
        data[i] = SPI1->DR;
        i++;
    }

    // Wait until not busy
    while (SPI1->SR & SPI_SR_BSY);
}
