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
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WRITE_SDA_0 	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9)
#define WRITE_SDA_1 	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9)
#define WRITE_SCL_0 	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8)
#define WRITE_SCL_1 	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8)
#define READ_SDA_VAL 	LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_9)

#define E_OK 0
#define E_NOT_OK 1

#define ACK 1
#define NACK 0

#define MPU6050_ADDR (0x68<<1)

#define WHO_AM_I     0x75
#define I_AM         0x68
#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C

#define GYRO_XOUT_H  0x43

#define ACCEL_XOUT_H 0x3B
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t iAm = 0;
__IO uint8_t initMPU;
__IO uint8_t configMPU;
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} GyroData;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} AccelData;

typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;
		int16_t temp;
		int16_t gx;
    int16_t gy;
    int16_t gz;
} AllData;


__IO GyroData gyroMPU;
__IO AccelData AccelMPU;
__IO AllData allDataMPU;

__IO float Ax, Ay, Az, Gx, Gy, Gz, temperature;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t timeDelay);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_Master_Init(void);

uint8_t MPU6050_Init (void);
uint8_t MPU6050_Config (void);
GyroData MPU6050_ReadGyro(void);
AccelData MPU6050_ReadAccel(void);
AllData MPU6050_ReadAll(void);
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
	I2C_Master_Init();
	initMPU = MPU6050_Init();
	configMPU = MPU6050_Config();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		//gyroMPU = MPU6050_ReadGyro();
		//AccelMPU = MPU6050_ReadAccel();
		allDataMPU = MPU6050_ReadAll();
		delay_us(1000000);
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
  TIM_InitStruct.Prescaler = 7;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 9;
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

  /**/
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8|LL_GPIO_PIN_9);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
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

void I2C_Master_Init(void)
{
  // C?u h?nh c?c ch?n SDA v? SCL ? tr?ng th?i m?c d?nh
  WRITE_SDA_1;
  WRITE_SCL_1;
}

void I2C_Start(void)
{
	WRITE_SCL_1;  	
	//delay_us(5);	
	WRITE_SDA_1;
	delay_us(5);
	WRITE_SDA_0;	//SDA reset to 0 before SCL.
	delay_us(5);
	WRITE_SCL_0;
	//delay_us(5);
}

void I2C_Stop(void)
{
	WRITE_SDA_0;
	//delay_us(5);
	WRITE_SCL_1; 	//SCL set to 1 before SDA.
	delay_us(5);
	WRITE_SDA_1;
	//delay_us(5);
}

uint8_t I2C_SendByte(uint8_t u8Data)
{
	uint8_t i;
	uint8_t stRet;
	for(int i=0; i< 8; i++)
	{		//Write byte data.
		if (u8Data & 0x80) 
		{
			WRITE_SDA_1;
		} 
		else 
		{
			WRITE_SDA_0;
		}
		delay_us(3);
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(5);
		u8Data <<= 1;
	}
	WRITE_SDA_1;					
	delay_us(3);
	WRITE_SCL_1;					
	delay_us(3);
	
	if (READ_SDA_VAL) 
	{	
		stRet = E_NOT_OK;				
	} 
	else 
	{
		stRet = E_OK;					
	}

	delay_us(2);
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}

uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i;						
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);	
	for (i = 0; i < 8; ++i) 
	{
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(5);
		if (READ_SDA_VAL) 
		{
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
	if (ack) 
	{	
		WRITE_SDA_0;
	} 
	else 
	{
		WRITE_SDA_1;
	}
	delay_us(3);
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}

uint8_t MPU6050_Init (void)
{
	I2C_Start();
	
	if (I2C_SendByte(MPU6050_ADDR) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(WHO_AM_I) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	I2C_Start();
    
  if (I2C_SendByte(MPU6050_ADDR | 0x01) == E_NOT_OK) // G?i d?a ch? v?i bit d?c.
  {
    I2C_Stop();
    return E_NOT_OK;
  }

	
	iAm = I2C_ReadByte(NACK);
	I2C_Stop();
	if (iAm != I_AM)
	{
			
		return E_NOT_OK;
	}
	
	return E_OK;
}

uint8_t MPU6050_Config (void)
{
	I2C_Start();
	if (I2C_SendByte(MPU6050_ADDR) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(PWR_MGMT_1) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(0x00) == E_NOT_OK) 
	{
      I2C_Stop();
      return E_NOT_OK;
  }
	
	if (I2C_SendByte(SMPLRT_DIV) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(0x00) == E_NOT_OK) 
	{
      I2C_Stop();
      return E_NOT_OK;
  }
	
	if (I2C_SendByte(CONFIG) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(0x00) == E_NOT_OK) 
	{
      I2C_Stop();
      return E_NOT_OK;
  }
	
	if (I2C_SendByte(ACCEL_CONFIG) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(0x00) == E_NOT_OK) 
	{ // ±2g
      I2C_Stop();
      return E_NOT_OK; 
	}
	
	if (I2C_SendByte(GYRO_CONFIG) == E_NOT_OK)
	{
		I2C_Stop();
		return E_NOT_OK;
	}
	
	if (I2C_SendByte(0x08) == E_NOT_OK) 
	{ // ±250°/s
      I2C_Stop();
      return E_NOT_OK;
  }
	
	I2C_Stop();
	return E_OK;
}

GyroData MPU6050_ReadGyro(void)
{
    GyroData gyroData = {0};
    
    // Start communication
    I2C_Start();
    
    // Send device address with write flag
    if (I2C_SendByte(MPU6050_ADDR | 0x00) == E_NOT_OK) {
        I2C_Stop();
        return gyroData; // Return zeroed data on error
    }
    
    // Send register address to start from GYRO_XOUT_H
    if (I2C_SendByte(GYRO_XOUT_H) == E_NOT_OK) {
        I2C_Stop();
        return gyroData; // Return zeroed data on error
    }
    
    // Repeat Start to switch to read mode
    I2C_Start();
    
    // Send device address with read flag
    if (I2C_SendByte(MPU6050_ADDR | 0x01) == E_NOT_OK) {
        I2C_Stop();
        return gyroData; // Return zeroed data on error
    }
    
    // Read high and low bytes for each axis
    uint8_t gyroX_H = I2C_ReadByte(ACK);
    uint8_t gyroX_L = I2C_ReadByte(ACK);
    uint8_t gyroY_H = I2C_ReadByte(ACK);
    uint8_t gyroY_L = I2C_ReadByte(ACK);
    uint8_t gyroZ_H = I2C_ReadByte(ACK);
    uint8_t gyroZ_L = I2C_ReadByte(NACK); // Last byte should be NACK

    // Stop communication
    I2C_Stop();
    
    // Combine high and low bytes to form the 16-bit gyro data for each axis
    gyroData.x = (int16_t)((gyroX_H << 8) | gyroX_L);
    gyroData.y = (int16_t)((gyroY_H << 8) | gyroY_L);
    gyroData.z = (int16_t)((gyroZ_H << 8) | gyroZ_L);
    
		Gx = (float)gyroData.x/131.0;
		Gy = (float)gyroData.y/131.0;
		Gz = (float)gyroData.z/131.0;
		
    return gyroData;
}

AccelData MPU6050_ReadAccel(void)
{
    AccelData accelData = {0};
    
    // Start communication
    I2C_Start();
    
    // Send device address with write flag
    if (I2C_SendByte(MPU6050_ADDR | 0x00) == E_NOT_OK) {
        I2C_Stop();
        return accelData; // Return zeroed data on error
    }
    
    // Send register address to start from GYRO_XOUT_H
    if (I2C_SendByte(ACCEL_XOUT_H) == E_NOT_OK) {
        I2C_Stop();
        return accelData; // Return zeroed data on error
    }
    
    // Repeat Start to switch to read mode
    I2C_Start();
    
    // Send device address with read flag
    if (I2C_SendByte(MPU6050_ADDR | 0x01) == E_NOT_OK) {
        I2C_Stop();
        return accelData; // Return zeroed data on error
    }
    
    // Read high and low bytes for each axis
    uint8_t accelX_H = I2C_ReadByte(ACK);
    uint8_t accelX_L = I2C_ReadByte(ACK);
    uint8_t accelY_H = I2C_ReadByte(ACK);
    uint8_t accelY_L = I2C_ReadByte(NACK);
    uint8_t accelZ_H = I2C_ReadByte(ACK);
    uint8_t accelZ_L = I2C_ReadByte(NACK); // Last byte should be NACK

    // Stop communication
    I2C_Stop();
    
    // Combine high and low bytes to form the 16-bit gyro data for each axis
    accelData.x = (int16_t)((accelX_H << 8) | accelX_L);
    accelData.y = (int16_t)((accelY_H << 8) | accelY_L);
    accelData.z = (int16_t)((accelZ_H << 8) | accelZ_L);
		
		Ax = (float)accelData.x/16384.0;
		Ay = (float)accelData.y/16384.0;
		Az = (float)accelData.z/16384.0;
    
    return accelData;
}

AllData MPU6050_ReadAll(void)
{
    AllData allData = {0};
    
    // Start communication
    I2C_Start();
    
    // Send device address with write flag
    if (I2C_SendByte(MPU6050_ADDR | 0x00) == E_NOT_OK) {
        I2C_Stop();
        return allData; // Return zeroed data on error
    }
    
    // Send register address to start from GYRO_XOUT_H
    if (I2C_SendByte(ACCEL_XOUT_H) == E_NOT_OK) {
        I2C_Stop();
        return allData; // Return zeroed data on error
    }
    
    // Repeat Start to switch to read mode
    I2C_Start();
    
    // Send device address with read flag
    if (I2C_SendByte(MPU6050_ADDR | 0x01) == E_NOT_OK) {
        I2C_Stop();
        return allData; // Return zeroed data on error
    }
    
    // Read high and low bytes for each axis
    uint8_t accelX_H = I2C_ReadByte(ACK);
    uint8_t accelX_L = I2C_ReadByte(ACK);
    uint8_t accelY_H = I2C_ReadByte(ACK);
    uint8_t accelY_L = I2C_ReadByte(ACK);
    uint8_t accelZ_H = I2C_ReadByte(ACK);
    uint8_t accelZ_L = I2C_ReadByte(ACK);
		uint8_t temp = I2C_ReadByte(ACK);
		uint8_t gyroX_H = I2C_ReadByte(ACK);
    uint8_t gyroX_L = I2C_ReadByte(ACK);
    uint8_t gyroY_H = I2C_ReadByte(ACK);
    uint8_t gyroY_L = I2C_ReadByte(ACK);
    uint8_t gyroZ_H = I2C_ReadByte(ACK);
    uint8_t gyroZ_L = I2C_ReadByte(NACK); // Last byte should be NACK

    // Stop communication
    I2C_Stop();
    
    // Combine high and low bytes to form the 16-bit gyro data for each axis
    allData.ax = (int16_t)((accelX_H << 8) | accelX_L);
    allData.ay = (int16_t)((accelY_H << 8) | accelY_L);
    allData.az = (int16_t)((accelZ_H << 8) | accelZ_L);
		allData.temp = temp;
		allData.gx = (int16_t)((gyroX_H << 8) | gyroX_L);
    allData.gy = (int16_t)((gyroY_H << 8) | gyroY_L);
    allData.gz = (int16_t)((gyroZ_H << 8) | gyroZ_L);
		
		Ax = (float)allData.ax/16384.0;
		Ay = (float)allData.ay/16384.0;
		Az = (float)allData.az/16384.0;
		temperature = (float)((float)((int16_t)allData.temp) / (float)340.0 + (float)36.53);
		Gx = (float)allData.gx/131.0;
		Gy = (float)allData.gy/131.0;
		Gz = (float)allData.gz/131.0;
    
    return allData;
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
