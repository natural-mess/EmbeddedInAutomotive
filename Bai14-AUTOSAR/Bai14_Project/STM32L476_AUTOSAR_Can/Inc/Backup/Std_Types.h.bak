/**
 * @file        Std_Types.h
 * @author      Phuc
 * @brief       Standard types for AUTOSAR
 * @version     1.0
 * @date        2024-12-29
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include <stdint.h>

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */

/**
 * @brief       Software version number
 */
#define STD_TYPES_SW_MAJOR_VERSION 0x01u
#define STD_TYPES_SW_MINOR_VERSION 0x00u
#define STD_TYPES_SW_PATCH_VERSION 0x00u

/**
 * @brief       Standard return types
 * @details     This type can be used as standard API return type which is shared between the RTE and the BSW
 *              modules. The Std_ReturnType shall normally be used with value E_OK or E_NOT_OK. 
 */
/* Standard Return Type */
typedef uint8_t Std_ReturnType;

/* Because E_OK is already defined within OSEK, the symbol E_OK has to be shared. To avoid
name clashes and redefinition problems, the symbols have to be defined in the following way
(approved within implementation): */
#ifndef STATUSTYPEDEFINED
    #define STATUSTYPEDEFINED
    #define E_OK 0x00u
    typedef unsigned char StatusType; /* OSEK compliance */
#endif

#define E_NOT_OK 0x01u

/**
 * @brief       Definition of datatype STD_HIGH, STD_LOW
 */
#define STD_HIGH 0x01u /* Physical state 5V or 3.3V */
#define STD_LOW  0x00u /* Physical state 0V */

/**
 * @brief       Definition of null pointer
 * @details     The implementation shall provide the NULL_PTR define with a void pointer to   *              zero definition
 */
#ifndef 	NULL_PTR
    #define	NULL_PTR    ((void*)0)
#endif

/**
 * @brief       Definition of datatype TRUE_FALSE
 */
#ifndef TRUE
    #define TRUE 1u
#endif

#ifndef FALSE
    #define FALSE 0u
#endif

/**
 * @brief       Definition of ImplementationDataType
 * @details     Concerning the signed integer types, AUTOSAR supports
 *              for compiler and target implementation only 2 complement arithmetic. This *              directly mpacts the chosen ranges for these types.
 */
typedef signed char         sint8;
typedef unsigned char       uint8;
typedef signed short        sint16;
typedef unsigned short      uint16;
typedef signed long         sint32;
typedef unsigned long       uint32;
typedef signed long long    sint64;
typedef unsigned long long  uint64;

typedef float               float32;
typedef double              float64;

/**
 * @brief       Definition of datatype Std_VersionInfoType
 * @details     This type shall be used to request the version of a BSW module using the <Module name>_Get
 *              VersionInfo() function.
 */
typedef struct {
    uint16 vendorID;
    uint16 moduleID;
    uint8  sw_major_version;
    uint8  sw_minor_version;
    uint8  sw_patch_version;
} Std_VersionInfoType;

#endif /* STD_TYPES_H */
