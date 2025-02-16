/**
 * @file        ComStack_Types.h
 * @author      Phuc
 * @brief       ComStack header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef CONSTACK_TYPES_H
#define CONSTACK_TYPES_H

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
 * @typedef     PduIdType
 * @brief       This type is used within the entire AUTOSAR Com Stack except for bus drivers.
 */
typedef uint8 PduIdType;

/** 
 * @typedef     PduLengthType
 * @brief       This type shall be used within the entire AUTOSAR Com Stack of an ECU except for bus drivers.
 */
typedef uint8 PduLengthType;

/** 
 * @typedef     PduInfoType
 * @brief       Variables of this type shall be used to store the basic information about a PDU of any type,
 *              namely a pointer variable pointing to its SDU (payload), a pointer to Meta Data of the PDU, and
 *              the corresponding length of the SDU in bytes.
 */
typedef struct
{
    uint8* SduDataPtr;          /* Pointer to the SDU (i.e. payload data) of the PDU. The type of this
                                pointer depends on the memory model being used at compile time. */
    uint8* MetaDataPtr;         /* Pointer to the meta data (e.g. CAN ID, socket ID, diagnostic addresses)
                                of the PDU, consisting of a sequence of meta data items. The length
                                and type of the meta data items is statically configured for each PDU.
                                Meta data items with more than 8 bits use platform byte order. */
    PduLengthType SduLength;    /* Length of the SDU in bytes. */
}PduInfoType;


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



#endif /* CONSTACK_TYPES_H */