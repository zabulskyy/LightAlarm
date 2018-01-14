/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "RTC.h"

#define I2C1_DEVICE_ADDRESS      0x68
uint8_t xBuffer[32];

#define read_sec 		xBuffer[0]&0xF
#define read_dsec 		(xBuffer[0]&(0xF<<4))>>4
#define read_min 		xBuffer[1]&0xF
#define	read_dmin 		(xBuffer[1]&(0xF<<4))>>4
#define read_hrs 		xBuffer[2]&0xF
#define read_dhrs 		(xBuffer[2]&(0xF<<4))>>4
#define read_day	 	xBuffer[3]&0xF

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

uint8_t scldel;
uint8_t sdadel;

void i2c_conf() {
    RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB -> OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6 | ~GPIO_OSPEEDER_OSPEEDR7;
    GPIOB -> MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
    GPIOB -> OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;
    GPIOB -> AFR[0] |= (4 << 24) | (4 << 28);

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN ;

    I2C1->CR1 |= I2C_CR1_ANFOFF;

    scldel = 4;
    sdadel = 5;
    I2C1->TIMINGR = 0xF0001317 | ((scldel & 0x0F) << 20) | ((sdadel & 0x0F) << 16);
    I2C1->CR2 |= (0x1E << 1);
    I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_write_byte(uint8_t addr, uint8_t data) {
    I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
    I2C1->CR2 |= I2C_CR2_START |  (2 << 16);
    while(I2C1->CR2 & I2C_CR2_START);
    I2C1->TXDR = addr;
    while (!(I2C1->ISR & I2C_ISR_TXE));

    I2C1->TXDR = data;
    while (!(I2C1->ISR & I2C_ISR_TXE));
    I2C1->CR2 |= I2C_CR2_STOP;
    while(I2C1->CR2 & I2C_CR2_STOP);
}

uint8_t i2c_read_byte(uint8_t addr) {
    uint8_t data = 0;

    I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
    I2C1->CR2 &= ~(0xff << 16);
    I2C1->CR2 |= I2C_CR2_START | (1 << 16);
    while(I2C1->CR2 & I2C_CR2_START);

    I2C1->TXDR = addr;
    while (!(I2C1->ISR & I2C_ISR_TXE));

    I2C1->CR2 |= I2C_CR2_RD_WRN;
    I2C1->CR2 |= I2C_CR2_START | (1 << 16);
    while(I2C1->CR2 & I2C_CR2_START);
    while (!(I2C1->ISR & I2C_ISR_RXNE));
    data = I2C1->RXDR;
    I2C1->CR2 |= I2C_CR2_STOP;
    while(I2C1->CR2 & I2C_CR2_STOP);
    return data;
}

int ifDisableAlarm(char curr_buf[]){
	char disable_cmd[] = "DISABLE ALARM";
	for (int i = 0; i < 10; i++){
//		char curr_buff_char = curr_buff[i];
//		char curr_disable_cmd = disable_cmd[i];
		if (curr_buf[i] != disable_cmd[i]){
			return 0;
		}
	return 1;
	}
}

int getAlarmTime(char curr_buf[]){
	int hour1 = curr_buf[10] - '0';
	int hour2 = curr_buf[11] - '0';

	int min1 = curr_buf[13] - '0';
	int min2 = curr_buf[14] - '0';

	return 1;

}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  extern void initialise_monitor_handles(void);
  initialise_monitor_handles();
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
   char buf[15];
   int buff_changed = 0;

   /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /*configure correct time*/
  configure_time(1, 4, 0, 2);
  uint8_t zero[] = {0};

  /*setup alarm data*/
  set_alarm(1, 4, 0, 3);

  HAL_I2C_Mem_Read(&hi2c1, 0x68*2, 0, 1, xBuffer, 0x10, 500);
  HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0xF, 1, (zero), 1, 500);	// reload alarm trigger
  while (1)
  {
	  if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)){
	  		  HAL_UART_Receive(&huart2, buf, 15, 1000);
	  		  buff_changed = 1;
	  	  }
	  if (buff_changed){

	  if(ifDisableAlarm(buf) == 1){
			  // DISABLE ALARM
		  	  disable_alarm();
			  // SEND SUCCESS MSG TO USER
			  HAL_UART_Transmit(&huart2, "Alarm DISABLED", 15, 1000);
		  }
	  else {
			  int dhr = buf[10] - '0';
			  int hr = buf[11] - '0';

			  int dmn = buf[13] - '0';
			  int mn = buf[14] - '0';

			  set_alarm(dhr, hr, dmn, mn);
			  HAL_UART_Transmit(&huart2, "Alarm set successfully", 15, 1000);
		  }
		  buff_changed = 0;
	  }

	  HAL_I2C_Mem_Read(&hi2c1, 0x68*2, 0, 1, xBuffer, 0x10, 500);
	  //printf("%i %i%i : %i%i : %i%i\n\n",read_day, read_dhrs, read_hrs, read_dmin, read_min, read_dsec, read_sec);
	  if (!(is_time_to_alarm()) && (read_day != 2)){
		  continue;
	  }	  // read pin which tells us about alarm activation

	  /* RING ALARM */


	    printf("-----\n");
	  	  printf("ALARM ALERT!!!\n");
	  	  printf("-----\n");
	  	  printf("alarm data:\n");
	  	  printf("%i%i : %i%i : %i%i\n", (xBuffer[9]&(0xF<<4))>>4, xBuffer[9]&0xF, (xBuffer[8]&(0xF<<4))>>4, xBuffer[8]&0xF, (xBuffer[7]&(0xF<<4))>>4, xBuffer[7]&0xF);
	  	  printf("-----\n");


	  HAL_I2C_Mem_Write(&hi2c1, 0x68*2, 0xF, 1, (zero), 1, 500);	// reload alarm trigger

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
