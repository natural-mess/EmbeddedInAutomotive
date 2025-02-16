/**
 * @file        Spi.h
 * @author      Phuc
 * @brief       SPI driver header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-04
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#ifndef SPI_H
#define SPI_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Std_Types.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */

/**
 * @typedef     Spi_StatusType
 * @brief       This type defines a range of specific status for SPI Handler/Driver.
 * @details     The type Spi_StatusType defines a range of specific status for SPI Handler/Driver.
 *              It informs about the SPI Handler/Driver status or specified SPI Hardware microcontroller peripheral.
 */
typedef enum
{
    SPI_UNINIT = 0x00,              /* The SPI Handler/Driver is not initialized or not usable. */
    SPI_IDLE = 0x01,                /* The SPI Handler/Driver is not currently transmitting any Job. */
    SPI_BUSY = 0x02                 /* The SPI Handler/Driver is performing a SPI Job (transmit). */
} Spi_StatusType;

/**
 * @typedef     Spi_JobResultType
 * @brief       This type defines a range of specific Jobs status for SPI Handler/Driver.
 * @details     The type Spi_JobResultType defines a range of specific Jobs status for SPI Handler/Driver.
 */
typedef enum
{
    SPI_JOB_OK = 0x00,              /* The last transmission of the Job has been finished successfully. */
    SPI_JOB_PENDING = 0x01,         /* The SPI Handler/Driver is performing a SPI Job. The meaning of this status is equal to SPI_BUSY. */
    SPI_JOB_FAILED = 0x02,          /* The last transmission of the Job has failed. */
    SPI_JOB_QUEUED = 0x03           /* An asynchronous transmit Job has been accepted, while actual transmission for this Job has not started yet. */
} Spi_JobResultType;

/**
 * @typedef     Spi_SeqResultType
 * @brief       This type defines a range of specific Sequences status for SPI Handler/Driver.
 * @details     The type Spi_SeqResultType defines a range of specific Sequences status for SPI Handler/Driver 
 *              and can be obtained calling the API service Spi_GetSequenceResult, it shall be provided for external use.
 */
typedef enum
{
    SPI_SEQ_OK = 0x00,              /* The last transmission of the Sequence has been finished successfully. */
    SPI_SEQ_PENDING = 0x01,         /* The SPI Handler/Driver is performing a SPI Sequence. The meaning of this status is equal to SPI_BUSY. */
    SPI_SEQ_FAILED = 0x02,          /* The last transmission of the Sequence has failed. */
    SPI_SEQ_CANCELED = 0x03           /* The last transmission of the Sequence has been canceled by user. */
}  Spi_SeqResultType;

/**
 * @typedef     Spi_AsyncModeType
 * @brief       Specifies the asynchronous mechanism mode for SPI busses handled asynchronously.
 * @details     The asynchronous mechanism is selected by the API Spi_SetAsyncMode.
 */
typedef enum
{
    SPI_POLLING_MODE = 0x00,           /* The asynchronous mechanism is ensured by polling, so interrupts related to SPI busses
                                        * handled asynchronously are disabled. */
    SPI_INTERRUPT_MODE = 0x01         /* The asynchronous mechanism is ensured by interrupt, so interrupts related to SPI busses
                                        * handled asynchronously are enabled. */
}  Spi_AsyncModeType;

/**
 * @typedef     Spi_DataBufferType
 * @brief       Type of application data buffer elements
 * @details     Spi_DataBufferType defines the type of application data buffer elements. Type is uint8.
 *              The data buffer has to be aligned to 32 bits. It shall be provided for external use.
 */
typedef uint8 Spi_DataBufferType;

/**
 * @typedef     Spi_NumberOfDataType
 * @brief       Type for defining the number of data elements to send and / or receive by Channel
 * @details     The type Spi_NumberOfDataType is used for defining the number of data elements of the type to send and / or receive
 *              by Channel.
 */
typedef uint16 Spi_NumberOfDataType;

/**
 * @typedef     Spi_ChannelType
 * @brief       Specifies the identification (ID) for a Channel.
 * @details     The type Spi_ChannelType specifies the identification (ID) for a Channel.
 */
typedef uint8 Spi_ChannelType;

/**
 * @typedef     Spi_JobType
 * @brief       Specifies the identification (ID) for a Job.
 * @details     The type Spi_JobType is used for specifying the identification (ID) for a Job.
 */
typedef uint16 Spi_JobType;

/**
 * @typedef     Spi_SequenceType
 * @brief       Specifies the identification (ID) for a sequence of jobs.
 * @details     The type Spi_SequenceType specifies the identification (ID) for a sequence of jobs.
 */
typedef uint8 Spi_SequenceType;

/**
 * @typedef     Spi_HWUnitType
 * @brief       Specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit)
 * @details     The type Spi_HWUnitType specifies the identification (ID) for a SPI Hardware microcontroller peripheral (unit).
 */
typedef uint8 Spi_HWUnitType;

/** 
 * @typedef     Spi_BaudRateType
 * @brief       Type definition for SPI baud rate.
 */
typedef uint16_t Spi_BaudRateType;  

/** 
 * @typedef     Spi_ClockPolarityType
 * @brief       Enumeration for SPI clock polarity.
 */
typedef enum 
{
    SPI_CPOL_LOW = 0,    /**< Clock polarity is low */
    SPI_CPOL_HIGH = 1    /**< Clock polarity is high */
} Spi_ClockPolarityType;

/** 
 * @typedef     Spi_ClockPhaseType
 * @brief       Enumeration for SPI clock phase.
 */
typedef enum 
{
    SPI_CPHA_1EDGE = 0,  /**< First edge is used for data capture */
    SPI_CPHA_2EDGE = 1    /**< Second edge is used for data capture */
} Spi_ClockPhaseType;

/** 
 * @typedef     Spi_ModeType
 * @brief       Enumeration for SPI operating mode.
 */
typedef enum 
{
    SPI_MODE_MASTER = 0,  /**< SPI operates in master mode */
    SPI_MODE_SLAVE = 1    /**< SPI operates in slave mode */
} Spi_ModeType;

/** 
 * @typedef     Spi_NSSManagementType
 * @brief       Enumeration for NSS (Slave Select) management.
 */
typedef enum 
{
    SPI_NSS_SOFT = 0,     /**< Software management of NSS */
    SPI_NSS_HARD = 1      /**< Hardware management of NSS */
} Spi_NSSManagementType;

/** 
 * @typedef     Spi_DataSizeType
 * @brief       Enumeration for data size in SPI communication.
 */
typedef enum 
{
    SPI_DATASIZE_8BIT = 0,  /**< 8-bit data size */
    SPI_DATASIZE_16BIT = 1   /**< 16-bit data size */
} Spi_DataSizeType;

/**
 * @typedef     Spi_ConfigType
 * @brief       The contents of the initialization data structure are SPI specific.
 * @details     This type of the external data structure shall contain the initialization data for the SPI Handler/
 *              Driver.
 */
typedef struct
{
    Spi_ChannelType Channel;        /* SPI Channel (mandatory) */
    Spi_JobType Job;                /* SPI Job (mandatory) */
    Spi_SequenceType Sequence;      /* SPI Sequence (mandatory) */

    /* The following information are optional, we can set them to default value for STM32L476*/
    Spi_BaudRateType BaudRate;      /* Baudrate (optional, default value is 1MHz) */
    Spi_ClockPolarityType CPOL;     /* Clock Polarity (optional, default value is CPOL = 0) */
    Spi_ClockPhaseType CPHA;        /* Clock Phase (optional, default value is CPHA = 0) */
    Spi_ModeType Mode;              /* SPI mode (optional, default value mode is Master) */
    Spi_NSSManagementType NSS;      /* NSS management (optional, default value mode is hardware) */
    Spi_DataSizeType DataSize;      /* Data size (optional, default value mode is 8 bit) */
} Spi_ConfigType;

/*
 ************************************************************************************************************
 * Inline functions
 ************************************************************************************************************
 */


/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */

/**
 * @brief       Service for SPI initialization.
 * @param       ConfigPtr: Pointer to configuration set
 * @return      void
 */
void Spi_Init (const Spi_ConfigType* ConfigPtr);

/**
 * @brief       Service for SPI de-initialization.
 * @param       void
 * @return      Std_ReturnType 
 *              E_OK: de-initialisation command has been accepted
 *              E_NOT_OK: de-initialisation command has not been accepted
 */
Std_ReturnType Spi_DeInit (void);

/**
 * @brief       Service for writing one or more data to an IB SPI Handler/Driver Channel specified by parameter.
 * @param       Channel: Channel ID
 * @param       DataBufferPtr: Pointer to source data buffer. If this pointer is null, it is assumed
 *                             that the data to be transmitted is not relevant and the default
 *                             transmit value of this channel will be used instead.
 * @return      Std_ReturnType 
 *              E_OK: write command has been accepted
 *              E_NOT_OK: write command has not been accepted
 */
Std_ReturnType Spi_WriteIB (Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr);

/**
 * @brief       Service to transmit data on the SPI bus.
 * @param       Sequence: Sequence ID
 * @return      Std_ReturnType 
 *              E_OK: Transmission command has been accepted
 *              E_NOT_OK: Transmission command has not been accepted
 */
Std_ReturnType Spi_AsyncTransmit (Spi_SequenceType Sequence);

/**
 * @brief       Service for reading synchronously one or more data from an IB SPI Handler/Driver Channel
 *              specified by parameter.
 * @param       Channel: Channel ID
 * @return      Std_ReturnType 
 *              E_OK: read command has been accepted
 *              E_NOT_OK:  read command has not been accepted
 */
Std_ReturnType Spi_ReadIB (Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer);

/**
 * @brief       Service to setup the buffers and the length of data for the EB SPI Handler/Driver Channel
 *              specified.
 * @param       Channel: Channel ID
 * @param       SrcDataBufferPtr: Pointer to source data buffer.
 * @param       Length: Length (number of data elements) of the data to be transmitted
                        from SrcDataBufferPtr and/or received from DesDataBufferPtr
                        Min.: 1 Max.: Max of data specified at configuration for this channel
 * @param       DesDataBufferPtr: Pointer to destination data buffer in RAM.
 * @return      Std_ReturnType 
 *              E_OK: Setup command has been accepted
 *              E_NOT_OK: Setup command has not been accepted
 */
Std_ReturnType Spi_SetupEB (Spi_ChannelType Channel, 
                            const Spi_DataBufferType* SrcDataBufferPtr,
                            Spi_DataBufferType* DesDataBufferPtr,
                            Spi_NumberOfDataType Length);

/**
 * @brief       Service returns the SPI Handler/Driver software module status.
 * @param       void
 * @return      Spi_StatusType 
 */
Spi_StatusType Spi_GetStatus (void);

/**
 * @brief       This service returns the last transmission result of the specified Job.
 * @param       Job: Job ID. An invalid job ID will return an undefined result.
 * @return      Spi_JobResultType 
 */
Spi_JobResultType Spi_GetJobResult (Spi_JobType Job);

/**
 * @brief       This service returns the last transmission result of the specified Sequence.
 * @param       Sequence: Sequence ID. An invalid sequence ID will return an undefined result.
 * @return      Spi_SeqResultType 
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

/**
 * @brief       This service returns the version information of this module.
 * @param       versioninfo: Pointer to where to store the version information of this module.
 * @return      Spi_GetVersionInfo 
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**
 * @brief       Service to transmit data on the SPI bus
 * @param       Sequence: Sequence ID
 * @return      Std_ReturnType 
 *              E_OK: Transmission has been successful
 *              E_NOT_OK: Transmission failed
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

/**
 * @brief       This service returns the status of the specified SPI Hardware microcontroller peripheral
 * @param       HWUnit: SPI Hardware microcontroller peripheral (unit) ID.
 * @return      Spi_StatusType 
 */
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);

/**
 * @brief       Service cancels the specified on-going sequence transmission.
 * @param       Sequence: Sequence ID.
 * @return      void 
 */
void Spi_Cancel(Spi_SequenceType Sequence);

/**
 * @brief       Service to set the asynchronous mechanism mode for SPI busses handled asynchronously
 * @param       Mode: New mode required.
 * @return      Std_ReturnType
 *              E_OK: Setting command has been done
 *              E_NOT_OK: setting command has not been accepted 
 */
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

#endif /* SPI_H */