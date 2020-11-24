#include "stm32f10x.h"                  // Device header

int main(){
	
	char data;
	
	//Enabling GPIO for USART2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   //USART2 ENABLE
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;		//CLOCK ENABLE FOR GPIOA
	
	//Pin configuration for USART2
	//TX pin Alternative push-pull config - 1011
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL &=~(1<<10); 
	GPIOA->CRL |=(1<<11);
	//RX pin floating input - default no need to do anyting
	
	//baudrate set-up
	//BUSCLOCK/(16*DESIRED_USART_SPEED) -> 72MHz/(16*9600) -> 468.75
	
	/*
	468 -> 1D4 DEC->HEX
	0.75 * 16 = 12 -> C
	468.75 = 0x1D4C
	*/
	
	USART2->BRR = 36000000/9600;
	
	//Enable RX, Enable TX, and USART bits in CR1 register
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (1<<3);
	USART2->CR1 |= (1<<2);
	
	while(1){
		if(USART2->SR & USART_SR_RXNE)
		{
			data = USART2->DR;
			USART2->DR = data;
		}
	}
}
