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

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
typedef enum {
  DISABLE = 0,
  ENABLE = 1    
} FunctionalState;

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


#endif /* CAN_H */