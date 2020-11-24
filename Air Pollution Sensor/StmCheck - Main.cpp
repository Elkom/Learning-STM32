#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

//Function for sending data
static void sendSerial(char *msg, ...){
	char buff[140];
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);
	
	for(int i = 0; i < strlen(buff); i++){
		USART1 -> DR = buff[i];
		while(!(USART1 -> SR & USART_SR_TXE));
	}
}


/*
USART1 - For PC 	(RX - A10, TX - A9)
USART2 - For Sensor (RX - A3,  TX - A2) - air pollution sensor
*/

char PMS_Data[32]; //Global declaration

//Functions to enable USART ports
void EN_USART1(int baud){
	//Enabling USART1 and port A
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
	
	//Setting TX pin for USART1 - 1011
	GPIOA -> CRH |= (1<<5);
	GPIOA -> CRH |= (1<<4);
	GPIOA -> CRH |= (1<<7);
	GPIOA -> CRH &= ~(1<<6);
	//RX default - no need to change
	
	//Setting baud rate for USART1
	USART1 -> BRR = 72000000 / baud;
	
	//Enable RX, Enable TX, and USART bits in CR1 register for USART1
	USART1->CR1 |= (1<<13);
	USART1->CR1 |= (1<<3);
	USART1->CR1 |= (1<<2);
}

void EN_USART2(int baud){
	//Enabling port A and USART 2
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	
	//Setting TX pin for USART2 - 1011
	GPIOA->CRL |= GPIO_CRL_MODE2;
	GPIOA->CRL &=~(1<<10); 
	GPIOA->CRL |=(1<<11);
	//RX pin floating input - default no need to do anyting
	
	//Setting baud rate for USART2
	USART2->BRR = 36000000/baud;
	
	//Enable RX, Enable TX, and USAT bits in CR1 register
	USART2->CR1 |= (1<<13);
	USART2->CR1 |= (1<<3);
	USART2->CR1 |= (1<<2);
}


bool checkBits(){
	//Function that verifies received data
	int zeroTo29 = 0;
	int checkSens = (PMS_Data[30] << 8) + PMS_Data[31];
	//Bitshifted high bits to their location and added the low bits
	for(int ct = 0; ct < 30; ct++){
		zeroTo29 += PMS_Data[ct];
	}
	//If received data is verified, true, if not, false
	if(zeroTo29 == checkSens)
		return true;
	else return false;
}


int main(){
	int i = 0; //	Counter
	int data3 = 0;
	
	EN_USART1(9600); // Enabling USART1 at baudrate 9600
	EN_USART2(9600); // Enabling USART2 at baudrate 9600
	
	while(1){
		//Receive data on USART2 - Sensor
		if(USART2->SR & USART_SR_RXNE){
			PMS_Data[i] = USART2->DR;
			while(!(USART2->SR & USART_SR_TC));
			i++;
		}
		//sendSerial("Zaga: \n");
		if(i==32){
			i = 0;
			/*DEBUG
			if(PMS_Data[0] == 0x42)
				sendSerial("B\n");
			if(PMS_Data[1] == 0x4d)
				sendSerial("M\n");
			if(checkBits())
				sendSerial("tru");
			*/

			if(PMS_Data[0] == 'B' && PMS_Data[1] == 'M' && checkBits()){
				data3 = (PMS_Data[8] << 8) + PMS_Data[9];
				sendSerial("%i", data3);
				/*
				PM10 particles used found from the data
				sent to pc using a function
				*/
			}
		}
	}
}
