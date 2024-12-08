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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "rc522.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t status;
uint8_t str[MAX_LEN]; // Max_LEN = 16
uint8_t sNum[5];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */
void read_sdCard(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct Node {
    uint8_t UID[32];
    char bsx[32];
    struct Node* next;
} Node;

// Head of the linked list
Node* head = NULL;

// Function to create a new node and add it to the linked list
void addNode(const uint8_t* UID, const char* bsx) {
    if (UID == NULL || bsx == NULL) {
        myprintf("Invalid input: UID or bsx is NULL\r\n");
        return;
    }
		
		Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        myprintf("Memory allocation failed!\r\n");
        return;
    }
		
		memcpy(newNode->UID, UID, 5); // Copy exactly 5 bytes
    //newNode->UID[5] = '\0';       // Ensure null termination for safe printing
    strncpy(newNode->bsx, bsx, sizeof(newNode->bsx) - 1);
    newNode->bsx[sizeof(newNode->bsx) - 1] = '\0';

    newNode->next = head;
    head = newNode;
}

/*
void freeList() {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    myprintf("Linked list freed.\r\n");
}
*/
bool searchLinkedList(const uint8_t* UID, char* bsx) {
    Node* cur = head;
    while (cur != NULL) {
        if (memcmp(cur->UID, UID, 5) == 0) { // Compare exactly 5 bytes
            strcpy(bsx, cur->bsx);          // Copy the corresponding BSX
            return true;
        }
        cur = cur->next;
    }
    return false;
}

FATFS FatFs; 	//Fatfs handle
FIL fil; 		//File handle
FRESULT fres; //Result after operations

DWORD free_clusters, free_sectors, total_sectors;
FATFS* getFreeFs;

char lineBuffer[128];
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
  HAL_Init();

  /* USER CODE BEGIN Init */
	//freeList();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	MFRC522_Init();
	read_sdCard();
	LL_TIM_EnableCounter(TIM2);
  LL_TIM_EnableIT_UPDATE(TIM2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		delay_ms(1000);
		status = MFRC522_Request(PICC_REQIDL, str);
		if (status != MI_OK)
		{
			continue;
		}
		delay_ms(1);
		status = MFRC522_Anticoll(str);
		if (status != MI_OK)
		{
			continue;
		}
		delay_ms(1);

		memcpy(sNum, str, 5);
		myprintf("SerialNumber: ");
		for (int i = 0; i < 5; i++) 
		{
			myprintf("%d ", sNum[i]); // UID
		}
		myprintf("\n");
		char bsx[32] = {0};
		if (searchLinkedList(sNum, bsx) == true)
		{
			myprintf("Found BSX: %s\n", bsx);
			flag_servo = 1;
			myprintf("Opening barier\n");
		}
		else
		{
			myprintf("UID not found in linked list.\n");
		}
		delay_ms(5000);
		myprintf("Closing barier\n");
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
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
  LL_PWR_EnableBkUpAccess();
  LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnablePLLMode();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 27, LL_RCC_PLLR_DIV_2);
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
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_SetSystemCoreClock(27000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  /**SPI2 GPIO Configuration
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_EnableNSSPulseMgt(SPI2);
  /* USER CODE BEGIN SPI2_Init 2 */
	LL_SPI_Enable(SPI2);
  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  /**SPI3 GPIO Configuration
  PC10   ------> SPI3_SCK
  PC11   ------> SPI3_MISO
  PC12   ------> SPI3_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI3, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_EnableNSSPulseMgt(SPI3);
  /* USER CODE BEGIN SPI3_Init 2 */
	LL_SPI_Enable(SPI3);
  /* USER CODE END SPI3_Init 2 */

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
  TIM_InitStruct.Prescaler = 26;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = 0;
  LL_TIM_Init(TIM1, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM1);
  LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
  LL_TIM_SetTriggerOutput2(TIM1, LL_TIM_TRGO2_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM1);
  /* USER CODE BEGIN TIM1_Init 2 */
	LL_TIM_EnableCounter(TIM1);
  /* USER CODE END TIM1_Init 2 */

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
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* TIM2 interrupt Init */
  NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 26;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 9999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 500;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetOCRefClearInputSource(TIM2, LL_TIM_OCREF_CLR_INT_NC);
  LL_TIM_DisableExternalClock(TIM2);
  LL_TIM_ConfigETR(TIM2, LL_TIM_ETR_POLARITY_NONINVERTED, LL_TIM_ETR_PRESCALER_DIV1, LL_TIM_ETR_FILTER_FDIV1);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */
	LL_TIM_EnableCounter(TIM2);
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
  LL_TIM_GenerateEvent_UPDATE(TIM2);
  /* USER CODE END TIM2_Init 2 */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**TIM2 GPIO Configuration
  PA15 (JTDI)   ------> TIM2_CH1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  LL_USART_InitTypeDef UART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUARTClockSource(LL_RCC_UART4_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**UART4 GPIO Configuration
  PA0   ------> UART4_TX
  PA1   ------> UART4_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  UART_InitStruct.BaudRate = 115200;
  UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  UART_InitStruct.Parity = LL_USART_PARITY_NONE;
  UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART4, &UART_InitStruct);
  LL_USART_ConfigAsyncMode(UART4);
  LL_USART_Enable(UART4);
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

  /**/
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);

  /**/
  LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_2);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void delay_ms(uint32_t ms) {
    // Calculate how many full overflows and remaining ticks are needed
    uint32_t full_overflows = ms / 65; // Number of full 65 ms periods
    uint32_t remaining_ticks = (ms % 65) * 1000; // Remaining ticks in ?s

    // Reset the counter and ensure timer is running
    LL_TIM_SetCounter(TIM1, 0);
    LL_TIM_EnableCounter(TIM1);

    // Handle full overflows
    for (uint32_t i = 0; i < full_overflows; i++) {
        while (LL_TIM_GetCounter(TIM1) < 65535); // Wait for one full overflow
        LL_TIM_SetCounter(TIM1, 0); // Reset counter
    }

    // Handle remaining ticks
    if (remaining_ticks > 0) {
        LL_TIM_SetCounter(TIM1, 0);
        while (LL_TIM_GetCounter(TIM1) < remaining_ticks);
    }

    // Optionally stop the timer (if it doesn't need to run continuously)
    LL_TIM_DisableCounter(TIM1);
}

/* Gui 1 byte du lieu tu Master cho Slave */
void SPI2_TransmitByte(uint8_t data)
{    
    // Doi cho den khi TX buffer trong (TXE flag duoc set)
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
		
    // Gui 1 byte du lieu
    LL_SPI_TransmitData8(SPI2, data);

    // Doi cho den khi byte duoc truyen xong (BSY flag duoc reset)
    while (LL_SPI_IsActiveFlag_BSY(SPI2));
}

uint8_t SPI2_ReceiveByte(void)
{
    uint8_t receivedDataTemp = 0x00u;
		
    // Doi cho den khi co du lieu nhan duoc (RXNE flag duoc set)
    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));

    // Doc du lieu nhan duoc tu SPI
    receivedDataTemp = LL_SPI_ReceiveData8(SPI2);
	
		while (LL_SPI_IsActiveFlag_BSY(SPI2));

		return receivedDataTemp; 
}

void SPI3_TransmitByte(uint8_t data)
{    
    // Doi cho den khi TX buffer trong (TXE flag duoc set)
    while (!LL_SPI_IsActiveFlag_TXE(SPI3));
		
    // Gui 1 byte du lieu
    LL_SPI_TransmitData8(SPI3, data);

    // Doi cho den khi byte duoc truyen xong (BSY flag duoc reset)
    while (LL_SPI_IsActiveFlag_BSY(SPI3));
}

uint8_t SPI3_ReceiveByte(void)
{
    uint8_t receivedDataTemp = 0x00u;
		
    // Doi cho den khi co du lieu nhan duoc (RXNE flag duoc set)
    while (!LL_SPI_IsActiveFlag_RXNE(SPI3));

    // Doc du lieu nhan duoc tu SPI
    receivedDataTemp = LL_SPI_ReceiveData8(SPI3);
	
		while (LL_SPI_IsActiveFlag_BSY(SPI3));
		return receivedDataTemp; 
}

// Send 1 byte through UART
void UART_SendChar(uint8_t ch) 
{
    while (!LL_USART_IsActiveFlag_TXE(UART4));
    LL_USART_TransmitData8(UART4, ch);
    while (!LL_USART_IsActiveFlag_TC(UART4));
}

// Receive 1 byte from UART
uint8_t UART_ReceiveChar(void) 
{
    while (!LL_USART_IsActiveFlag_RXNE(UART4));
    return LL_USART_ReceiveData8(UART4);
}

// Send a string through UART
void UART_SendString(char *str) 
{
    while (*str) {
        UART_SendChar(*str++);
    }
}

// Receive a string from UART (stop when see character '\n')
void UART_ReceiveString(char *buffer, uint8_t maxLength) 
{
    uint8_t index = 0;
    char receivedChar;

    do {
        receivedChar = UART_ReceiveChar();
        buffer[index++] = receivedChar;
    } while (receivedChar != '\n' && index < maxLength - 1);

    buffer[index] = '\0';
}

void myprintf(const char *fmt, ...) 
{
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  //HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
	UART_SendString((char *)buffer);
}

//void read_sdCard(void)
//{
//	myprintf("\r\n~ Reading data from SD Card ~\r\n\r\n");
//	delay_ms(1000);
//	
//	//Open the file system
//  fres = f_mount(&FatFs, "/", 1); //1=mount now
//  if (fres != FR_OK) 
//	{
//		myprintf("f_mount error (%i)\r\n", fres);
//		while(1);
//  }
//	
//  fres = f_getfree("", &free_clusters, &getFreeFs);
//  if (fres != FR_OK) 
//	{
//		myprintf("f_getfree error (%i)\r\n", fres);
//		while(1);
//  }

//  //Formula comes from ChaN's documentation
//  total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
//  free_sectors = free_clusters * getFreeFs->csize;

//  myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);
//	
//  //Now let's try to open file "data.csv"
//  fres = f_open(&fil, "data.csv", FA_READ);
//  if (fres != FR_OK) 
//	{
//		myprintf("f_open error (%i)\r\n", fres);
//		while(1);
//  }
//  myprintf("Opened 'data.csv' successfully!\r\n");

//  //char lineBuffer[128];
//	while (f_gets(lineBuffer, sizeof(lineBuffer), &fil)) 
//	{
//		myprintf("Read line: %s\r\n", lineBuffer);

//		// Parse the line to extract UID and bsx
//		char UID_temp[32] = {0};
//		char bsx_temp[32] = {0};
//    int parsed = sscanf(lineBuffer, "%31[^,],%31s", UID_temp, bsx_temp);
//		if (parsed == 2) 
//		{
//			uint8_t UID[5] = {0};
//			int success = sscanf(UID_temp, "%hhu %hhu %hhu %hhu %hhu", &UID[0], &UID[1], &UID[2], &UID[3], &UID[4]);
//			if (success == 5) {
//            addNode(UID, bsx_temp);  // Thêm vào linked list
//        } else {
//            myprintf("Failed to convert UID: %s\r\n", UID_temp);
//        }
//		} 
//		else 
//		{
//			myprintf("Failed to parse line: %s\r\n", lineBuffer);
//		}
//	}

//  //Close file!
//  f_close(&fil);
//	
//	f_mount(NULL, "/", 0);
//	
//	myprintf("Read SD Card done!\r\n");
//	
//	// Print the linked list contents
//  myprintf("Linked list contents:\r\n");
//	/*
//  Node* current = head;
//  while (current != NULL) 
//	{
//    myprintf("UID: %s, bsx: %s\r\n", current->UID, current->bsx);
//    current = current->next;
//  }
//	*/
//	Node* cur = head;
//	while (cur != NULL) 
//	{
//		myprintf("UID: ");
//		for (int i = 0; i < 5; i++) 
//		{
//				myprintf("%d ", cur->UID[i]);
//		}
//		myprintf(" BSX: %s\n", cur->bsx);
//		cur = cur->next;
//	}
//}

void read_sdCard(void) {
    myprintf("\r\n~ Reading data from SD Card ~\r\n");
    delay_ms(1000);

    // Mount file system
    if (f_mount(&FatFs, "/", 1) != FR_OK) {
        myprintf("Failed to mount file system\r\n");
        return;
    }

    // Open the CSV file
    if (f_open(&fil, "data.csv", FA_READ) != FR_OK) {
        myprintf("Failed to open 'data.csv'\r\n");
        f_mount(NULL, "/", 0);
        return;
    }
    myprintf("Opened 'data.csv' successfully!\r\n");

    // Read and parse lines from the file
    char lineBuffer[128];
    while (f_gets(lineBuffer, sizeof(lineBuffer), &fil)) {
        char UID_temp[32] = {0}, bsx_temp[32] = {0};
        if (sscanf(lineBuffer, "%31[^,],%31s", UID_temp, bsx_temp) == 2) {
            uint8_t UID[5];
            if (sscanf(UID_temp, "%hhu %hhu %hhu %hhu %hhu", &UID[0], &UID[1], &UID[2], &UID[3], &UID[4]) == 5) {
                addNode(UID, bsx_temp);
            } else {
                myprintf("Invalid UID format: %s\r\n", UID_temp);
            }
        } else {
            myprintf("Failed to parse line: %s\r\n", lineBuffer);
        }
    }

    // Close file and unmount file system
    f_close(&fil);
    f_mount(NULL, "/", 0);

    // Print the linked list contents
    myprintf("Linked list contents:\r\n");
    for (Node* cur = head; cur != NULL; cur = cur->next) {
        myprintf("UID: %d %d %d %d %d BSX: %s\r\n", cur->UID[0], cur->UID[1], cur->UID[2], cur->UID[3], cur->UID[4], cur->bsx);
    }

    myprintf("Read SD Card done!\r\n");
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
