/**
 * @file        Spi_Cfg.h
 * @author      Phuc
 * @brief       Configuration of SPI
 * @version     1.0
 * @date        2025-01-04
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#ifndef SPI_CFG_H
#define SPI_CFG_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include <Spi.h>

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */

/**
 * @brief       Definition of SPI Channel
 */
#define SPI_CHANNEL_1   0u          /* SPI Channel 1 - Connect with ESP32 */
#define SPI_CHANNEL_2   1u          /* SPI Channel 2 - Can be connected to other device */
#define SPI_CHANNEL_3   2u          /* SPI Channel 3 - Can be connected to other device */

/**
 * @brief       Definition of SPI Job
 */
#define SPI_JOB_SEND_DATA_TO_ESP32_SPI1          0u      /* Job to send data to ESP32 through SPI1 */
#define SPI_JOB_RECEIVE_DATA_FROM_ESP32_SPI1     1u      /* Job to receive data from ESP32 through SPI1 */
#define SPI_JOB_SEND_DATA_TO_ESP32_SPI2          2u      /* Job to send data to ESP32 through SPI2 */
#define SPI_JOB_RECEIVE_DATA_FROM_ESP32_SPI2     3u      /* Job to receive data from ESP32 through SPI2 */
#define SPI_JOB_SEND_DATA_TO_ESP32_SPI3          4u      /* Job to send data to ESP32 through SPI3 */
#define SPI_JOB_RECEIVE_DATA_FROM_ESP32_SPI3     5u      /* Job to receive data from ESP32 through SPI3 */

/**
 * @brief       Definition of SPI Sequence
 */
#define SPI_SEQUENCE_SEND_TO_ESP32          0u      /* Sequence 0: Send data to ESP32 */
#define SPI_SEQUENCE_RECEIVE_FROM_ESP32     1u      /* Sequence 1: Receive data from ESP32 */

/**
 * @typedef     Spi_JobConfigType
 * @brief       SPI configuration for each job
 * @details     SPI configuration for each job such as Channel, baudrate, CPOL, CPHA, Mode
 */
typedef struct 
{
    Spi_ChannelType Channel;    /* SPI channel used for the particular Job */
    uint32 baudrate;            /* Baurate for the particular Job */
    uint8 CPOL;                 /* Clock polarity for the particular Job (0 - low, 1 - high) */
    uint8 CPHA;                 /* Clock phase for the particular Job (0 - 1 edge, 1 - 2 edge) */
    uint8 mode;                 /* Mode Master or Slave (0 - slave, 1 - master) */
} Spi_JobConfigType;

/**
 * @brief       SPI configuration for job sending data to ESP32
 * @details     SPI configuration for job sending data to ESP32 such as Channel, baudrate, CPOL, CPHA, Mode
 */
const Spi_JobConfigType SpiJobConfig_SendDataToESP32 = 
{
    .Channel = SPI_CHANNEL_1,   /* Using SPI1 */
    .Baudrate = 1000000,        /* Baudrate of 1MHz */
    .CPOL = 0,                  /* CPOL = 0 (low clock polarity) */
    .CPHA = 0,                  /* CPHA = 0 (1 edge) */
    .Mode = 1                   /* Master mode */
}

/**
 * @brief       SPI configuration for job receiving data to ESP32
 * @details     SPI configuration for job receiving data to ESP32 such as Channel, baudrate, CPOL, CPHA, Mode
 */
const Spi_JobConfigType SpiJobConfig_SendDataToESP32 = 
{
    .Channel = SPI_CHANNEL_1,   /* Using SPI1 */
    .Baudrate = 1000000,        /* Baudrate of 1MHz */
    .CPOL = 0,                  /* CPOL = 0 (low clock polarity) */
    .CPHA = 0,                  /* CPHA = 0 (1 edge) */
    .Mode = 1                   /* Master mode */
}

/**
 * @typedef     Spi_SequenceConfigType
 * @brief       Configuration of SPI sequence
 * @details     Configuration of SPI sequence of jobs to be excuted in priority order
 */
typedef struct
{
    Spi_JobType Jobs[2];        /* List of jobs in sequence */
    uint8 JobCount;             /* Number of jobs in sequence */
} Spi_SequenceConfigType;

/**
 * @brief       Configuration of SPI sequence sending data to ESP32
 */
const Spi_SequenceConfigType SpiSequenceConfig_0 =
{
    .Jobs = {SPI_JOB_SEND_DATA_TO_ESP32},   /* Only contains Job send data to ESP32 */
    .JobCount = 1
}

/**
 * @brief       Configuration of SPI sequence receiving data to ESP32
 */
const Spi_SequenceConfigType SpiSequenceConfig_1 =
{
    .Jobs = {SPI_JOB_RECEIVE_DATA_TO_ESP32},   /* Only contains Job receive data to ESP32 */
    .JobCount = 1
}

#endif /* SPI_CFG_H */
