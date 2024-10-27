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
#define LCD_ADDRESS 0x3F
#define LCD_CMD     0x00
#define LCD_DATA    0x40
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile int col = 0;
volatile int row = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t timeDelay);
void LCD_Send_Command(uint8_t cmd) ;
void LCD_Send_Data(uint8_t data) ;
void LCD_SendString(char *str);
void LCD_Init (void);
void lcd_put_cur(int row, int col);
void LCD_Clear (void);
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
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
  lcd_put_cur(0, 0);
  LCD_SendString("HELLO WORLD!");
//	delay_us(5000000);
//	LCD_Clear();
//	// Display Number
//  //lcd_init();
//  int num = 1234;
//  char numChar[5];
//  //sprintf(numChar, "%d", num);
//  lcd_put_cur(0, 0);
//  LCD_SendString (numChar);
//	delay_us(5000000);
//	LCD_Clear();
//// Display Float
//  //lcd_init();
//  float flt = 12.345;
//  char fltChar[6];
//  //sprintf(fltChar, "%.3f", flt);
//  lcd_put_cur(0, 0);
//  LCD_SendString (fltChar);
//	LCD_Clear();
//	delay_us(5000000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  for (int i=0;i<128;i++)
	  {
			//LCD_Init();
		  lcd_put_cur(row, col);

		  LCD_Send_Data(i+48);

		  col++;

		  if (col > 15) 
			{
				row++; 
				col = 0;
			}
			if (row > 1) 
				row=0;

			delay_us(250000);
	  }
//		for (int i=0;i<128;i++)
//	  {
//			row++;
//			delay_us(1000000);
//		}
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_DisableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x00100D14;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */
	LL_I2C_Enable(I2C1);
  /* USER CODE END I2C1_Init 2 */

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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

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

void LCD_Send_Command(uint8_t cmd) 
{
    //uint8_t data[2];
    //data[0] = cmd; // L?nh
    //data[1] = 0x00; // RS = 0 (l?nh)
	
			//uint16_t command_word = 0;
			//command_word = cmd;  // rs=0
			//uint8_t cmd_t[2];
			//cmd_t[0] = (command_word>>8)&0xFF;
			//cmd_t[1] = (command_word)&0xFF;
		char data_u, data_l;
		uint8_t data_t[4];
		data_u = (cmd&0xf0);
		data_l = ((cmd<<4)&0xf0);
		data_t[0] = data_u|0x0C;  //en=1, rs=0 -> bxxxx1100
		data_t[1] = data_u|0x08;  //en=0, rs=0 -> bxxxx1000
		data_t[2] = data_l|0x0C;  //en=1, rs=0 -> bxxxx1100
		data_t[3] = data_l|0x08;  //en=0, rs=0 -> bxxxx1000
		
		char *pdata = (char *)data_t;
		LL_I2C_HandleTransfer(I2C1, LCD_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, sizeof(data_t), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
		/* Loop until STOP flag is raised  */
		while(!LL_I2C_IsActiveFlag_STOP(I2C1))
		{
    /* (2.1) Transmit data (TXIS flag raised) *********************************/

    /* Check TXIS flag value in ISR register */
			if(LL_I2C_IsActiveFlag_TXIS(I2C1))
			{
				/* Write data in Transmit Data register.
				TXIS flag is cleared by writing data in TXDR register */
				LL_I2C_TransmitData8(I2C1, (*pdata++));
			}
		}

		LL_I2C_ClearFlag_STOP(I2C1);
		
    // G?i l?nh qua I2C
    //LL_I2C_HandleTransfer(I2C1, LCD_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, sizeof(cmd_t), LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    //while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
    //LL_I2C_TransmitData8(I2C1, cmd_t[0]);
    //while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
    //LL_I2C_TransmitData8(I2C1, cmd_t[1]);
    //LL_I2C_GenerateStopCondition(I2C1);
}

//void LCD_Send_Data(uint8_t data) 
//{
//    //uint8_t buffer[2];
//    //buffer[0] = data; // D? li?u
//    //buffer[1] = 0x40; // RS = 1 (d? li?u)
//	
//		char data_u, data_l;
//		uint8_t data_t[4];
//		data_u = (data&0xf0);
//		data_l = ((data<<4)&0xf0);
//		data_t[0] = data_u|0x0D;  //en=1, rs=1 -> bxxxx1101
//		data_t[1] = data_u|0x09;  //en=0, rs=1 -> bxxxx1001
//		data_t[2] = data_l|0x0D;  //en=1, rs=1 -> bxxxx1101
//		data_t[3] = data_l|0x09;  //en=0, rs=1 -> bxxxx1001
//		char *pdata = (char *)data_t;
//		LL_I2C_HandleTransfer(I2C1, LCD_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, sizeof(data_t), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
//		/* Loop until STOP flag is raised  */
//		while(!LL_I2C_IsActiveFlag_STOP(I2C1))
//		{
//    /* (2.1) Transmit data (TXIS flag raised) *********************************/

//    /* Check TXIS flag value in ISR register */
//			if(LL_I2C_IsActiveFlag_TXIS(I2C1))
//			{
//				/* Write data in Transmit Data register.
//				TXIS flag is cleared by writing data in TXDR register */
//				LL_I2C_TransmitData8(I2C1, (*pdata++));
//			}
//		}

//		LL_I2C_ClearFlag_STOP(I2C1);
//    // G?i d? li?u qua I2C
//    //LL_I2C_HandleTransfer(I2C1, LCD_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, sizeof(data_t), LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
//    //while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
//    //LL_I2C_TransmitData8(I2C1, data_t[0]);
//    //while (!LL_I2C_IsActiveFlag_TXIS(I2C1));
//    //LL_I2C_TransmitData8(I2C1, data_t[1]);
//    //LL_I2C_GenerateStopCondition(I2C1);
//}
void LCD_Send_Data(uint8_t data) 
{
    char data_u, data_l;
    uint8_t data_t[4];
    
    // Chia d? li?u thành n?a cao và n?a th?p
    data_u = (data & 0xF0);
    data_l = ((data << 4) & 0xF0);
    
    // Thi?t l?p các bit di?u khi?n
    data_t[0] = data_u | 0x0D;  // EN=1, RS=1
    data_t[1] = data_u | 0x09;  // EN=0, RS=1
    data_t[2] = data_l | 0x0D;  // EN=1, RS=1
    data_t[3] = data_l | 0x09;  // EN=0, RS=1

    LL_I2C_HandleTransfer(I2C1, LCD_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, sizeof(data_t), LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    
    for (int i = 0; i < sizeof(data_t); i++) {
        int timeout = 10000;
        while (!LL_I2C_IsActiveFlag_TXIS(I2C1) && timeout--) {
            // thêm delay n?u c?n
        }
        if (timeout <= 0) {
            Error_Handler();
        }
        LL_I2C_TransmitData8(I2C1, data_t[i]);
    }
    
    while (!LL_I2C_IsActiveFlag_STOP(I2C1));
    LL_I2C_ClearFlag_STOP(I2C1);
}

/* Function to send a string to the LCD */
void LCD_SendString(char *str)
{
    while (*str)
    {
        LCD_Send_Data(*str++); // G?i t?ng ký t?
    }
}

void LCD_Init (void)
{
	// 4 bit initialisation
	delay_us(50000);  // wait for >40ms
	LCD_Send_Command (0x30);
	delay_us(5000);  // wait for >4.1ms
	LCD_Send_Command (0x30);
	delay_us(1000);  // wait for >100us
	LCD_Send_Command (0x30);
	delay_us(10000);
	LCD_Send_Command (0x20);  // 4bit mode
	delay_us(10000);

  // dislay initialisation
	LCD_Send_Command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay_us(1000);
	LCD_Send_Command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	delay_us(1000);
	LCD_Send_Command (0x01);  // clear display
	delay_us(1000);
	LCD_Send_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay_us(1000);
	LCD_Send_Command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_put_cur(int row, int col)
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

    LCD_Send_Command (col);
}

void LCD_Clear (void)
{
	LCD_Send_Command (0x80);
	for (int i=0; i<70; i++)
	{
		LCD_Send_Data (' ');
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
