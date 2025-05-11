
#ifndef SPI_H_
#define SPI_H_


#define SPI1EN (1U << 12)
#define GPIOAEN (1U << 17)
#define SPI_TXE (1U << 1)
#define SPI_BUSY (1U << 7)

void spi_gpio_init(void);
void spi_config(void);
void spi_transmit(uint8_t *data , uint32_t size);
void spi_receive(uint8_t *data, uint32_t size);

#endif /* SPI_H_ */