/**
 * @file        Spi.c
 * @author      Phuc
 * @brief       SPI driver source file in AUTOSAR
 * @version     1.0
 * @date        2025-01-04
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Spi.h"
#include "Spi_Hw.h"
#include "Spi_Cfg.h"

/*
 ************************************************************************************************************
 * Static variables
 ************************************************************************************************************
 */
/* Variables to manage SPI status */
static Spi_StatusType SpiStatus = SPI_UNINIT;
static Spi_JobResultType JobResult = SPI_JOB_OK;
static Spi_SeqResultType SeqResult = SPI_SEQ_OK;

/*
 ************************************************************************************************************
 * Function definition
 ************************************************************************************************************
 */
/**
 * @brief       Service for SPI initialization.
 * @param       ConfigPtr: Pointer to configuration set
 * @return      void
 */
void Spi_Init (const Spi_ConfigType* ConfigPtr)
{
    /* Check if ConfigPtr is NULL */
    if (ConfigPtr == NULL_PTR)
    {
        return;
    }

    /* Setup default configuration */
    Spi_SetupDefaultConfig((Spi_ConfigType*)ConfigPtr);

    /* Initialize and activate hardware SPI1, SPI2 and SPI3 */
    switch (ConfigPtr->Channel)
    {
    case SPI_CHANNEL_1:
        /* Initialize SPI1 */
        Spi_Hw_Init_SPI1(ConfigPtr);

        /* Enable SPI1 */
        Spi_Hw_Enable_SPI1();
        break;

    case SPI_CHANNEL_2:
        /* Initialize SPI2 */
        Spi_Hw_Init_SPI2(ConfigPtr);

        /* Enable SPI2 */
        Spi_Hw_Enable_SPI2();
        break;
    
    case SPI_CHANNEL_3:
        /* Initialize SPI3 */
        Spi_Hw_Init_SPI3(ConfigPtr);

        /* Enable SPI3 */
        Spi_Hw_Enable_SPI3();
        break;

    default:
        /* If not SPI1, SPI2 or SPI3, initialize nothing */
        return;
    }

    /* Update initial status of SPI after successfully initialized */
    SpiStatus = SPI_IDLE;   /* SPI is ready and in idle mode */
    JobResult = SPI_JOB_OK; /* Configure Job status to default state */
    SeqResult = SPI_SEQ_OK; /* Configure Sequence status to default state */
}

/**
 * @brief       Service for SPI de-initialization.
 * @param       void
 * @return      Std_ReturnType 
 *              E_OK: de-initialisation command has been accepted
 *              E_NOT_OK: de-initialisation command has not been accepted
 */
Std_ReturnType Spi_DeInit (void)
{
    /* Check SPI status before de-initializing */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;  /* SPI has not been initialized */
    }

    /* De-initialize SPI1 hardware */
    Spi_Hw_DeInit_SPI1();

    /* De-initialize SPI2 hardware */
    Spi_Hw_DeInit_SPI2();

    /* De-initialize SPI3 hardware */
    Spi_Hw_DeInit_SPI3();

    /* Set SPI status to uninitialize */
    SpiStatus = SPI_UNINIT;

    /* Update job status and sequence status to default values */
    JobResult = SPI_JOB_OK;
    SeqResult = SPI_SEQ_OK;

    return E_OK;  /* SPI de-initialization successful */
}

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
Std_ReturnType Spi_WriteIB (Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;  /* SPI has not been initialized */
    }

    /* Check if data buffer is null */
    if (DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;  /* Invalid data buffer */
    }

    switch (Channel)
    {
    case SPI_CHANNEL_1:
        /* Wait for SPI1 available for sending data */
        Spi_Hw_WaitTransmitBufferEmpty_SPI1();

        /* Send data using SPI1 */
        Spi_SendData_SPI1(DataBufferPtr);
        break;

    case SPI_CHANNEL_2:
        /* Wait for SPI2 available for sending data */
        Spi_Hw_WaitTransmitBufferEmpty_SPI2();

        /* Send data using SPI2 */
        Spi_SendData_SPI2(DataBufferPtr);
        break;

    case SPI_CHANNEL_3:
        /* Wait for SPI3 available for sending data */
        Spi_Hw_WaitTransmitBufferEmpty_SPI3();

        /* Send data using SPI3 */
        Spi_SendData_SPI3(DataBufferPtr);
        break;
    
    default:
        /* If not SPI1, SPI2 or SPI3, return E_NOT_OK */
        return E_NOT_OK;
    }

    return E_OK; /* Write successful */
}

/**
 * @brief       Service to transmit data on the SPI bus.
 * @param       Sequence: Sequence ID
 * @return      Std_ReturnType 
 *              E_OK: Transmission command has been accepted
 *              E_NOT_OK: Transmission command has not been accepted
 */
Std_ReturnType Spi_AsyncTransmit (Spi_SequenceType Sequence)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;  /* SPI has not been initialized */
    }

    /* Check if sequence is too big or doesn't exist */
    if (Sequence >= SPI_SEQUENCE_MAX)
    {
        return E_NOT_OK; /* Sequence is invalid */
    }

    /* Take Sequence configuration */
    const Spi_SequenceConfigType* SequenceConfig = &Spi_Sequences[Sequence];

    /* Iterate through each Job in Sequence */
    for (uint8 jobIndex = 0; jobIndex < SequenceConfig->JobCount; jobIndex++)
    {
        Spi_JobType currentJob = SequenceConfig->Jobs[jobIndex];

        /* Take current Job configuration */
        const Spi_JobConfigType* JobConfig = &Spi_Jobs[currentJob];

        /* Take SPI channel from JobConfig */
        Spi_ChannelType channel = JobConfig->Channel;

        /* Send data for the current Job through corresponding channel */
        switch (channel)
        {
        case SPI_CHANNEL_1:
            /* Send data using SPI1 */
            Spi_WriteIB(SPI_CHANNEL_1, JobConfig->DataBuffer);
            break;

        case SPI_CHANNEL_2:
            /* Send data using SPI2 */
            Spi_WriteIB(SPI_CHANNEL_2, JobConfig->DataBuffer);
            break;

        case SPI_CHANNEL_3:
            /* Send data using SPI3 */
            Spi_WriteIB(SPI_CHANNEL_3, JobConfig->DataBuffer);
            break;
        
        default:
            /* If not SPI1, SPI2 or SPI3, return E_NOT_OK */
            return E_NOT_OK;
        }

        /* Update Job status */
        JobResult = SPI_JOB_OK;
    }
    /* After finishing all Jobs, upate Sequence status */
    SeqResult = SPI_SEQ_OK;

    return E_OK;
}

/**
 * @brief       Service for reading synchronously one or more data from an IB SPI Handler/Driver Channel
 *              specified by parameter.
 * @param       Channel: Channel ID
 * @return      Std_ReturnType 
 *              E_OK: read command has been accepted
 *              E_NOT_OK:  read command has not been accepted
 */
Std_ReturnType Spi_ReadIB (Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;  /* SPI has not been initialized */
    }

    /* Check if data buffer is null */
    if (DataBufferPointer == NULL_PTR)
    {
        return E_NOT_OK;  /* Invalid data buffer */
    }

    /* Check SPI channels (SPI1, SPI2 or SPI3) and read data */
    switch (Channel)
    {
    case SPI_CHANNEL_1:
        /* Wait until data in SPI1 is available to read */
        Spi_Hw_WaitReceiveBufferFull_SPI1();

        /* Read data from SPI1 */
        *DataBufferPointer = Spi_ReceiveData_SPI1();
        break;

    case SPI_CHANNEL_2:
        /* Wait until data in SPI2 is available to read */
        Spi_Hw_WaitReceiveBufferFull_SPI2();

        /* Read data from SPI2 */
        *DataBufferPointer = Spi_ReceiveData_SPI2();
        break;

    case SPI_CHANNEL_3:
        /* Wait until data in SPI3 is available to read */
        Spi_Hw_WaitReceiveBufferFull_SPI3();

        //* Read data from SPI3 */
        *DataBufferPointer = Spi_ReceiveData_SPI3();
        break;
    
    default:
        /* If not SPI1, SPI2 or SPI3, return E_NOT_OK */
        return E_NOT_OK;
    }

    return E_OK; /* Read successful */
}

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
                            Spi_NumberOfDataType Length)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;
    }

    if ((SrcDataBufferPtr == NULL_PTR) || (DesDataBufferPtr == NULL_PTR) || (Length == 0)) 
    {
        return E_NOT_OK;  /* Return error if pointers are NULL or length is zero */
    }

    /* Iterate through each element in buffer */
    for (Spi_NumberOfDataType i = 0; i < Length; i++)
    {
        switch (Channel)
        {
        case SPI_CHANNEL_1:
            /* Send data from source buffer */
            Spi_WriteIB(SPI_CHANNEL_1, &SrcDataBufferPtr[i]);

            /* Read data to destination buffer */
            Spi_ReadIB(SPI_CHANNEL_1, &DesDataBufferPtr[i]);
            break;
        
        case SPI_CHANNEL_2:
            /* Send data from source buffer */
            Spi_WriteIB(SPI_CHANNEL_2, &SrcDataBufferPtr[i]);

            /* Read data to destination buffer */
            Spi_ReadIB(SPI_CHANNEL_2, &DesDataBufferPtr[i]);
            break;

        case SPI_CHANNEL_3:
            /* Send data from source buffer */
            Spi_WriteIB(SPI_CHANNEL_3, &SrcDataBufferPtr[i]);

            /* Read data to destination buffer */
            Spi_ReadIB(SPI_CHANNEL_3, &DesDataBufferPtr[i]);
            break;

        default:
            /* If not SPI1, SPI2 or SPI3, return E_NOT_OK */
            return E_NOT_OK;
        }
    }
    return E_OK; /* Setup successful */
}

/**
 * @brief       Service returns the SPI Handler/Driver software module status.
 * @param       void
 * @return      Spi_StatusType 
 */
Spi_StatusType Spi_GetStatus (void)
{
    /* If SPI has not been initialized */
    if (SpiStatus == SPI_UNINIT)
    {
        return SPI_UNINIT;
    }

    /* Check status of SPI1 */
    if (Spi_Hw_CheckStatus_SPI1() == SPI_BUSY)
    {
        return SPI_BUSY;    /* SPI1 is busy */
    }

    /* Check status of SPI2 */
    if (Spi_Hw_CheckStatus_SPI2() == SPI_BUSY)
    {
        return SPI_BUSY;    /* SPI2 is busy */
    }

    /* Check status of SPI3 */
    if (Spi_Hw_CheckStatus_SPI3() == SPI_BUSY)
    {
        return SPI_BUSY;    /* SPI3 is busy */
    }

    /* If there is no busy SPI */
    return SPI_IDLE;
}

/**
 * @brief       This service returns the last transmission result of the specified Job.
 * @param       Job: Job ID. An invalid job ID will return an undefined result.
 * @return      Spi_JobResultType 
 */
Spi_JobResultType Spi_GetJobResult (Spi_JobType Job)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return SPI_JOB_FAILED; /* SPI has not been initialized */
    }

    /* Base on onoing Job, check corresponding SPI channel */
    if (Job == SPI_JOB_0 || Job == SPI_JOB_1)
    {
        /* Check Job status on SPI1 */
        return Spi_Hw_CheckJobStatus_SPI1();
    }
    else if (Job == SPI_JOB_2 || Job == SPI_JOB_3)
    {
        /* Check Job status on SPI2 */
        return Spi_Hw_CheckJobStatus_SPI2();
    }
    else if (Job == SPI_JOB_4 || Job == SPI_JOB_5)
    {
        /* Check Job status on SPI3 */
        return Spi_Hw_CheckJobStatus_SPI3();
    }
    else
    {
        /* If Job is invalid */
        return SPI_JOB_FAILED;
    }
}

/**
 * @brief       This service returns the last transmission result of the specified Sequence.
 * @param       Sequence: Sequence ID. An invalid sequence ID will return an undefined result.
 * @return      Spi_SeqResultType 
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return SPI_SEQ_FAILED; /* SPI has not been initialized */
    }

    /* Base on ongoing Sequence, check status of corresponding SPI */
    if (Sequence == SPI_SEQUENCE_0) 
    {
        /* Check status of Sequence on SPI1 */
        return Spi_Hw_CheckSequenceStatus_SPI1();
    }
    else if (Sequence == SPI_SEQUENCE_1) 
    {
        /* Check status of Sequence on SPI2 */
        return Spi_Hw_CheckSequenceStatus_SPI2();
    }
    else if (Sequence == SPI_SEQUENCE_2) 
    {
        /* Check status of Sequence on SPI3 */
        return Spi_Hw_CheckSequenceStatus_SPI3();
    }
    else
    {
        /* If Sequence is invalid */
        return SPI_SEQ_FAILED;
    }
}

/**
 * @brief       This service returns the version information of this module.
 * @param       versioninfo: Pointer to where to store the version information of this module.
 * @return      Spi_GetVersionInfo 
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
    /* Check if VersionInfo is null */
    if (VersionInfo == NULL_PTR) 
    {
        return;  
    }

    /* Information of vendor ID and module ID */
    VersionInfo->vendorID = 1;          
    VersionInfo->moduleID = 123;        

    /* Information of sw version */
    VersionInfo->sw_major_version = 1;  
    VersionInfo->sw_minor_version = 0;  
    VersionInfo->sw_patch_version = 0;  
}

/**
 * @brief       Service to transmit data on the SPI bus
 * @param       Sequence: Sequence ID
 * @return      Std_ReturnType 
 *              E_OK: Transmission has been successful
 *              E_NOT_OK: Transmission failed
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;    /* SPI has not been initialized*/
    }

    /* Call Spi_AsyncTransmit function first to start transmission */
    Std_ReturnType result = Spi_AsyncTransmit(Sequence);
    if (result != E_OK)
    {
        return E_NOT_OK;       /* Impossible to transmit asynchronously */
    }

    /* Wait until all Job in Sequence finish */
    Spi_SeqResultType seqResult;
    do
    {
        seqResult = Spi_GetSequenceResult(Sequence);
    } while (seqResult == SPI_SEQ_PENDING);  /* continue waiting if Sequence is ongoing */

    /* Check final result of Sequence */
    if (seqResult == SPI_SEQ_OK)
    {
        return E_OK;    /* Transmit successfully */
    }
    else
    {
        return E_NOT_OK;    /* transmission failed */
    }
}

/**
 * @brief       This service returns the status of the specified SPI Hardware microcontroller peripheral
 * @param       HWUnit: SPI Hardware microcontroller peripheral (unit) ID.
 * @return      Spi_StatusType 
 */
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit)
{
    /* Check status of SPI1 */
    switch (HWUnit)
    {
    case SPI_CHANNEL_1:
        return Spi_Hw_CheckHWStatus_SPI1();
        break;
    
    case SPI_CHANNEL_2:
        return Spi_Hw_CheckHWStatus_SPI2();
        break;

    case SPI_CHANNEL_3:
        return Spi_Hw_CheckHWStatus_SPI3();
        break;

    default:
        return SPI_UNINIT;
        break;
    }
}

/**
 * @brief       Service cancels the specified on-going sequence transmission.
 * @param       Sequence: Sequence ID.
 * @return      void 
 */
void Spi_Cancel(Spi_SequenceType Sequence)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return;    /* SPI has not been initialized*/
    }

    /* Base on Sequence, cancel ongoing transmission on corresponding SPI */
    if (Sequence == SPI_SEQUENCE_0) 
    {
        /* Cancel transmission on SPI1 */
        Spi_Hw_Cancel_SPI1();
    }
    else if (Sequence == SPI_SEQUENCE_1) 
    {
        /* Cancel transmission on SPI2 */
        Spi_Hw_Cancel_SPI2();
    }
    else if (Sequence == SPI_SEQUENCE_2) 
    {
        /* Cancel transmission on SPI3 */
        Spi_Hw_Cancel_SPI3();
    }
    else
    {
        /* If Sequence is invalid */
        return;
    }
    
    SeqResult = SPI_SEQ_CANCELED;
}

/**
 * @brief       Service to set the asynchronous mechanism mode for SPI busses handled asynchronously
 * @param       Mode: New mode required.
 * @return      Std_ReturnType
 *              E_OK: Setting command has been done
 *              E_NOT_OK: setting command has not been accepted 
 */
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode)
{
    /* Check SPI status */
    if (SpiStatus == SPI_UNINIT)
    {
        return E_NOT_OK;    /* SPI has not been initialized*/
    }

    /* Check for new mode */
    if (Mode == SPI_POLLING_MODE)
    {
        /* Change to Polling mode*/
        Spi_Hw_SetPollingMode();
        SpiStatus = SPI_IDLE;   /* Set SPI to IDLE in Polling mode */
    }
    else if (Mode == SPI_INTERRUPT_MODE)
    {
        /* Change to Polling mode*/
        Spi_Hw_SetInterruptMode();
        SpiStatus = SPI_IDLE;   /* Set SPI to IDLE in Interrupt mode */
    }
    else
    {
        /* if Mode is invalid */
        return E_NOT_OK;
    }

    return E_OK;
}
