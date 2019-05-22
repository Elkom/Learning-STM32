/**
  ******************************************************************************
	* @name			4 7 segment display in one project
  * @author  	Elmin Softic
  * @date    	14-May-2019 to 22-May-2019
  * @brief   	This was supposed to be a driver for a small board I made that had
  *          	a 4 in 1 7 segment display and resistors.
  *          	It was supposed to be able to pint out float numbers, but 
  *          	some half-way I realised I don't have the knowledge at this time
  *          	to make everything work as intended, so I turned this into
  *          	something I could practice, learn and have fun with while improving
  *						my STM programming capabilities.
	*						I was 17 at the time of writing this.
	*
  ******************************************************************************
  */

#include "stm32f10x.h"                  // Device header
#include "4x7DigDisp.h"									//Driver for 4 7-Segmet display

/*Display is Common Cathode, hence turning off the pin B for turning on the required segment*/

int num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; //Numbers 0 to 9
int DP = 0x80; //Dot Point
/*To use dot point while while writing a number use the bitwise OR (|) in conjuction*/

void segm_N_ON(int n){
	//Activates segment N
	
	if(n == 1){
		//First number from the left
		GPIOB -> BSRR = (1<<16); 	//B0 	- Off
		GPIOB -> BSRR = (1<<1);		//B1 	- On
		GPIOB -> BSRR = (1<<10);	//B10 - On
		GPIOB -> BSRR = (1<<11);	//B11 - On
	}

	if(n == 2){
		//Second number from the left
		GPIOB -> BSRR = (1<<0);		//B0	- On
		GPIOB -> BSRR = (1<<17);	//B1	- Off
		GPIOB -> BSRR = (1<<10);	//B10 - On
		GPIOB -> BSRR = (1<<11);	//B11 - On
	}
	
	if(n == 3){
		//Third number from the left
		GPIOB -> BSRR = (1<<1);		//B0	-	On
		GPIOB -> BSRR = (1<<0);		//B1	-	On
		GPIOB -> BSRR = (1<<26);	//B10	-	Off
		GPIOB -> BSRR = (1<<11);	//B11 - On
	}
	
	if(n == 4){
		//Fourth number from the left
		GPIOB -> BSRR = (1<<1);		//B0	-	On
		GPIOB -> BSRR = (1<<0);		//B1	-	On
		GPIOB -> BSRR = (1<<10);	//B10	-	On
		GPIOB -> BSRR = (1<<27);	//B11	-	Off
	}
}

void Segm_START(){
	//Turning on port A and port B
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
	
	//Setting pins A0 to A7 as digital OUTPUT
	GPIOA -> CRL = GPIO_CRL_MODE;
	
	//Setting push-pull configuration for A0 to A7
	GPIOA -> CRL &= ~GPIO_CRL_CNF;
	
	//Setting pins B0, B1, B10, B11 as digital OUTPUT
	GPIOB -> CRL |= GPIO_CRL_MODE0; //B0 MODE 11
	GPIOB -> CRL &= ~GPIO_CRL_CNF0; //B0 CNF 00
	
	GPIOB -> CRL |= GPIO_CRL_MODE1; //B1 MODE 11
	GPIOB -> CRL &= ~GPIO_CRL_CNF1; //B1 CNF 00
	
	GPIOB -> CRH |= GPIO_CRH_MODE10; //B10 MODE 11
	GPIOB -> CRH &= ~GPIO_CRH_CNF10; //B10 CNF 00
	
	GPIOB -> CRH |= GPIO_CRH_MODE11; //B11 MODE 11
	GPIOB -> CRH &= ~GPIO_CRH_CNF11; //B11 CNF 00
}

void Segm_OFF(){
	//Turns off active segment including Dot Point
	for(int i = 16; i < 24; i++)
		GPIOA -> BSRR = (1<<i);
}

void Segm_DP_ON(int segm){
	//Turns on only the dot point of a chosen segment
	segm_N_ON(segm);
	GPIOA -> BSRR |= DP;
}

void Segm_DP_OFF(int segm){
	//Turns off only the dot point of a chosen segment
	segm_N_ON(segm);
	GPIOA -> BSRR = (1<<23);
}

void del(int dd){
	//Delay of 1/dd seconds, i.e. 1/2 seconds
	for(int de = 0; de <= (4629630/dd); de++);
}

void Segm_TEST(){
	//Quick test of every segment of every place
	int tst[7] = {3, 4, 5, 0, 1, 2, 6}; //Array of segments to be turned on
	//Repeating for all places
	for(int i = 1; i < 5; i++){
		segm_N_ON(i);
		GPIOA -> BSRR = (1<<3);
		del(4);
		GPIOA -> BSRR = (1<<19);
		//Flashing DP 3 times
		for(int k = 0; k < 3; k++){
			Segm_DP_ON(i);
			del(4);
			Segm_DP_OFF(i);
			del(4);
		}
		Segm_DP_ON(i);
		del(4);
		//Turning on the segments
		for(int h = 0; h < 7; h++){
			GPIOA -> BSRR = (1<<tst[h]);
			del(7);
		}
		Delay_1s();
		//Turning off the segments
		for(int h = 6; h >= 0; h--){
			GPIOA -> BSRR = (1<<(tst[h]+16));
			del(7);
		}
		GPIOA -> BSRR = (1<<19);
		del(7);
		//Flashing DP 2 times
		for(int k = 0; k < 2; k++){
			Segm_DP_ON(i);
			del(4);
			Segm_DP_OFF(i);
			del(4);
		}
		Segm_OFF();
	}
}

void Segm_WRITE(int number){
	//Prints given number to display
	int tempCP = number; //Copy of number;
	int countDigits = 0, segment = 1;
	//Counting how many digits are in the whole part
	while(tempCP != 0){
		countDigits++;
		tempCP /= 10;
	}
	tempCP = number;
	//Getting the individual digits
	int nm[4]; //Had to for some error, otherwise it would be nm[countDigits]
	for(int cnt = 0; cnt <= countDigits; cnt++){
		nm[cnt] = tempCP % 10;
		tempCP /= 10;
	}
	//Writing the digits
	for(int times = 0; times <= 50; times++){
			segment = 1;
			//Writing out all digits it can of whole part
			for(int tein = countDigits-1; tein >= 0; tein--){
				segm_N_ON(segment);
				GPIOA -> BSRR = num[nm[tein]];
				for(int k = 0; k < 25000; k++);
				Segm_OFF();
				segment++;
			}
	}
}

