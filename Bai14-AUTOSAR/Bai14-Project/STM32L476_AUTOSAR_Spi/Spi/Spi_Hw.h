/**
 * @file        Spi_Hw.h
 * @author      Phuc
 * @brief       SPI Configuration setup for STM32L476
 * @version     1.0
 * @date        2025-01-05
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#ifndef SPI_HW_H
#define SPI_HW_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_gpio.h"
#include "Spi.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
#define SPI_BaudRatePrescaler_2     LL_SPI_BAUDRATEPRESCALER_DIV2
#define SPI_BaudRatePrescaler_4     LL_SPI_BAUDRATEPRESCALER_DIV4
#define SPI_BaudRatePrescaler_8     LL_SPI_BAUDRATEPRESCALER_DIV8
#define SPI_BaudRatePrescaler_16    LL_SPI_BAUDRATEPRESCALER_DIV16
#define SPI_BaudRatePrescaler_32    LL_SPI_BAUDRATEPRESCALER_DIV32
#define SPI_BaudRatePrescaler_64    LL_SPI_BAUDRATEPRESCALER_DIV64
#define SPI_BaudRatePrescaler_128   LL_SPI_BAUDRATEPRESCALER_DIV128
#define SPI_BaudRatePrescaler_256   LL_SPI_BAUDRATEPRESCALER_DIV256

/*
 ************************************************************************************************************
 * Inline functions
 ************************************************************************************************************
 */
/**
 * @brief       Initialize SPI1 and GPIO used in SPI1
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Init_SPI1(const Spi_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL_PTR) {
        return;  
    }
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
    
    /**SPI1 GPIO Configuration
    PA5   ------> SPI1_SCK
    PA6   ------> SPI1_MISO
    PA7   ------> SPI1_MOSI
    PB6   ------> SPI1_CS
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*PB6   ------> SPI1_CS*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI1 parameter configuration*/
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = (ConfigPtr->Mode == SPI_MODE_MASTER) ? LL_SPI_MODE_MASTER : LL_SPI_MODE_SLAVE;
    SPI_InitStruct.DataWidth = (ConfigPtr->DataSize == SPI_DATASIZE_8BIT) ? LL_SPI_DATAWIDTH_8BIT : LL_SPI_DATAWIDTH_16BIT;
    SPI_InitStruct.ClockPolarity = (ConfigPtr->CPOL == SPI_CPOL_LOW) ? LL_SPI_POLARITY_LOW : LL_SPI_POLARITY_HIGH;
    SPI_InitStruct.ClockPhase = (ConfigPtr->CPHA == SPI_CPHA_1EDGE) ? LL_SPI_PHASE_1EDGE : LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS = (ConfigPtr->NSS == SPI_NSS_SOFT) ? LL_SPI_NSS_SOFT : LL_SPI_NSS_HARD_INPUT;
    SPI_InitStruct.BaudRate = ConfigPtr->BaudRate;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_EnableNSSPulseMgt(SPI1);
}

/**
 * @brief       Initialize SPI2 and GPIO used in SPI2
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Init_SPI2(const Spi_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL_PTR) {
        return;  
    }
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    /**SPI2 GPIO Configuration
    PB13   ------> SPI2_SCK
    PB14   ------> SPI2_MISO
    PB15   ------> SPI2_MOSI
    PB1   ------> SPI2_CS
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*PB1   ------> SPI2_CS*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI2 parameter configuration*/
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = (ConfigPtr->Mode == SPI_MODE_MASTER) ? LL_SPI_MODE_MASTER : LL_SPI_MODE_SLAVE;
    SPI_InitStruct.DataWidth = (ConfigPtr->DataSize == SPI_DATASIZE_8BIT) ? LL_SPI_DATAWIDTH_8BIT : LL_SPI_DATAWIDTH_16BIT;
    SPI_InitStruct.ClockPolarity = (ConfigPtr->CPOL == SPI_CPOL_LOW) ? LL_SPI_POLARITY_LOW : LL_SPI_POLARITY_HIGH;
    SPI_InitStruct.ClockPhase = (ConfigPtr->CPHA == SPI_CPHA_1EDGE) ? LL_SPI_PHASE_1EDGE : LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS = (ConfigPtr->NSS == SPI_NSS_SOFT) ? LL_SPI_NSS_SOFT : LL_SPI_NSS_HARD_INPUT;
    SPI_InitStruct.BaudRate = ConfigPtr->BaudRate;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI2, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_EnableNSSPulseMgt(SPI2);
}

/**
 * @brief       Initialize SPI3 and GPIO used in SPI3
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Init_SPI3(const Spi_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL_PTR) {
        return;  
    }

    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
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

    /*PD2   ------> SPI3_CS*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* SPI3 parameter configuration*/
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = (ConfigPtr->Mode == SPI_MODE_MASTER) ? LL_SPI_MODE_MASTER : LL_SPI_MODE_SLAVE;
    SPI_InitStruct.DataWidth = (ConfigPtr->DataSize == SPI_DATASIZE_8BIT) ? LL_SPI_DATAWIDTH_8BIT : LL_SPI_DATAWIDTH_16BIT;
    SPI_InitStruct.ClockPolarity = (ConfigPtr->CPOL == SPI_CPOL_LOW) ? LL_SPI_POLARITY_LOW : LL_SPI_POLARITY_HIGH;
    SPI_InitStruct.ClockPhase = (ConfigPtr->CPHA == SPI_CPHA_1EDGE) ? LL_SPI_PHASE_1EDGE : LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS = (ConfigPtr->NSS == SPI_NSS_SOFT) ? LL_SPI_NSS_SOFT : LL_SPI_NSS_HARD_INPUT;
    SPI_InitStruct.BaudRate = ConfigPtr->BaudRate;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI3, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_EnableNSSPulseMgt(SPI3);
}

/** 
 * @brief       Sets up default configuration for SPI.
 * @details     This function initializes the SPI configuration parameters to default values
 *              if they are set to zero. Default settings are provided for baud rate, 
 *              clock polarity, clock phase, mode, NSS, and data size.
 * @param       config Pointer to the SPI configuration structure to be initialized.
 * @return      void
 */
static inline void Spi_SetupDefaultConfig(Spi_ConfigType* config) {
    if (config->BaudRate == 0) 
    {
        config->BaudRate = SPI_BaudRatePrescaler_64;  
    }

    if (config->CPOL == 0) 
    {
        config->CPOL = SPI_CPOL_LOW;
    }

    if (config->CPHA == 0) 
    {
        config->CPHA = SPI_CPHA_1EDGE;
    }

    if (config->Mode == 0) 
    {
        config->Mode = SPI_MODE_MASTER;
    }

    if (config->NSS == 0) 
    {
        config->NSS = SPI_NSS_SOFT;
    }

    if (config->DataSize == 0) 
    {
        config->DataSize = SPI_DATASIZE_8BIT;
    }
}

/**
 * @brief       Enables the SPI1 peripheral for communication.
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Enable_SPI1(void) 
{
    LL_SPI_Enable(SPI1);  
}

/**
 * @brief       Enables the SPI2 peripheral for communication.
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Enable_SPI2(void) 
{
    LL_SPI_Enable(SPI2);  
}

/**
 * @brief       Enables the SPI3 peripheral for communication.
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Enable_SPI3(void) 
{
    LL_SPI_Enable(SPI3);  
}


/**
 * @brief       De-initialize SPI1
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI1(void)
{
    LL_SPI_DeInit(SPI1);    /* Turn off SPI1 */
    LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);     /* Turn off clock for SPI1 */
}

/**
 * @brief       De-initialize SPI2
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI2(void)
{
    LL_SPI_DeInit(SPI2);    /* Turn off SPI2 */
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI2);     /* Turn off clock for SPI2 */
}

/**
 * @brief       De-initialize SPI3
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI3(void)
{
    LL_SPI_DeInit(SPI3);    /* Turn off SPI3 */
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI3);     /* Turn off clock for SPI3 */
}

/**
 * @brief       Wait until TX buffer in SPI1 is empty
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI1(void)
{
    /* Wait until TX buffer empty (TXE flag is set) */
    while (!LL_SPI_IsActiveFlag_TXE(SPI1));
}

/**
 * @brief       Send data through SPI1
 * @param       DataBufferPtr: Pointer to source data buffer.
 * @return      void
 */
static inline void Spi_SendData_SPI1 (const Spi_DataBufferType* DataBufferPtr)
{
    LL_SPI_TransmitData8(SPI1, *DataBufferPtr);
}

/**
 * @brief       Wait until TX buffer in SPI2 is empty
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI2(void)
{
    /* Wait until TX buffer empty (TXE flag is set) */
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
}

/**
 * @brief       Send data through SPI2
 * @param       DataBufferPtr: Pointer to source data buffer.
 * @return      void
 */
static inline void Spi_SendData_SPI2 (const Spi_DataBufferType* DataBufferPtr)
{
    LL_SPI_TransmitData8(SPI2, *DataBufferPtr);
}

/**
 * @brief       Wait until TX buffer in SPI3 is empty
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI3(void)
{
    /* Wait until TX buffer empty (TXE flag is set) */
    while (!LL_SPI_IsActiveFlag_TXE(SPI3));
}

/**
 * @brief       Send data through SPI3
 * @param       DataBufferPtr: Pointer to source data buffer.
 * @return      void
 */
static inline void Spi_SendData_SPI3 (const Spi_DataBufferType* DataBufferPtr)
{
    LL_SPI_TransmitData8(SPI3, *DataBufferPtr);
}

/**
 * @brief       Wait until data in SPI1 is available to read
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitReceiveBufferFull_SPI1 (void)
{
    // Wait until data is available to read (RXNE flag is set)
    while (!LL_SPI_IsActiveFlag_RXNE(SPI1));
}

/**
 * @brief       Read data from SPI1
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_DataBufferType Spi_ReceiveData_SPI1(void)
{
    /* Read data from SPI1 */
    return LL_SPI_ReceiveData8(SPI1);
}

/**
 * @brief       Wait until data in SPI2 is available to read
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitReceiveBufferFull_SPI2 (void)
{
    // Wait until data is available to read (RXNE flag is set)
    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
}

/**
 * @brief       Read data from SPI2
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_DataBufferType Spi_ReceiveData_SPI2(void)
{
    /* Read data from SPI2 */
    return LL_SPI_ReceiveData8(SPI2);
}

/**
 * @brief       Wait until data in SPI3 is available to read
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_WaitReceiveBufferFull_SPI3 (void)
{
    // Wait until data is available to read (RXNE flag is set)
    while (!LL_SPI_IsActiveFlag_RXNE(SPI3));
}

/**
 * @brief       Read data from SPI3
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_DataBufferType Spi_ReceiveData_SPI3(void)
{
    /* Read data from SPI3 */
    return LL_SPI_ReceiveData8(SPI3);
}

/**
 * @brief       Check current status of SPI1
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI1(void) 
{
    if (LL_SPI_IsActiveFlag_TXE(SPI1) == RESET || LL_SPI_IsActiveFlag_RXNE(SPI1) == RESET) 
    {
        return SPI_BUSY;  
    }
    return SPI_IDLE;  
}

/**
 * @brief       Check current status of SPI2
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI2(void) 
{
    if (LL_SPI_IsActiveFlag_TXE(SPI2) == RESET || LL_SPI_IsActiveFlag_RXNE(SPI2) == RESET) 
    {
        return SPI_BUSY;  
    }
    return SPI_IDLE;  
}

/**
 * @brief       Check current status of SPI3
 * @param       void
 * @return      Spi_DataBufferType
 */
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI3(void) 
{
    if (LL_SPI_IsActiveFlag_TXE(SPI3) == RESET || LL_SPI_IsActiveFlag_RXNE(SPI3) == RESET) 
    {
        return SPI_BUSY;  
    }
    return SPI_IDLE;  
}

/**
 * @brief       Check current status of job on SPI1
 * @param       void
 * @return      Spi_JobResultType
 */
static inline Spi_JobResultType Spi_Hw_CheckJobStatus_SPI1(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI1)== SET) 
    {
        return SPI_JOB_PENDING;
    }
    return SPI_JOB_OK;
}

/**
 * @brief       Check current status of job on SPI2
 * @param       void
 * @return      Spi_JobResultType
 */
static inline Spi_JobResultType Spi_Hw_CheckJobStatus_SPI2(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI2)== SET) 
    {
        return SPI_JOB_PENDING;
    }
    return SPI_JOB_OK;
}

/**
 * @brief       Check current status of job on SPI3
 * @param       void
 * @return      Spi_JobResultType
 */
static inline Spi_JobResultType Spi_Hw_CheckJobStatus_SPI3(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI3)== SET) 
    {
        return SPI_JOB_PENDING;
    }
    return SPI_JOB_OK;
}

/**
 * @brief       Check current status of sequence on SPI1
 * @param       void
 * @return      Spi_SeqResultType
 */
static inline Spi_SeqResultType Spi_Hw_CheckSequenceStatus_SPI1(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI1)== SET) 
    {
        return SPI_SEQ_PENDING;
    }
    return SPI_SEQ_OK;
}

/**
 * @brief       Check current status of sequence on SPI2
 * @param       void
 * @return      Spi_SeqResultType
 */
static inline Spi_SeqResultType Spi_Hw_CheckSequenceStatus_SPI2(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI2)== SET) 
    {
        return SPI_SEQ_PENDING;
    }
    return SPI_SEQ_OK;
}

/**
 * @brief       Check current status of sequence on SPI3
 * @param       void
 * @return      Spi_SeqResultType
 */
static inline Spi_SeqResultType Spi_Hw_CheckSequenceStatus_SPI3(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI3)== SET) 
    {
        return SPI_SEQ_PENDING;
    }
    return SPI_SEQ_OK;
}

/**
 * @brief       Check current hardware status on SPI1
 * @param       void
 * @return      Spi_StatusType
 */
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI1(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI1)== SET) 
    {
        return SPI_BUSY;
    }
    return SPI_IDLE;
}

/**
 * @brief       Check current hardware status on SPI2
 * @param       void
 * @return      Spi_StatusType
 */
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI2(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI2)== SET) 
    {
        return SPI_BUSY;
    }
    return SPI_IDLE;
}

/**
 * @brief       Check current hardware status on SPI3
 * @param       void
 * @return      Spi_StatusType
 */
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI3(void) 
{
    if (LL_SPI_IsActiveFlag_BSY(SPI3)== SET) 
    {
        return SPI_BUSY;
    }
    return SPI_IDLE;
}

/**
 * @brief       Cancel ongoing communication on SPI1
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Cancel_SPI1(void) 
{
    LL_SPI_Disable(SPI1);
    LL_SPI_Enable(SPI1);
}

/**
 * @brief       Cancel ongoing communication on SPI2
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Cancel_SPI2(void) 
{
    LL_SPI_Disable(SPI2);
    LL_SPI_Enable(SPI2);
}

/**
 * @brief       Cancel ongoing communication on SPI3
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Cancel_SPI3(void) 
{
    LL_SPI_Disable(SPI3);
    LL_SPI_Enable(SPI3);
}

/**
 * @brief       Set SPI to polling mode
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_SetPollingMode (void)
{
    NVIC_DisableIRQ(SPI1_IRQn);

    NVIC_DisableIRQ(SPI2_IRQn);

    NVIC_DisableIRQ(SPI3_IRQn);
}

/**
 * @brief       Set SPI to interrupt mode
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_SetInterruptMode (void)
{
    NVIC_SetPriority(SPI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(SPI1_IRQn);

    NVIC_SetPriority(SPI2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(SPI2_IRQn);

    NVIC_SetPriority(SPI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(SPI3_IRQn);
}

/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */


#endif /* SPI_HW_H */
