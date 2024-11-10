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
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_TX_PIN        LL_GPIO_PIN_0
#define UART_RX_PIN        LL_GPIO_PIN_1
#define UART_GPIO_PORT     GPIOA

#define BAUD_RATE_DELAY    104 // Delay cho baud 9600 ls khoang 104 us
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t receivedChar;
//__IO uint8_t sendchar[] = {'h', 'e', 'l', 'l', 'o'};
__IO uint8_t sendchar[] = {1, 2, 3, 4, 5};
__IO uint16_t test[9] = {0};
__IO uint16_t testParity = 0;
char *str = "STM32ToESP32\n";
char receivedStr[15];
typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVEN
}Parity_Mode;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t delay);
void UARTSoftware_Init(void);
uint8_t UART_ReceiveChar(void);
void UART_SendChar(uint8_t ch);
void UART_SendString(char *str);
void UART_ReceiveString(char *buffer, uint8_t maxLength);
uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode);
uint8_t Parity_Check(uint8_t data, uint8_t receivedParityBit, Parity_Mode mode) ;
void UART_SendChar_WithParity(uint8_t ch, Parity_Mode parityMode);
void UART_SendString_WithParity(char *str, Parity_Mode parityMode);
uint8_t UART_ReceiveChar_WithParity(Parity_Mode parityMode);
void UART_ReceiveString_WithParity(char *buffer, uint8_t maxLength, Parity_Mode parityMode);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	UARTSoftware_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		/*
		UART_SendString(str);
		UART_ReceiveString(receivedStr, 14);
		LL_mDelay(2000);
		memset(receivedStr, 0, sizeof(receivedStr));
		LL_mDelay(2000);
		*/
		//UART_SendChar_WithParity(69, Parity_Mode_EVEN);
		UART_SendString_WithParity(str, Parity_Mode_EVEN);
		UART_ReceiveString_WithParity(receivedStr, 15, Parity_Mode_EVEN);
		LL_mDelay(2000);
		memset(receivedStr, 0, sizeof(receivedStr));
		LL_mDelay(2000);
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
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
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(80000000);

  LL_SetSystemCoreClock(80000000);
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
  TIM_InitStruct.Prescaler = 79;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF-1;
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
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
void delay_us(uint32_t timeDelay)
{
    for (uint32_t i = 0; i < timeDelay; i++)
    {
        LL_TIM_SetCounter(TIM2, 0);
        while (!LL_TIM_IsActiveFlag_UPDATE(TIM2));
        LL_TIM_ClearFlag_UPDATE(TIM2);
    }
}*/
void delay_us(uint32_t timeDelay)
{
    // Ð?t l?i giá tr? d?m v? 0
    LL_TIM_SetCounter(TIM2, 0);

    // Ð?i cho d?n khi giá tr? d?m d?t du?c delay (tính b?ng microsecond)
    while (LL_TIM_GetCounter(TIM2) < timeDelay);
}
void UARTSoftware_Init(void)
{
	LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN);
	delay_us(1);
}


void UART_SendChar(uint8_t ch)
{
    // G?i Start Bit (th?p)
    LL_GPIO_ResetOutputPin(UART_GPIO_PORT, UART_TX_PIN);
    delay_us(BAUD_RATE_DELAY);
		//test = ch;
    // G?i t?ng bit (LSB tru?c)
    for (uint8_t i = 0; i < 8; i++)
    {
        if (ch & (1 << i))
        {
            LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN); // Bit 1
        }
        else
        {
            LL_GPIO_ResetOutputPin(UART_GPIO_PORT, UART_TX_PIN); // Bit 0
        }
        delay_us(BAUD_RATE_DELAY);
    }

    // G?i Stop Bit (cao)
    LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN);
    delay_us(BAUD_RATE_DELAY);
}

uint8_t UART_ReceiveChar(void)
{
    char receivedChar = 0;

    // Ch? Start Bit
    while (LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN));

    // Delay d? v?o gi?a Start Bit
    delay_us(BAUD_RATE_DELAY + BAUD_RATE_DELAY/ 2);

    // ??c t?ng bit (LSB tru?c)
    for (uint8_t i = 0; i < 8; i++)
    {
        if (LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN))
        {
            receivedChar |= (1 << i); // Set bit n?u RX ? m?c cao
        }
				delay_us(BAUD_RATE_DELAY); // Delay gi?a c?c bit
    }

    // Ch? Stop Bit
    delay_us(BAUD_RATE_DELAY/2);
		if (!LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN))
    {
        // N?u Stop Bit không ? m?c cao, có th? d? li?u b? l?i
        return 0; // Ho?c x? lý l?i tùy ch?n
    }
    return receivedChar;
}

void UART_SendString(char *str) 
{
    while (*str) {
        UART_SendChar(*str++);
    }
}


void UART_ReceiveString(char *buffer, uint8_t maxLength) 
{
    uint8_t index = 0;
    char receivedChar;

    // Nhan tung bit cho toi khi nhan duoc ky tu '\n' hoac dat den chieu dai toi da
    do {
        receivedChar = UART_ReceiveChar();
        buffer[index++] = receivedChar;
    } while (receivedChar != '\n' && index < maxLength - 1);

    buffer[index] = '\0'; // Ket thuc chuoi nhan duoc bang ky tu NULL
}

uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode)
{
	uint8_t count = 0;
	uint8_t data1 = data;
	for(int i = 0; i < 8; i++)
	{
		if(data1 & 0x01)
		{
			count++;
		}
		data1 >>= 1;
	}
	switch(Mode)
	{
		case Parity_Mode_NONE:
			return data; 
			break;
		case Parity_Mode_ODD:
			if((count % 2)==0)
			{
				return ((data << 1) | 1);
			} 
			else 
			{
				return (data<<1);
			}
			break;
		case Parity_Mode_EVEN:
			if((count % 2)!=0)
			{
				return ((data << 1) | 1);
			} else {
				return (data << 1);
			}
			break;
		default:
			return data;
			break;
	}
}

uint8_t Parity_Check(uint8_t data, uint8_t receivedParityBit, Parity_Mode mode) 
{
    uint8_t count = 0;
    
    for (uint8_t i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            count++;
        }
    }

    count += receivedParityBit;

    // Ki?m tra theo ch? d? parity
    switch (mode) {
        case Parity_Mode_NONE:
            return 1; 
        
        case Parity_Mode_ODD:
            if (count % 2 == 0) {
                return 0;  
            }
            return 1; 

        case Parity_Mode_EVEN:
            if (count % 2 != 0) {
                return 0;  
            }
            return 1;  

        default:
            return 0; 
    }
}

void UART_SendChar_WithParity(uint8_t ch, Parity_Mode parityMode) 
{
    uint16_t dataWithParity = Parity_Generate(ch, parityMode);
    
    LL_GPIO_ResetOutputPin(UART_GPIO_PORT, UART_TX_PIN);
    delay_us(BAUD_RATE_DELAY);

    
    for (uint8_t i = 0; i < 8; i++) {  
        if ((dataWithParity>>1) & (1 << i)) {
            LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN);  // Bit 1
        } else {
            LL_GPIO_ResetOutputPin(UART_GPIO_PORT, UART_TX_PIN);  // Bit 0
        }
        delay_us(BAUD_RATE_DELAY);
    }

    uint8_t parityBit = dataWithParity & 0x01;
    if (parityBit) {
        LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN);  // Bit parity = 1
    } else {
        LL_GPIO_ResetOutputPin(UART_GPIO_PORT, UART_TX_PIN);  // Bit parity = 0
    }
    delay_us(BAUD_RATE_DELAY);

    // G?i Stop Bit (m?c cao)
    LL_GPIO_SetOutputPin(UART_GPIO_PORT, UART_TX_PIN);
    delay_us(BAUD_RATE_DELAY);
}
void UART_SendString_WithParity(char *str, Parity_Mode parityMode) 
{
    while (*str) 
		{
        UART_SendChar_WithParity(*str++, parityMode);
    }
}

uint8_t UART_ReceiveChar_WithParity(Parity_Mode parityMode) 
{
    uint8_t receivedChar = 0;
    uint8_t parityBit = 0;

    while (LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN));

    delay_us(BAUD_RATE_DELAY + BAUD_RATE_DELAY / 2);

    for (uint8_t i = 0; i < 8; i++) {
        if (LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN))
				{
            receivedChar |= (1 << i); 
        }
        delay_us(BAUD_RATE_DELAY); 
    }

    // Ð?c bit parity
    parityBit = LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN) ? 1 : 0;

    // Ki?m tra Stop Bit
    delay_us(BAUD_RATE_DELAY);
    if (!LL_GPIO_IsInputPinSet(UART_GPIO_PORT, UART_RX_PIN)) 
		{
        return 0;
    }

    // Ki?m tra parity
    if (Parity_Check(receivedChar, parityBit, parityMode) != 1) 
		{
        return 0;
    }

    return receivedChar;
}

void UART_ReceiveString_WithParity(char *buffer, uint8_t maxLength, Parity_Mode parityMode) 
{
    uint8_t index = 0;
    uint8_t receivedChar;

    do {
        receivedChar = UART_ReceiveChar_WithParity(parityMode);
        buffer[index++] = receivedChar;
    } while (receivedChar != '\n' && index < maxLength - 1);  

    buffer[index] = '\0'; 
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
