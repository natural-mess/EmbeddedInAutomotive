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
#include "Spi.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */


/**
 * @brief       Definition of SPI Channel
 */
#define SPI_CHANNEL_MAX  3u         /* There are 3 channels in total */
#define SPI_CHANNEL_1    0u         /* SPI Channel 1 - Connect with ESP32 */
#define SPI_CHANNEL_2    1u         /* SPI Channel 2 - Can be connected to other device */
#define SPI_CHANNEL_3    2u         /* SPI Channel 3 - Can be connected to other device */

/**
 * @brief       Definition of SPI Job
 */
#define SPI_JOB_MAX             6u      /* There are 6 jobs in total */
#define SPI_JOB_0               0u      /* Job 0 ID */
#define SPI_JOB_1               1u      /* Job 1 ID */
#define SPI_JOB_2               2u      /* Job 2 ID */
#define SPI_JOB_3               3u      /* Job 3 ID */
#define SPI_JOB_4               4u      /* Job 4 ID */
#define SPI_JOB_5               5u      /* Job 5 ID */

uint8 transmitDataBuffer_1[1] = {0x10};
uint8 transmitDataBuffer_2[1] = {0x11};
uint8 transmitDataBuffer_3[1] = {0x20};
uint8 transmitDataBuffer_4[1] = {0x21};
uint8 transmitDataBuffer_5[1] = {0x30};
uint8 transmitDataBuffer_6[1] = {0x31};

/**
 * @brief       Definition of SPI Sequence
 */
#define SPI_SEQUENCE_MAX                    3u      /* There are 3 sequences in total */
#define SPI_SEQUENCE_0                      0u      /* Sequence 0 ID */
#define SPI_SEQUENCE_1                      1u      /* Sequence 0 ID */
#define SPI_SEQUENCE_2                      2u      /* Sequence 0 ID */

/**
 * @typedef     Spi_JobConfigType
 * @brief       SPI configuration for each job
 * @details     SPI configuration for each job such as Channel, baudrate, CPOL, CPHA, Mode
 */
typedef struct 
{
    Spi_ChannelType Channel;    /* SPI channel used for the particular Job */
    uint8* DataBuffer;
} Spi_JobConfigType;

/**
 * @brief       SPI configuration for job sending data to ESP32
 * @details     SPI configuration for job sending data to ESP32 such as Channel, baudrate, CPOL, CPHA, Mode
 */
Spi_JobConfigType Spi_Jobs[] = {
    {SPI_CHANNEL_1, transmitDataBuffer_1},       /* Job 0 on Channel 1 with transmit buffer */
    {SPI_CHANNEL_1, transmitDataBuffer_2},       /* Job 1 on Channel 1 with transmit buffer */
    {SPI_CHANNEL_2, transmitDataBuffer_3},       /* Job 2 on Channel 2 with transmit buffer */
    {SPI_CHANNEL_2, transmitDataBuffer_4},       /* Job 3 on Channel 2 with transmit buffer */
    {SPI_CHANNEL_3, transmitDataBuffer_5},       /* Job 4 on Channel 3 with transmit buffer */
    {SPI_CHANNEL_3, transmitDataBuffer_6}        /* Job 5 on Channel 3 with transmit buffer */
};

/**
 * @typedef     Spi_SequenceConfigType
 * @brief       Configuration of SPI sequence
 * @details     Configuration of SPI sequence of jobs to be excuted in priority order
 */
typedef struct
{
    Spi_JobType Jobs[SPI_JOB_MAX];        /* List of jobs in sequence */
    uint8 JobCount;                       /* Number of jobs in sequence */
} Spi_SequenceConfigType;

/**
 * @brief       Configuration of SPI sequence sending data to ESP32
 */
const Spi_SequenceConfigType Spi_Sequences[SPI_SEQUENCE_MAX] = 
{
    {
        .Jobs = {SPI_JOB_0, SPI_JOB_1}, 
        .JobCount = 2           /* Sequence 1 with 2 jobs (transmit) */
    },
    {
        .Jobs = {SPI_JOB_2, SPI_JOB_3}, 
        .JobCount = 2           /* Sequence 2 with 2 jobs (transmit) */
    },
    {
        .Jobs = {SPI_JOB_4, SPI_JOB_5}, 
        .JobCount = 2           /* Sequence 3 with 2 jobs (transmit) */
    }
};

#endif /* SPI_CFG_H */
