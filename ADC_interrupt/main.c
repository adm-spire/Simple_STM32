
uint32_t sensor_value;



int main(void){
    uart2txrx_init();
    pa1_adc_init();
    ADC1->CR |= ADC_START;

    while(1){

    }
}
void callback(void){
	sensor_value = ADC1->DR;
	printf("sensor value : %d\n\r", (int)sensor_value);

}
void ADC_COMP_IRQHandler(void){
	if((ADC1->ISR & ADC_ISR_EOC) != 0){
		//clear EOC
		ADC1->ISR |= ADC_ISR_EOC;
		//result
		callback();


	}
}