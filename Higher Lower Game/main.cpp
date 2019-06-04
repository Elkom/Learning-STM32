#include "stm32f10x.h"                 // Device header
#include "cstdlib"										//Library for srand() and rand()

void segm_N_ON(int n);	//Activates segment N

void Segm_OFF(){
	//Turns off active segment including Dot Point
	for(int i = 16; i < 24; i++)
		GPIOA -> BSRR = (1<<i);
}

int main(){
	/******SETUP*****/
	//Turning on port A and port B
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
	
	//Setting pins A0 to A7 as digital OUTPUT - For every diode of any segment
	GPIOA -> CRL = GPIO_CRL_MODE;
	
	//Setting push-pull configuration for A0 to A7
	GPIOA -> CRL &= ~GPIO_CRL_CNF;
	
	//Setting pins B10, B11 as digital OUTPUT
	GPIOB -> CRH |= GPIO_CRH_MODE10; //B10 MODE 11
	GPIOB -> CRH &= ~GPIO_CRH_CNF10; //B10 CNF 00
	
	GPIOB -> CRH |= GPIO_CRH_MODE11; //B11 MODE 11
	GPIOB -> CRH &= ~GPIO_CRH_CNF11; //B11 CNF 00
	
	//Setting digital INPUT for pins B5 and B4
	//B5 Higher, B4 Lower
	GPIOB->CRL &= ~GPIO_CRL_MODE5_0;
	GPIOB->CRL &= ~GPIO_CRL_CNF5_1;
	
	GPIOB -> CRL &= ~GPIO_CRL_MODE4_0;
	GPIOB -> CRL &= ~GPIO_CRL_CNF4_1;

	std::srand(17); //Initial seed - chose my age at the time of writing
	
	int num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; //Numbers 0 to 9
	
	int broj = 0; //The random number
	broj = std::rand() % 100; //intial random number
	int cifra;
	/******LOOP******/
	while(1){
		broj = std::rand()%100;
		//Checking the input for Higher
		if(GPIOB->IDR & GPIO_IDR_IDR5){
			if(broj > 50)
				for(int times = 0; times < 100; times++){
						Segm_OFF();
						segm_N_ON(3);
						cifra = broj / 10;
						GPIOA -> BSRR = num[cifra];
						for(int k = 0; k < 25000; k++);
						Segm_OFF();
						segm_N_ON(4);
						cifra = broj % 10;
						GPIOA -> BSRR = num[cifra];
						for(int k = 0; k < 25000; k++);
					}
				else{
					for(int times = 0; times < 50; times++){
					segm_N_ON(4);
					Segm_OFF();
					GPIOA -> BSRR = 0x79;
					for(int k = 0; k < 25000; k++);
				}
					std::srand(broj);
					broj = std::rand()%100;
				for(int Delay1s = 0; Delay1s <= 4629630; Delay1s++);
		}
	}
		
	//Checking the input foro Lower
	if(GPIOB->IDR & GPIO_IDR_IDR4){
		if(broj < 50)
			for(int times = 0; times < 100; times++){
					Segm_OFF();
					segm_N_ON(3);
					cifra = broj / 10;
					GPIOA -> BSRR = num[cifra];
					for(int k = 0; k < 25000; k++);
					Segm_OFF();
					segm_N_ON(4);
					cifra = broj % 10;
					GPIOA -> BSRR = num[cifra];
					for(int k = 0; k < 25000; k++);
			}
		else{
			for(int times = 0; times < 50; times++){
			segm_N_ON(4);
			Segm_OFF();
			GPIOA -> BSRR = 0x79;
			for(int k = 0; k < 25000; k++);
		}
			std::srand(broj);
			broj = std::rand()%100;
		for(int Delay1s = 0; Delay1s <= 4629630; Delay1s++);
		}		
	}
	
	//Always outputing 50 - the middle between 0 and 99
		for(int times = 0; times < 50; times++){
			Segm_OFF();
			segm_N_ON(3);
			GPIOA -> BSRR = num[5];
			for(int k = 0; k < 25000; k++);
			Segm_OFF();
			segm_N_ON(4);
			GPIOA -> BSRR = num[0];
			for(int k = 0; k < 25000; k++);
		}
	}
}
void segm_N_ON(int n){
	//Activates segment N
	
	if(n == 3){
		//Third number from the left
		GPIOB -> BSRR = (1<<26);	//B10	-	Off
		GPIOB -> BSRR = (1<<11);	//B11 - On
	}
	
	if(n == 4){
		//Fourth number from the left
		GPIOB -> BSRR = (1<<10);	//B10	-	On
		GPIOB -> BSRR = (1<<27);	//B11	-	Off
	}
}
