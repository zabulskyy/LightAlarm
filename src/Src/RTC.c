#include "main.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "gpio.h"

#define I2C1_DEVICE_ADDRESS      0x68
uint8_t xBuffer[32];

void enable_alarm(){

	uint8_t one[] = {1};
	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 3, 1, one, 1, 500);     // weekday
	return;
}
void set_alarm(int dhr, int hr, int dmn, int mn){

	//enable_alarm();
	uint8_t alarm_seconds[] =   { 0 << 4 		| 0		| 0 << 7 };
	uint8_t alarm_minutes[] =   { dmn << 4 		| mn 	| 0 << 7 };
	uint8_t alarm_hours[] =     { dhr << 4 		| hr 	| 0 << 7 };
	uint8_t alarm_week_day[] =  { 1 << 6 		| 0 	| 1 << 7 };

	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0x7,   1, alarm_seconds,  1, 500);	// seconds
	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0x8,   1, alarm_minutes,  1, 500);	// minutes
	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0x9,   1, alarm_hours,    1, 500);	// hours
	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0xA, 	 1, alarm_week_day, 1, 500);	// weekday
	enable_alarm();
	return;
}

uint8_t is_time_to_alarm(){
	HAL_I2C_Mem_Read(&hi2c1, 0x68*2, 0, 1, xBuffer, 0x10, 500);
	return xBuffer[0xF]&0xF;
}

void disable_alarm(){

	uint8_t two[] = {2};
	HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 3, 1, two, 1, 500);     // weekday
	return;
}

void configure_time(int dh, int h, int dm, int m){

	   uint8_t zero[] = {0};
	   uint8_t seconds[] = {0 << 4 | 0};
	   uint8_t minutes[] = {dm << 4 | m};
	   uint8_t hours[] =   {dh << 4 | h};

	   HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0, 1, seconds, 1, 500);  // seconds
	   HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 1, 1, minutes, 1, 500);  // minutes
	   HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 2, 1, hours, 1, 500);    // hours
}
