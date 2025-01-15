/**
 * @file        Lin.h
 * @author      Phuc
 * @brief       LIN driver header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef LIN_H
#define LIN_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "stm32l4xx.h"
#include "stm32l476xx.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"
#include "Lin_GeneralTypes.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
#define SYNC_FIELD 0x55

/**
 * @typedef     Lin_ConfigType
 * @brief       This is the type of the external data structure containing the overall initialization data for the LIN
 *              driver and the SFR settings affecting the LIN channels.
 */
typedef struct
{
    uint32 Lin_BaudRate;                /* Baud rate for the LIN channel. */
    GPIO_TypeDef *Lin_Port;             /* GPIO port for the LIN channel, e.g., GPIOA. */
    uint16 Lin_TxPin;                   /* Tx pin for the LIN channel, e.g., GPIO_PIN_2. */
    uint16 Lin_RxPin;                   /* Rx pin for the LIN channel, e.g., GPIO_PIN_3. */
    uint8 Lin_Channel;                  /* LIN channel number. */
    FunctionalState Lin_WakeupSupport;  /* Wake-up mode support (TRUE/FALSE). */
    IRQn_Type Lin_IRQn;                 /* Interrupt number for the LIN channel. */
    uint32 Lin_Prescaler;               /* Prescaler value for adjusting baud rate. */
    uint32 Lin_Mode;                    /* Operating mode of LIN (0: master, 1: slave). */
    uint8 Lin_TimeoutDuration;          /* Timeout duration to detect errors. */
} Lin_ConfigType;

/*
 ************************************************************************************************************
 * Inline functions
 ************************************************************************************************************
 */
inline static void LIN_SendBreak(void)
{
	LL_USART_RequestBreakSending(USART2);
}

inline static void LIN_SendSync(void)
{
	// wait until TX ready
	while (!LL_USART_IsActiveFlag_TXE(USART2));
	// send 0x55
	LL_USART_TransmitData8(USART2, SYNC_FIELD);
	// wait until sending process is done
  while (!LL_USART_IsActiveFlag_TC(USART2));
}

inline static uint8_t LIN_CalculateParity(uint8_t id)
{
	uint8_t p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01);
  uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01));
  return (p0 | (p1 << 1)) << 6;
}

inline static void LIN_SendID(uint8_t id)
{
	uint8_t id_with_parity = id | LIN_CalculateParity(id);
	// wait until TX ready
	while (!LL_USART_IsActiveFlag_TXE(USART2)){}
  LL_USART_TransmitData8(USART2, id_with_parity);
  // wait until sending process is done
  while (!LL_USART_IsActiveFlag_TC(USART2));
}

inline static void LIN_SendData(uint8_t *data, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++)
  {
    // wait until TX ready
		while (!LL_USART_IsActiveFlag_TXE(USART2)){}
		LL_USART_TransmitData8(USART2, data[i]);
		// wait until sending process is done
		while (!LL_USART_IsActiveFlag_TC(USART2));
  }
}

inline static void LIN_ReceiveData(uint8_t *buffer, uint8_t length)
{
  for (uint8_t i = 0; i < length; i++)
  {
		// wait until data is available
    while (!LL_USART_IsActiveFlag_RXNE(USART2));
		// read data
    buffer[i] = LL_USART_ReceiveData8(USART2);;
  }
}

inline static uint8_t LIN_CalculateChecksum(uint8_t *data, uint8_t length)
{
  uint16_t checksum = 0;
  for (uint8_t i = 0; i < length; i++)
  {
    checksum += data[i];
    if (checksum > 0xFF)
    {
      checksum -= 0xFF;
    }
  }
  return ~checksum;
}

inline static void LIN_SendChecksum(uint8_t *data, uint8_t length)
{
  uint8_t checksum = LIN_CalculateChecksum(data, length);
	// wait until TX ready
	while (!LL_USART_IsActiveFlag_TXE(USART2)){}
  LL_USART_TransmitData8(USART2, checksum);
  // wait until sending process is done
  while (!LL_USART_IsActiveFlag_TC(USART2));
}

/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */
/**
 * @brief       Initializes the LIN module.
 * @param       Config: Pointer to LIN driver configuration set.
 * @return      void
 */
void Lin_Init(const Lin_ConfigType *Config);

/**
 * @brief       This function checks if a wakeup has occurred on the addressed LIN channel.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: API call has been accepted
 *              E_NOT_OK: API call has not been accepted
 */
Std_ReturnType Lin_CheckWakeup(uint8 Channel);

/**
 * @brief       Returns the version information of this module
 * @param       versioninfo: Pointer to where is stored the version information of this module.
 * @return      void
 */
void Lin_GetVersionInfo(Std_VersionInfoType *versioninfo);

/**
 * @brief       Sends a LIN header and a LIN response, if necessary. The direction of the frame response
 *              (master response, slave response, slave-to-slave communication) is provided by the PduInfoPtr
 * @param       Channel: LIN channel to be addressed
 * @param       PduInfoPtr: Pointer to PDU containing the PID, checksum model, response type, Dl and SDU data pointer
 * @return      Std_ReturnType
 *              E_OK: Send command has been accepted.
 *              E_NOT_OK: Send command has not been accepted, development or production error occurred.
 */
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType *PduInfoPtr);

/**
 * @brief       The service instructs the driver to transmit a go-to-sleep-command on the addressed LIN channel.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Sleep command has been accepted
 *              E_NOT_OK: Sleep command has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_GoToSleep(uint8 Channel);

/**
 * @brief       Sets the channel state to LIN_CH_SLEEP, enables the wake-up detection and optionally sets
 *              the LIN hardware unit to reduced power operation mode (if supported by HW).
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Command has been accepted
 *              E_NOT_OK: Command has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel);

/**
 * @brief       Generates a wake up pulse and sets the channel state to LIN_CH_OPERATIONAL.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Wake-up request has been accepted
 *              E_NOT_OK: Wake-up request has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_Wakeup(uint8 Channel);

/**
 * @brief       Sets the channel state to LIN_CH_OPERATIONAL without generating a wake up pulse.
 * @param       Channel: LIN channel to be addressed
 * @return      Std_ReturnType
 *              E_OK: Wake-up request has been accepted
 *              E_NOT_OK: Wake-up request has not been accepted, development or production error occurred
 */
Std_ReturnType Lin_WakeupInternal(uint8 Channel);

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
Lin_StatusType Lin_GetStatus (uint8 Channel, const uint8** Lin_SduPtr);

#endif /* LIN_H */