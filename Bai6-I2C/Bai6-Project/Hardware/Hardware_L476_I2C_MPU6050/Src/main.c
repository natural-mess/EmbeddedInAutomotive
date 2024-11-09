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
#define E_OK 0
#define E_NOT_OK 1

#define ACK 1
#define NACK 0

#define MPU6050_ADDR 0x68

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
__IO uint8_t initFlag;
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;
__IO float Ax, Ay, Az, Gx, Gy, Gz;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
uint8_t MPU6050_Init(void);
uint8_t I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
void MPU6050_ReadAccel(int16_t* accelX, int16_t* accelY, int16_t* accelZ);
void MPU6050_ReadGyro(int16_t* gyroX, int16_t* gyroY, int16_t* gyroZ);
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	initFlag = MPU6050_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		MPU6050_ReadAccel(&accelX, &accelY, &accelZ);
    MPU6050_ReadGyro(&gyroX, &gyroY, &gyroZ);
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
  PB8   ------> I2C1_SCL
  PB9   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
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
  TIM_InitStruct.Autoreload = 9;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

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
uint8_t I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    LL_I2C_HandleTransfer(I2C1, devAddr << 1, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1)); 
    LL_I2C_TransmitData8(I2C1, regAddr);

    while(!LL_I2C_IsActiveFlag_TXIS(I2C1));
    LL_I2C_TransmitData8(I2C1, data);

    while(!LL_I2C_IsActiveFlag_STOP(I2C1));
    LL_I2C_ClearFlag_STOP(I2C1);

    return E_OK;
}

uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
    
    LL_I2C_HandleTransfer(I2C1, devAddr << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    while(!LL_I2C_IsActiveFlag_TXIS(I2C1));
    LL_I2C_TransmitData8(I2C1, regAddr);

    LL_I2C_HandleTransfer(I2C1, devAddr << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
    *data = LL_I2C_ReceiveData8(I2C1);

    while(!LL_I2C_IsActiveFlag_STOP(I2C1));
    LL_I2C_ClearFlag_STOP(I2C1);

    return E_OK;
}

uint8_t MPU6050_Init(void)
{
    uint8_t check;
    if (I2C_ReadByte(MPU6050_ADDR, WHO_AM_I, &check) != E_OK || check != I_AM)
    {
        return E_NOT_OK;
    }
    else
    {
        I2C_WriteByte(MPU6050_ADDR, PWR_MGMT_1, 0x00);
			  I2C_WriteByte(MPU6050_ADDR, SMPLRT_DIV, 0x00);
				I2C_WriteByte(MPU6050_ADDR, CONFIG, 0x00);
        I2C_WriteByte(MPU6050_ADDR, ACCEL_CONFIG, 0x00);
        I2C_WriteByte(MPU6050_ADDR, GYRO_CONFIG, 0x00);
    }
		return E_OK;
}

void MPU6050_ReadAccel(int16_t* accelX, int16_t* accelY, int16_t* accelZ)
{
    uint8_t high, low;

    // Ð?c Accel X
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H, &high);
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H + 1, &low);
    *accelX = (int16_t)(high << 8 | low);

    // Ð?c Accel Y
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H + 2, &high);
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H + 3, &low);
    *accelY = (int16_t)(high << 8 | low);

    // Ð?c Accel Z
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H + 4, &high);
    I2C_ReadByte(MPU6050_ADDR, ACCEL_XOUT_H + 5, &low);
    *accelZ = (int16_t)(high << 8 | low);
	
		Ax = (float)(*accelX)/16384.0;
		Ay = (float)(*accelY)/16384.0;
		Az = (float)(*accelZ)/16384.0;
}

void MPU6050_ReadGyro(int16_t* gyroX, int16_t* gyroY, int16_t* gyroZ)
{
    uint8_t high, low;

    // Ð?c Gyro X
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H, &high);
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H + 1, &low);
    *gyroX = (int16_t)(high << 8 | low);

    // Ð?c Gyro Y
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H + 2, &high);
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H + 3, &low);
    *gyroY = (int16_t)(high << 8 | low);

    // Ð?c Gyro Z
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H + 4, &high);
    I2C_ReadByte(MPU6050_ADDR, GYRO_XOUT_H + 5, &low);
    *gyroZ = (int16_t)(high << 8 | low);
	
		Gx = (float)(*gyroX)/131.0;
		Gy = (float)(*gyroY)/131.0;
		Gz = (float)(*gyroZ)/131.0;
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
