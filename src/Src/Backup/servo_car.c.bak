#include "tim.h"

void turnServoCar(int angle){
	TIM3->CCR2 = angle;
	HAL_Delay(1000);
	printf("rotate back");
}

int turnLight(int curAn){
	if(curAn < 0){
		return 0;
	}
	turnServoCar(2200);
	turnServoCar(curAn);

	return 1;

}

