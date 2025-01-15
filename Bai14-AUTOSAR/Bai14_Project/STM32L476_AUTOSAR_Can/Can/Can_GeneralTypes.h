/**
 * @file        Can_GeneralTypes.h
 * @author      Phuc
 * @brief       CAN general types header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef CAN_GENERALTYPES_H
#define CAN_GENERALTYPES_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Std_Types.h"
#include "Can.h"
#include "ComStack_Types.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
 /** 
 * @typedef     Can_IdType
 * @brief       Represents the Identifier of an L-PDU. The two most significant bits specify the frame type: 00
 *              CAN message with Standard CAN ID 01 CAN FD frame with Standard CAN ID 10 CAN message
 *              with Extended CAN ID 11 CAN FD frame with Extended CAN ID
 */
typedef uint32 Can_IdType;

/** 
 * @typedef     Can_PduType
 * @brief       This type unites PduId (swPduHandle), SduLength (length), SduData (sdu), and CanId (id) for any
 *              CAN L-SDU.
 */
typedef struct {
    PduIdType swPduHandle; 
    uint8 length; 
    Can_IdType id;
    uint8 *sdu;    
} Can_PduType;

/** 
 * @typedef     Can_HwHandleType
 * @brief       Represents the hardware object handles of a CAN hardware unit. For CAN hardware units with
 *              more than 255 HW objects use extended range.
 */
typedef uint8 Can_HwHandleType;

/** 
 * @typedef     Can_HwType
 * @brief       This type defines a data structure which clearly provides an Hardware Object Handle including its
 *              corresponding CAN Controller and therefore CanDrv as well as the specific CanId.
 */
typedef struct {
    Can_IdType CanId;       /* Standard/Extended CAN ID of CAN L-PDU */
    Can_HwHandleType Hoh;   /* ID of the corresponding Hardware Object Range */
    uint8 ControllerId;     /* ControllerId provided by CanIf clearly identify the corresponding controller */
} Can_HwType;

/* Overlayed return value of Std_ReturnType for CAN driver API Can_Write() */
#define CAN_BUSY 0x02u      /* transmit request could not be processed because no transmit object was available */

/** 
 * @typedef     Can_ErrorStateType
 * @brief       Error states of a CAN controller.
 */
typedef enum {
    CAN_ERRORSTATE_ACTIVE,   /* The CAN controller takes fully part in communication. */
    CAN_ERRORSTATE_PASSIVE,  /* The CAN controller takes part in communication, but does not send active error frames. */
    CAN_ERRORSTATE_BUSOFF    /* The CAN controller does not take part in communication */
} Can_ErrorStateType;

/** 
 * @typedef     Can_ControllerStateType
 * @brief       States that are used by the several ControllerMode functions.
 */
typedef enum {
    CAN_CS_UNINIT,          /* CAN controller state UNINIT */
    CAN_CS_STARTED,         /* CAN controller state STARTED */
    CAN_CS_STOPPED,         /* CAN controller state STOPPED */
    CAN_CS_SLEEP            /* CAN controller state SLEEP */
} Can_ControllerStateType;

/** 
 * @typedef     Can_ErrorType
 * @brief       The enumeration represents a superset of CAN Error Types which typical CAN HW is able to
 *              report. That means not all CAN HW will be able to support the complete set.
 */
typedef enum {
    CAN_ERROR_BIT_MONITORING1 = 0x01u,          /* A 0 was transmitted and a 1 was read back */
    CAN_ERROR_BIT_MONITORING0 = 0x02u,          /* A 1 was transmitted and a 0 was read back */
    CAN_ERROR_BIT = 0x03u,                      /* The HW reports a CAN bit error but cannot report distinguish between CAN_ERROR_
                                                BIT_MONITORING1 and CAN_ERROR_BIT_MONITORING0 */
    CAN_ERROR_CHECK_ACK_FAILED = 0x04u,         /* Acknowledgement check failed */
    CAN_ERROR_ACK_DELIMITER = 0x05u,            /* Acknowledgement delimiter check failed */
    CAN_ERROR_ARBITRATION_LOST = 0x06u,         /* The sender lost in arbitration */
    CAN_ERROR_OVERLOAD = 0x07u,                 /* CAN overload detected via an overload frame. Indicates that the receive buffers of a
                                                receiver are full. */
    CAN_ERROR_CHECK_FORM_FAILED = 0x08u,        /* Violations of the fixed frame format */
    CAN_ERROR_CHECK_STUFFING_FAILED = 0x09u,    /* Stuffing bits not as expected */
    CAN_ERROR_CHECK_CRC_FAILED = 0xAu,          /* CRC failed */
    CAN_ERROR_BUS_LOCK = 0xBu                   /* Bus lock (Bus is stuck to dominant level) */
} Can_ErrorType;

/** 
 * @typedef     Can_TimeStampType
 * @brief       Variables of this type are used to express time stamps based on relative time.
 *              Value range: * Seconds: 0 .. 4.294.967.295 s (circa 136 years) * Nanoseconds: 0 .. 999.999.999 ns
 */
typedef struct {
    uint32 nanoseconds;         /* Nanoseconds part of the time */
    uint32 seconds;             /* Seconds part of the time */
} Can_TimeStampType;

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



#endif /* CAN_GENERALTYPES_H */