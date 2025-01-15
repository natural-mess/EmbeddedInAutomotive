/**
 * @file        Lin.c
 * @author      Phuc
 * @brief       LIN driver source file in AUTOSAR
 * @version     1.0
 * @date        2025-01-15
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Lin.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
/* Define the maximum number of LIN channels */
#define MAX_LIN_CHANNELS 2    /* Maximum number of LIN channels. */

/* Structure for LIN Channel Configuration */
typedef struct {
    uint32 Lin_BaudRate;                        /* Baud rate for the LIN channel. */
    FunctionalState LinChannelWakeupSupport;    /* Wake-up support (ENABLE/DISABLE). */
    uint8 Lin_ChannelID;                        /* ID of the LIN channel. */
    GPIO_TypeDef* Lin_Port;                     /* GPIO port for the LIN channel. */
    uint16 Lin_TxPin;                           /* Tx pin of the LIN channel. */
    uint16 Lin_RxPin;                           /* Rx pin of the LIN channel. */
} LinChannelConfigType;

/* Array to store the state of each LIN channel */
Lin_StatusType LinChannelState[MAX_LIN_CHANNELS] = {
    LIN_CH_SLEEP,    // Initial state for each channel
    LIN_CH_SLEEP     // State for the second channel if needed
};

/* Array to store configuration for each LIN channel */
LinChannelConfigType LinChannelConfig[MAX_LIN_CHANNELS] = {
    {
        .Lin_BaudRate = 19200,              /* Baud rate for the LIN channel. */
        .LinChannelWakeupSupport = ENABLE,  /* Wake-up support. */
        .Lin_ChannelID = 0,                 /* ID of the LIN channel. */
        .Lin_Port = GPIOA,                  /* GPIO port used for the LIN channel. */
        .Lin_TxPin = LL_GPIO_PIN_2,            /* Tx pin of the LIN channel. */
        .Lin_RxPin = LL_GPIO_PIN_3            /* Rx pin of the LIN channel. */
    }
    // Add more channel configurations if needed
};

/* Array to store the data for each LIN channel */
uint8 LinChannelData[MAX_LIN_CHANNELS][8]; // Assuming a maximum of 8 bytes per channel

/*
 ************************************************************************************************************
 * Function definition
 ************************************************************************************************************
 */
/**
 * @brief       Initializes the LIN module.
 * @param       Config: Pointer to LIN driver configuration set.
 * @return      void
 */
void Lin_Init(const Lin_ConfigType *Config)
{
    /* Check if the configuration is valid */
    if (Config == NULL_PTR) 
    {
        return; /* Return if the configuration is invalid */
    }

    /* Activate clock for GPIOA used for communicate LIN */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    /**USART2 GPIO Configuration
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    USART_InitStruct.BaudRate = Config->Lin_BaudRate;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART2, &USART_InitStruct);
    LL_USART_SetLINBrkDetectionLen(USART2, LL_USART_LINBREAK_DETECT_10B);
    LL_USART_DisableDMADeactOnRxErr(USART2);
    LL_USART_ConfigLINMode(USART2);
    LL_USART_Enable(USART2);
    LL_USART_EnableLIN(USART2);
}

/**
 * @brief       This function checks if a wakeup has occurred on the addressed LIN channel.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: API call has been accepted
 *              E_NOT_OK: API call has not been accepted
 */
Std_ReturnType Lin_CheckWakeup(uint8 Channel)
{
     if (Channel != 0) 
     {
        return E_NOT_OK; // Invalid channel
    }

    if (LL_USART_IsActiveFlag_WKUP(USART2)) 
    {
        /* Clear the wake-up flag */
        LL_USART_ClearFlag_WKUP(USART2);
        
        /* Return E_OK if wakeup was detected */
        return E_OK;
    }

    /* No wakeup detected */
    return E_NOT_OK;
}

/**
 * @brief       Returns the version information of this module
 * @param       versioninfo: Pointer to where is stored the version information of this module.
 * @return      void
 */
void Lin_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    /* Check if versioninfo is null */
    if (versioninfo == NULL_PTR) 
    {
        return;  
    }

    /* Information of vendor ID and module ID */
    versioninfo->vendorID = 1;          
    versioninfo->moduleID = 123;        

    /* Information of sw version */
    versioninfo->sw_major_version = 1;  
    versioninfo->sw_minor_version = 0;  
    versioninfo->sw_patch_version = 0;  
}

/**
 * @brief       Sends a LIN header and a LIN response, if necessary. The direction of the frame response
 *              (master response, slave response, slave-to-slave communication) is provided by the PduInfoPtr
 * @param       Channel: LIN channel to be addressed
 * @param       PduInfoPtr: Pointer to PDU containing the PID, checksum model, response type, Dl and SDU data pointer
 * @return      Std_ReturnType
 *              E_OK: Send command has been accepted.
 *              E_NOT_OK: Send command has not been accepted, development or production error occurred.
 */
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType *PduInfoPtr)
{
    if (PduInfoPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    LIN_SendBreak();
    LIN_SendSync();
    LIN_SendID(PduInfoPtr->Pid);
    LIN_SendData(PduInfoPtr->SduPtr, PduInfoPtr->Dl);
    LIN_SendChecksum(PduInfoPtr->SduPtr, PduInfoPtr->Dl);

    return E_OK;
}

/**
 * @brief       The service instructs the driver to transmit a go-to-sleep-command on the addressed LIN channel.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Sleep command has been accepted
 *              E_NOT_OK: Sleep command has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_GoToSleep(uint8 Channel)
{
    // Check the validity of the Channel
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Invalid Channel
    }

    // Send the "go-to-sleep" signal by transmitting the Break field and sending the sleep ID frame
    LIN_SendBreak(); // Transmit Break field to signal sleep

    // Wait for the transmission to complete
    while (!LL_USART_IsActiveFlag_TC(USART2));

    uint8 LIN_GO_TO_SLEEP = 0xFF;
    LIN_SendData(&LIN_GO_TO_SLEEP, 1); // Transmit frame with sleep ID

    // Wait for the transmission to complete
    while (!LL_USART_IsActiveFlag_TC(USART2));

    // Set the LIN channel state to sleep mode
    LinChannelState[Channel] = LIN_CH_SLEEP;

    return E_OK; // Sleep command executed successfully
}

/**
 * @brief       Sets the channel state to LIN_CH_SLEEP, enables the wake-up detection and optionally sets
 *              the LIN hardware unit to reduced power operation mode (if supported by HW).
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Command has been accepted
 *              E_NOT_OK:  Command has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel)
{
    // Check if the Channel is valid
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Return error if channel is invalid
    }

    // Send the "go-to-sleep" signal by transmitting the Break field and sending the sleep ID
    LIN_SendBreak(); // Transmit Break field to signal sleep mode

    // Wait for the transmission to complete
    while (!LL_USART_IsActiveFlag_TC(USART2));

    // Update the LIN channel state to sleep mode
    LinChannelState[Channel] = LIN_CH_SLEEP;

    // Activate wake-up detection if necessary
    if (LinChannelConfig[Channel].LinChannelWakeupSupport == ENABLE)
    {
        // Enable wake-up detection mode, which could be through interrupts or bus monitoring
    }

    return E_OK; // Return `E_OK` if the process is successful
}

/**
 * @brief       Generates a wake up pulse and sets the channel state to LIN_CH_OPERATIONAL.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Wake-up request has been accepted
 *              E_NOT_OK: Wake-up request has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_Wakeup(uint8 Channel)
{
     // Check if the Channel is valid
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return E_NOT_OK; // Return error if Channel is invalid
    }

    // Check the channel state; it must be LIN_CH_SLEEP to continue
    if (LinChannelState[Channel] != LIN_CH_SLEEP)
    {
        return E_NOT_OK; // Return error if the channel is not in sleep state
    }

    // Send a wake-up signal by transmitting a dominant bit
		uint8 dominantBit = 0x80;
    LIN_SendData(&dominantBit, 1); // Transmit byte with dominant bit 0b10000000

    // Wait for the transmission to complete
    while (!LL_USART_IsActiveFlag_TC(USART2));

    // Update the channel state to LIN_CH_OPERATIONAL
    LinChannelState[Channel] = LIN_OPERATIONAL;

    return E_OK; // Return `E_OK` if successful
}

/**
 * @brief       Sets the channel state to LIN_CH_OPERATIONAL without generating a wake up pulse.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Wake-up request has been accepted
 *              E_NOT_OK: Wake-up request has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_WakeupInternal(uint8 Channel)
{
    return E_OK;
}

/**
 * @brief       Gets the status of the LIN driver
 * @param       Channel: LIN channel to be addressed
 * @param       Lin_SduPtr: Pointer to pointer to a shadow buffer or memory mapped LIN
 * @return      Lin_StatusType
 *              LIN_NOT_OK: Development or production error occurred
 *              LIN_TX_OK: Successful transmission
 *              LIN_TX_BUSY: Ongoing transmission (Header or Response)
 *              LIN_TX_HEADER_ERROR: Erroneous header transmission such as:
 *              - Mismatch between sent and read back data
 *              - Identifier parity error or Physical bus error
 *              LIN_TX_ERROR: Erroneous response transmission such as:
 *              - Mismatch between sent and read back data
 *              - Physical bus error
 *              LIN_RX_OK: Reception of correct response
 *              LIN_RX_BUSY: Ongoing reception: at least one response byte
 *              has been received, but the checksum byte has not been received
 *              LIN_RX_ERROR: Erroneous response reception such as:
 *              - Framing error
 *              - Overrun error
 *              - Checksum error or Short response
 *              LIN_RX_NO_RESPONSE: No response byte has been received so far
 *              LIN_OPERATIONAL: Normal operation; the related LIN channel
 *              is woken up from the LIN_CH_SLEEP and no data has been sent.
 *              LIN_CH_SLEEP: Sleep state operation; in this state wake-up
 *              detection from slave nodes is enabled.
 */
Lin_StatusType Lin_GetStatus (uint8 Channel, const uint8** Lin_SduPtr)
{
    // Check the validity of the input pointer
    if (Lin_SduPtr == NULL_PTR)
    {
        return LIN_NOT_OK; // Return error if pointer is invalid
    }

    // Check if the Channel is within a valid range
    if (Channel >= MAX_LIN_CHANNELS)
    {
        return LIN_NOT_OK; // Return error if Channel is invalid
    }

    // Retrieve the current status from hardware or a status variable
    Lin_StatusType currentStatus = LinChannelState[Channel];

    // If the status is LIN_RX_OK or LIN_TX_OK, update Lin_SduPtr
    if (currentStatus == LIN_RX_OK || currentStatus == LIN_TX_OK)
    {
        *Lin_SduPtr = LinChannelData[Channel]; // LinChannelData is the data region containing the SDU
    }
    else
    {
        *Lin_SduPtr = NULL_PTR; // If no data, set the SDU pointer to NULL
    }

    return currentStatus; // Return the current status of the LIN channel
}
