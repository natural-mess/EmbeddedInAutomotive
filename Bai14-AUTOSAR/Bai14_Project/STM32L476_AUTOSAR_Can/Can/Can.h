/**
 * @file        Can.h
 * @author      Phuc
 * @brief       CAN driver header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef CAN_H
#define CAN_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
#define CAN_AF ((uint8_t)0x09)
#define CAN_SJW_1TQ                 (0x00000000U) 

/** @defgroup CAN_interrupts CAN Interrupts
  * @{
  */
#define CAN_IT_TME                  ((uint32_t)CAN_IER_TMEIE)   /*!< Transmit mailbox empty interrupt */

/* Receive Interrupts */
#define CAN_IT_FMP0                 ((uint32_t)CAN_IER_FMPIE0)  /*!< FIFO 0 message pending interrupt */
#define CAN_IT_FF0                  ((uint32_t)CAN_IER_FFIE0)   /*!< FIFO 0 full interrupt            */
#define CAN_IT_FOV0                 ((uint32_t)CAN_IER_FOVIE0)  /*!< FIFO 0 overrun interrupt         */
#define CAN_IT_FMP1                 ((uint32_t)CAN_IER_FMPIE1)  /*!< FIFO 1 message pending interrupt */
#define CAN_IT_FF1                  ((uint32_t)CAN_IER_FFIE1)   /*!< FIFO 1 full interrupt            */
#define CAN_IT_FOV1                 ((uint32_t)CAN_IER_FOVIE1)  /*!< FIFO 1 overrun interrupt         */

/* Operating Mode Interrupts */
#define CAN_IT_WKU                  ((uint32_t)CAN_IER_WKUIE)  /*!< Wake-up interrupt           */
#define CAN_IT_SLK                  ((uint32_t)CAN_IER_SLKIE)  /*!< Sleep acknowledge interrupt */

/* Error Interrupts */
#define CAN_IT_EWG                  ((uint32_t)CAN_IER_EWGIE) /*!< Error warning interrupt   */
#define CAN_IT_EPV                  ((uint32_t)CAN_IER_EPVIE) /*!< Error passive interrupt   */
#define CAN_IT_BOF                  ((uint32_t)CAN_IER_BOFIE) /*!< Bus-off interrupt         */
#define CAN_IT_LEC                  ((uint32_t)CAN_IER_LECIE) /*!< Last error code interrupt */
#define CAN_IT_ERR                  ((uint32_t)CAN_IER_ERRIE) /*!< Error Interrupt           */

/**
 * @typedef     Can_ConfigType
 * @brief       This is the type of the external data structure containing the overall initialization data for the CAN
 *              driver and SFR settings affecting all controllers. Furthermore it contains pointers to controller
 *              configuration structures. The contents of the initialization data structure are CAN hardware
 *              specific.
 */
typedef struct
{
    uint16 CAN_Prescaler;                   /* Specifies the length of a time quantum.
                                            It ranges from 1 to 1024. */
    uint8 CAN_Mode;                         /* Specifies the CAN operating mode.
                                            This parameter can be a value of CAN_operating_mode. */
    uint8 CAN_SJW;                          /* Specifies the maximum number of time quanta 
                                            the CAN hardware is allowed to lengthen or 
                                            shorten a bit to perform resynchronization.
                                            This parameter can be a value of CAN_synchronisation_jump_width. */
    uint8 CAN_BS1;                          /* Specifies the number of time quanta in Bit Segment 1.
                                            This parameter can be a value of CAN_time_quantum_in_bit_segment_1. */
    uint8 CAN_BS2;                          /* Specifies the number of time quanta in Bit Segment 2.
                                            This parameter can be a value of CAN_time_quantum_in_bit_segment_2. */
    
    FunctionalState CAN_TTCM;               /* Enable or disable the time-triggered communication mode.
                                            This parameter can be set either to ENABLE or DISABLE. */
    FunctionalState CAN_ABOM;               /* Enable or disable the automatic bus-off management.
                                            This parameter can be set either to ENABLE or DISABLE. */
    FunctionalState CAN_AWUM;               /* Enable or disable the automatic wake-up mode.
                                            This parameter can be set either to ENABLE or DISABLE. */
    FunctionalState CAN_NART;               /* Enable or disable the no-automatic retransmission mode.
                                            This parameter can be set either to ENABLE or DISABLE. */
    FunctionalState CAN_RFLM;               /* Enable or disable the Receive FIFO Locked mode.
                                            This parameter can be set either to ENABLE or DISABLE. */
    FunctionalState CAN_TXFP;               /* Enable or disable the transmit FIFO priority.
                                            This parameter can be set either to ENABLE or DISABLE. */
} Can_ConfigType;

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
 * @brief       This function initializes the module.
 * @param       Config: Pointer to driver configuration.
 * @retval      void
 */
void Can_Init(const Can_ConfigType* Config);

/**
 * @brief       This service returns the version information of this module.
 * @param       versioninfo: Pointer to where to store the version information of this module.
 * @return      Spi_GetVersionInfo 
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**
 * @brief       This function de-initializes the module.
 * @param       void
 * @retval      void
 */
void Can_DeInit(void);

/**
 * @brief       This service shall set the baud rate configuration of the CAN controller. Depending on
 *              necessary baud rate modifications the controller might have to reset.
 * @param       Controller: CAN controller, whose baud rate shall be set
 * @param       BaudRateConfigID: references a baud rate configuration by ID (see CanController BaudRateConfig
 * @retval      Std_ReturnType: 
 *              E_OK: Service request accepted, setting of (new) baud rate started
 *              E_NOT_OK: Service request not accepted
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);

/**
 * @brief       This function performs software triggered state transitions of the CAN controller State machine.
 * @param       Controller: CAN controller for which the status shall be changed
 * @param       Transition: Transition value to request new CAN controller state
 * @retval      Std_ReturnType: 
 *              E_OK: request accepted
 *              E_NOT_OK: request not accepted, a development error occurred
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);

/**
 * @brief       This function disables all interrupts for this CAN controller.
 * @param       Controller: CAN controller for which interrupts shall be disabled.
 * @retval      Void
 */
void Can_DisableControllerInterrupts(uint8 Controller);

/**
 * @brief       This function enables all allowed interrupts.
 * @param       Controller: CAN controller for which interrupts shall be re-enabled
 * @retval      Void
 */
void Can_EnableControllerInterrupts(uint8 Controller);

/**
 * @brief       This function checks if a wakeup has occurred for the given controller
 * @param       Controller: Controller to be checked for a wakeup.
 * @retval      Std_ReturnType: 
 *              E_OK: API call has been accepted
 *              E_NOT_OK: API call has not been accepted
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller);

/**
 * @brief     This service obtains the error state of the CAN controller.
 * @param     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for ErrorState.
 * @param     ErrorStatePtr: Pointer to a memory location, where the error state of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Error state request has been accepted.
 *            E_NOT_OK: Error state request has not been accepted
 */
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType *ErrorStatePtr);

/**
 * @brief     This service reports about the current status of the requested CAN controller.
 * @param     Controller: CAN controller for which the status shall be requested.
 * @param     ControllerModePtr: Pointer to a memory location, where the current mode of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Controller mode request has been accepted.
 *            E_NOT_OK: Controller mode request has not been accepted.
 */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType *ControllerModePtr);

/**
 * @brief     Returns the Rx error counter for a CAN controller. 
 * @param     ControllerId: CAN controller, whose current Rx error counter shall be acquired.
 * @param     RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Rx error counter available.
 *            E_NOT_OK: Wrong ControllerId, or Rx error counter not available.
 */
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8 *RxErrorCounterPtr);

/**
 * @brief     Returns the Tx error counter for a CAN controller. 
 * @param     ControllerId: CAN controller, whose current Rx error counter shall be acquired.
 * @param     RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Rx error counter available.
 *            E_NOT_OK: Wrong ControllerId, or Rx error counter not available.
 */
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8 *TxErrorCounterPtr);

/**
 * @brief     This function is called by CanIf to pass a CAN message to CanDrv for transmission
 * @param     Hth: information which HW-transmit handle shall be used for transmit
 * @param     PduInfo: Pointer to SDU user memory, Data Length and Identifier.
 * @retval    Std_ReturnType: 
 *            E_OK: Write command has been accepted
 *            E_NOT_OK: development error occurred
*/
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);

#endif /* CAN_H */