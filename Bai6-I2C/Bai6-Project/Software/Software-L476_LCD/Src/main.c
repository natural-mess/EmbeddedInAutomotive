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
#define LCD_ADDRESS 0x3F  // Dia chi I2C cua LCD 
#define LCD_CMD 0x00      // Gui lenh
#define LCD_DATA 0x40     // Gui du lieu
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t readbyte_ack;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t timeDelay);
void i2c_start(void);
void i2c_stop(void);
uint8_t I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);
void LCD_Send(uint8_t data, uint8_t mode);
void LCD_Init(void);
void LCD_SendString(char *str);
void PCD_PutCur(int row, int col);
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
  //LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  //LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  //NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  //NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	PCD_PutCur(0, 0);
	LCD_SendString("Hello, World!");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() != 0)
  {
  }
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(4000000);

  LL_SetSystemCoreClock(4000000);
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 3;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */
	LL_TIM_EnableCounter(TIM2);
  /* USER CODE END TIM2_Init 2 */

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
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /*PB6-SCL, PB7-SDA*/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6|LL_GPIO_PIN_7);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* delay 1us */
void delay_us(uint32_t timeDelay)
{
    for (uint32_t i = 0; i < timeDelay; i++)
    {
        LL_TIM_SetCounter(TIM2, 0);
			  LL_TIM_EnableCounter(TIM2);
        while (!LL_TIM_IsActiveFlag_UPDATE(TIM2));
        LL_TIM_ClearFlag_UPDATE(TIM2);
    }
}
// Bat dau giao tiep I2C
void I2C_Start(void)
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5); //delay 1us
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
    delay_us(5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
}

// Dung giao tiep I2C
void I2C_Stop(void)
{
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    delay_us(5);
}

// Gui mot byte qua I2C
uint8_t I2C_SendByte(uint8_t byte)
{
		uint8_t ack;
    for (int i = 0; i < 8; i++) 
		{
        if ((byte & 0x80) != 0) 
				{
            LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
        } else 
				{
            LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
        }
        delay_us(5);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
        delay_us(5);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
        byte <<= 1; // Shift byte sang trái
    }
    // Nhan ACK tu Slave
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
		if (LL_GPIO_IsOutputPinSet(GPIOB, LL_GPIO_PIN_7) == 0) // Neu SDA = 0, nhan ACK
			ack = 1;
		else
			ack = 0;
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
		return ack;
}

// Ham doc 1 byte tu I2C
uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t receivedByte = 0;

    // Dat SDA o trang thai dau vao
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (trang thái cao)
    
    for (int i = 0; i < 8; i++) 
    {
        receivedByte <<= 1; // Dich sang trai de chuan bi nhan bit tiep theo
        
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
        delay_us(5);
        
        if (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7))  // Kiem tra trang thai SDA
        {
            receivedByte |= 0x01; // Neu SDA = 1, set bit LSB cua receivedByte
        }
        
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
        delay_us(5);
    }
    
    // Gui ACK hoac NACK cho Slave
    if (ack) 
    {
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0 (ACK)
    } 
    else 
    {
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (NACK)
    }
    
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (phuc hoi trang thai cao cho SDA)
    
    return receivedByte;
}

// Gui lenh den LCD
void LCD_Send(uint8_t data, uint8_t mode)
{
    I2C_Start();
    I2C_SendByte(LCD_ADDRESS << 1 | mode); // Gui dia chi LCD
    I2C_SendByte(data); // Gui du lieu
    I2C_Stop();
}

// Khoi tao LCD
void LCD_Init(void)
{
// 4 bit initialisation
  delay_us(50000);  // wait for >40ms
  //LCD_Send (0x30, LCD_CMD);
  //delay_us(5000);  // wait for >4.1ms
  //LCD_Send (0x30, LCD_CMD);
  //delay_us(1000);  // wait for >100us
  //LCD_Send (0x30, LCD_CMD);
  //delay_us(10000);
  //LCD_Send (0x20, LCD_CMD);  // 4bit mode
  //delay_us(10000);

  // display initialisation
  LCD_Send (0x28, LCD_CMD); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
  delay_us(1000);
  LCD_Send (0x08, LCD_CMD); //Display on/off control --> D=0,C=0, B=0  ---> display off
  delay_us(1000);
  LCD_Send (0x01, LCD_CMD);  // clear display
  delay_us(2000);
  LCD_Send (0x06, LCD_CMD); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  delay_us(1000);
  LCD_Send (0x0C, LCD_CMD); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

// Gui chuoi den
void LCD_SendString(char *str)
{
    while (*str) 
		{
        LCD_Send(*str++, LCD_DATA); // Gui tung ki tu
    }
}

void PCD_PutCur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    LCD_Send (col, LCD_CMD);
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
