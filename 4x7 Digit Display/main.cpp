#include "stm32f10x.h"                  // Device header
#include "4x7DigDisp.h"									//Driver for 4 7-Segmet display

int main(){
	/******SETUP*****/
	Segm_START();
	/******LOOP******/
	while(1){
		Segm_TEST();
	}
}

