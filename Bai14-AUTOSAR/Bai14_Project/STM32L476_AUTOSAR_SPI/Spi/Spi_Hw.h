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

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */

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
static inline void Spi_Hw_Init_SPI1(void)
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    
    /**SPI1 GPIO Configuration
    PA5   ------> SPI1_SCK
    PA6   ------> SPI1_MISO
    PA7   ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV64;
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
static inline void Spi_Hw_Init_SPI2(void)
{

}

/**
 * @brief       Initialize SPI3 and GPIO used in SPI3
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_Init_SPI3(void)
{

}

/**
 * @brief       De-initialize SPI1
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI1(void)
{
    LL_SPI_DeInit(SPI1);    /* Turn off SPI1 */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);     /* Turn off clock for SPI1 */
}

/**
 * @brief       De-initialize SPI2
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI2(void)
{
    LL_SPI_DeInit(SPI2);    /* Turn off SPI2 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);     /* Turn off clock for SPI2 */
}

/**
 * @brief       De-initialize SPI3
 * @param       void
 * @return      void
 */
static inline void Spi_Hw_DeInit_SPI3(void)
{
    LL_SPI_DeInit(SPI3);    /* Turn off SPI3 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);     /* Turn off clock for SPI3 */
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
    LL_SPI_TransmitData8(SPI1, DataBufferPtr);
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
    LL_SPI_TransmitData8(SPI2, DataBufferPtr);
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
    LL_SPI_TransmitData8(SPI3, DataBufferPtr);
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

/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */


#endif /* SPI_HW_H */
