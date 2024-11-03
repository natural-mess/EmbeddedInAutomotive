/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WRITE_SDA_0     LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9)
#define WRITE_SDA_1     LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9)
#define WRITE_SCL_0     LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8)
#define WRITE_SCL_1     LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8)
#define READ_SDA_VAL    LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_9)
#define READ_SCL_VAL    LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_8)

#define SLAVE_ADDRESS   0x3F
#define E_OK 0
#define E_NOT_OK 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t receivedData[100];
volatile uint8_t receiveCounter = 0;
volatile uint8_t sendData[4] = {1, 2, 3, 4}; // Data to send to master
volatile uint8_t i = 0;
volatile uint8_t started = 0;
volatile uint8_t receivedByte = 0;
volatile uint8_t readSCL = 0;
volatile uint8_t readSDA= 0;
volatile uint32_t readPortBOut = 0;
volatile uint32_t readPortBIn = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void delay_ms(uint32_t timeDelay);
void I2C_Slave_Init(void);
void I2C_Slave_WaitForStart(void);
uint8_t I2C_Slave_ReceiveByte(void);
void I2C_Slave_SendAck(void);
void I2C_Slave_SendData(uint8_t data);
void I2C_Slave_WaitForStop(void);
void I2C_Slave_WaitForClockHigh(void);
void I2C_Slave_WaitForClockLow(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	I2C_Slave_Init();
  /* USER CODE END 2 */
	readSCL = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_8);
	readSDA = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_9);
	readPortBIn = LL_GPIO_ReadInputPort(GPIOB);
	readPortBOut = LL_GPIO_ReadOutputPort(GPIOB);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		i=0;
    I2C_Slave_WaitForStart();
		for (i = 0; i<4; i++)
		{
			receivedData[i++] = I2C_Slave_ReceiveByte();
			I2C_Slave_SendAck();
		}
		//I2C_Slave_WaitForStop();
    //I2C_Slave_SendData(sendData[0]); // G?i byte d?u tiên
    //I2C_Slave_WaitForStop();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  LL_PWR_DisableOverDriveMode();
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  TIM_InitStruct.Prescaler = 7;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 9;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  /* USER CODE BEGIN TIM1_Init 2 */
	LL_TIM_EnableCounter(TIM1);
  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8|LL_GPIO_PIN_9);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void delay_ms(uint32_t timeDelay)
{
    for (uint32_t i = 0; i < timeDelay; i++)
    {
        LL_TIM_SetCounter(TIM1, 0);
        LL_TIM_EnableCounter(TIM1);
        while (!LL_TIM_IsActiveFlag_UPDATE(TIM1));
        LL_TIM_ClearFlag_UPDATE(TIM1);
    }
}

void I2C_Slave_Init(void)
{
  // C?u hình các chân SDA và SCL ? tr?ng thái m?c d?nh
  WRITE_SDA_1;
  WRITE_SCL_1;
}

void I2C_Slave_WaitForClockHigh(void)
{
    
    while (!READ_SCL_VAL) 
    {
    }
}

void I2C_Slave_WaitForClockLow(void)
{
    while (READ_SCL_VAL) 
    {  
    }
}

void I2C_Slave_WaitForStart(void)
{
  // Ch? tín hi?u START t? master: SCL gi? ? m?c cao và SDA chuy?n t? cao xu?ng th?p
  while (READ_SDA_VAL || READ_SCL_VAL)
  {
    // Ch? d?n khi SDA xu?ng th?p trong khi SCL dang cao
  }
	started = 1;
}

uint8_t I2C_Slave_ReceiveByte(void)
{
    receivedByte = 0;
    for (int i = 0; i < 8; i++)
    {
        I2C_Slave_WaitForClockHigh();  // Ch? SCL lên cao
        receivedByte <<= 1;            // D?ch bit sang trái d? chu?n b? nh?n bit ti?p theo

        if (READ_SDA_VAL) // Ki?m tra tr?ng thái SDA
        {
            receivedByte |= 0x01;      // Ghi bit 1 n?u SDA ? m?c cao
        }

        I2C_Slave_WaitForClockLow();
    }

    return receivedByte;
}

void I2C_Slave_SendAck(void)
{
	I2C_Slave_WaitForClockHigh(); 
  // G?i tín hi?u ACK d? xác nh?n dã nh?n d? li?u
  WRITE_SDA_0; // Ðua SDA xu?ng th?p d? g?i ACK
  delay_ms(5);
  WRITE_SDA_1; // Ðua SDA lên cao tr? l?i
}

void I2C_Slave_SendData(uint8_t data)
{
  // G?i m?t byte d? li?u t?i master
  uint8_t i;
  for (i = 0; i < 8; ++i)
  {
    if (data & (1 << (7 - i)))
    {
      WRITE_SDA_1; // Ð?t SDA lên cao n?u bit tuong ?ng là 1
    }
    else
    {
      WRITE_SDA_0; // Ð?t SDA xu?ng th?p n?u bit tuong ?ng là 0
    }
    delay_ms(3);
    WRITE_SCL_1; // Ðua SCL lên cao
    delay_ms(5);
    WRITE_SCL_0; // Ðua SCL xu?ng th?p
  }
}

void I2C_Slave_WaitForStop(void)
{
  // Ch? tín hi?u STOP t? master: SCL gi? ? m?c cao và SDA chuy?n t? th?p lên cao
  while (!(READ_SCL_VAL && READ_SDA_VAL))
  {
    // Ch? d?n khi SCL cao và SDA chuy?n t? th?p lên cao
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
