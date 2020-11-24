#include "stm32f10x.h"                  // Device header

int main(){
	char data[8] = {'1', '0', '0', '0', '1', '0', '0', '0'};
	int i = 0;
	
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN; // enable port a
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN; //enable usart1
	//TX pin alterntiv push-pull config 1011
	GPIOA -> CRH |= (1<<5);
	GPIOA -> CRH |= (1<<4);
	GPIOA -> CRH |= (1<<7);
	GPIOA -> CRH &= ~(1<<6);
	
	USART1 -> BRR = 72000000 / 9600;
	
	//Enable RX, Enable TX, and USART bits in CR1 register
	USART1->CR1 |= (1<<13);
	USART1->CR1 |= (1<<3);
	USART1->CR1 |= (1<<2);
	
	//Enabling 0 to 7 pins in a as output
	GPIOA->CRL = GPIO_CRL_MODE;
	GPIOA->CRL &= ~GPIO_CRL_CNF;
	
	while(1){
		if(USART1->SR & USART_SR_RXNE){
			data[i] = USART1->DR;
			while(!(USART1->SR & USART_SR_TC));
			i++;
		}
		if(i == 8){
			for(int y = 0; y < 8; y++){
				if(data[y] == '1')
					GPIOA -> BSRR = (1<<(0+y));
				else 
					GPIOA -> BSRR = (1<<(16+y));
			}
			i = 0;
		}
	}
}
