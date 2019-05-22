#include "stm32f10x.h"                  // Device header

#define Delay_1s() for(int Delay1s = 0; Delay1s <= 4629630; Delay1s++); //Delay of 1s

void Segm_START();				//Initializes everything needed for the operation of the display - To be used in SETUP
void Segm_OFF();					//Turns off currently active segment
void Segm_TEST();					//Tests all the segments and decimal points of every number
void Segm_WRITE(int);			//Prints given number to display
void Segm_DP_ON(int);			//Turns on the DP on chosen segment
void Segm_DP_OFF(int);		//Turns off the DP on chosen segment

