/**
 * @file        Dio.h
 * @author      Phuc
 * @brief       Dio driver header file in AUTOSAR
 * @version     1.0
 * @date        2024-12-29
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

#ifndef DIO_H
#define DIO_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Std_Types.h"
#include "stm32l4xx_ll_gpio.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
#define DIO_PORT_A   0u
#define DIO_PORT_B   1u
#define DIO_PORT_C   2u
#define DIO_PORT_D   3u
#define DIO_PORT_E   4u
#define DIO_PORT_F   5u
#define DIO_PORT_G   6u
#define DIO_PORT_H   7u

/**
 * @brief       Macro to extract the port from a channel ID
 * @param       ChannelId: The channel ID
 * @return      The port number
 */
#define DIO_GET_PORT(ChannelId)     ((ChannelId) >> 4u)

/**
 * @brief       Macro to extract the pin from a channel ID
 * @param       ChannelId: The channel ID
 * @return      The pin number
 */
#define DIO_GET_PIN(ChannelId)      (1u << ((ChannelId) & 0x0Fu))

/**
 * @brief       Macro determines ChannelId for each GPIO pin
 * @param       GPIOx: GPIO port x
 * @param       Pin: GPIO pin
 */
#define DIO_CHANNEL(DIO_PORT_x, Pin)     (((DIO_PORT_x) << 4u) + (Pin))

/* GPIO A Pins 0 to 15 */
#define DIO_CHANNEL_A0      DIO_CHANNEL(DIO_PORT_A, 0)
#define DIO_CHANNEL_A1      DIO_CHANNEL(DIO_PORT_A, 1)
#define DIO_CHANNEL_A2      DIO_CHANNEL(DIO_PORT_A, 2)
#define DIO_CHANNEL_A3      DIO_CHANNEL(DIO_PORT_A, 3)
#define DIO_CHANNEL_A4      DIO_CHANNEL(DIO_PORT_A, 4)
#define DIO_CHANNEL_A5      DIO_CHANNEL(DIO_PORT_A, 5)
#define DIO_CHANNEL_A6      DIO_CHANNEL(DIO_PORT_A, 6)
#define DIO_CHANNEL_A7      DIO_CHANNEL(DIO_PORT_A, 7)
#define DIO_CHANNEL_A8      DIO_CHANNEL(DIO_PORT_A, 8)
#define DIO_CHANNEL_A9      DIO_CHANNEL(DIO_PORT_A, 9)
#define DIO_CHANNEL_A10     DIO_CHANNEL(DIO_PORT_A, 10)
#define DIO_CHANNEL_A11     DIO_CHANNEL(DIO_PORT_A, 11)
#define DIO_CHANNEL_A12     DIO_CHANNEL(DIO_PORT_A, 12)
#define DIO_CHANNEL_A13     DIO_CHANNEL(DIO_PORT_A, 13)
#define DIO_CHANNEL_A14     DIO_CHANNEL(DIO_PORT_A, 14)
#define DIO_CHANNEL_A15     DIO_CHANNEL(DIO_PORT_A, 15)

/* GPIO B Pins 0 to 15 */
#define DIO_CHANNEL_B0      DIO_CHANNEL(DIO_PORT_B, 0)
#define DIO_CHANNEL_B1      DIO_CHANNEL(DIO_PORT_B, 1)
#define DIO_CHANNEL_B2      DIO_CHANNEL(DIO_PORT_B, 2)
#define DIO_CHANNEL_B3      DIO_CHANNEL(DIO_PORT_B, 3)
#define DIO_CHANNEL_B4      DIO_CHANNEL(DIO_PORT_B, 4)
#define DIO_CHANNEL_B5      DIO_CHANNEL(DIO_PORT_B, 5)
#define DIO_CHANNEL_B6      DIO_CHANNEL(DIO_PORT_B, 6)
#define DIO_CHANNEL_B7      DIO_CHANNEL(DIO_PORT_B, 7)
#define DIO_CHANNEL_B8      DIO_CHANNEL(DIO_PORT_B, 8)
#define DIO_CHANNEL_B9      DIO_CHANNEL(DIO_PORT_B, 9)
#define DIO_CHANNEL_B10     DIO_CHANNEL(DIO_PORT_B, 10)
#define DIO_CHANNEL_B11     DIO_CHANNEL(DIO_PORT_B, 11)
#define DIO_CHANNEL_B12     DIO_CHANNEL(DIO_PORT_B, 12)
#define DIO_CHANNEL_B13     DIO_CHANNEL(DIO_PORT_B, 13)
#define DIO_CHANNEL_B14     DIO_CHANNEL(DIO_PORT_B, 14)
#define DIO_CHANNEL_B15     DIO_CHANNEL(DIO_PORT_B, 15)

/* GPIO C Pins 0 to 15 */
#define DIO_CHANNEL_C0      DIO_CHANNEL(DIO_PORT_C, 0)
#define DIO_CHANNEL_C1      DIO_CHANNEL(DIO_PORT_C, 1)
#define DIO_CHANNEL_C2      DIO_CHANNEL(DIO_PORT_C, 2)
#define DIO_CHANNEL_C3      DIO_CHANNEL(DIO_PORT_C, 3)
#define DIO_CHANNEL_C4      DIO_CHANNEL(DIO_PORT_C, 4)
#define DIO_CHANNEL_C5      DIO_CHANNEL(DIO_PORT_C, 5)
#define DIO_CHANNEL_C6      DIO_CHANNEL(DIO_PORT_C, 6)
#define DIO_CHANNEL_C7      DIO_CHANNEL(DIO_PORT_C, 7)
#define DIO_CHANNEL_C8      DIO_CHANNEL(DIO_PORT_C, 8)
#define DIO_CHANNEL_C9      DIO_CHANNEL(DIO_PORT_C, 9)
#define DIO_CHANNEL_C10     DIO_CHANNEL(DIO_PORT_C, 10)
#define DIO_CHANNEL_C11     DIO_CHANNEL(DIO_PORT_C, 11)
#define DIO_CHANNEL_C12     DIO_CHANNEL(DIO_PORT_C, 12)
#define DIO_CHANNEL_C13     DIO_CHANNEL(DIO_PORT_C, 13)
#define DIO_CHANNEL_C14     DIO_CHANNEL(DIO_PORT_C, 14)
#define DIO_CHANNEL_C15     DIO_CHANNEL(DIO_PORT_C, 15)

/* GPIO D Pins 0 to 15 */
#define DIO_CHANNEL_D0      DIO_CHANNEL(DIO_PORT_D, 0)
#define DIO_CHANNEL_D1      DIO_CHANNEL(DIO_PORT_D, 1)
#define DIO_CHANNEL_D2      DIO_CHANNEL(DIO_PORT_D, 2)
#define DIO_CHANNEL_D3      DIO_CHANNEL(DIO_PORT_D, 3)
#define DIO_CHANNEL_D4      DIO_CHANNEL(DIO_PORT_D, 4)
#define DIO_CHANNEL_D5      DIO_CHANNEL(DIO_PORT_D, 5)
#define DIO_CHANNEL_D6      DIO_CHANNEL(DIO_PORT_D, 6)
#define DIO_CHANNEL_D7      DIO_CHANNEL(DIO_PORT_D, 7)
#define DIO_CHANNEL_D8      DIO_CHANNEL(DIO_PORT_D, 8)
#define DIO_CHANNEL_D9      DIO_CHANNEL(DIO_PORT_D, 9)
#define DIO_CHANNEL_D10     DIO_CHANNEL(DIO_PORT_D, 10)
#define DIO_CHANNEL_D11     DIO_CHANNEL(DIO_PORT_D, 11)
#define DIO_CHANNEL_D12     DIO_CHANNEL(DIO_PORT_D, 12)
#define DIO_CHANNEL_D13     DIO_CHANNEL(DIO_PORT_D, 13)
#define DIO_CHANNEL_D14     DIO_CHANNEL(DIO_PORT_D, 14)
#define DIO_CHANNEL_D15     DIO_CHANNEL(DIO_PORT_D, 15)

/* GPIO E Pins 0 to 15 */
#define DIO_CHANNEL_E0      DIO_CHANNEL(DIO_PORT_E, 0)
#define DIO_CHANNEL_E1      DIO_CHANNEL(DIO_PORT_E, 1)
#define DIO_CHANNEL_E2      DIO_CHANNEL(DIO_PORT_E, 2)
#define DIO_CHANNEL_E3      DIO_CHANNEL(DIO_PORT_E, 3)
#define DIO_CHANNEL_E4      DIO_CHANNEL(DIO_PORT_E, 4)
#define DIO_CHANNEL_E5      DIO_CHANNEL(DIO_PORT_E, 5)
#define DIO_CHANNEL_E6      DIO_CHANNEL(DIO_PORT_E, 6)
#define DIO_CHANNEL_E7      DIO_CHANNEL(DIO_PORT_E, 7)
#define DIO_CHANNEL_E8      DIO_CHANNEL(DIO_PORT_E, 8)
#define DIO_CHANNEL_E9      DIO_CHANNEL(DIO_PORT_E, 9)
#define DIO_CHANNEL_E10     DIO_CHANNEL(DIO_PORT_E, 10)
#define DIO_CHANNEL_E11     DIO_CHANNEL(DIO_PORT_E, 11)
#define DIO_CHANNEL_E12     DIO_CHANNEL(DIO_PORT_E, 12)
#define DIO_CHANNEL_E13     DIO_CHANNEL(DIO_PORT_E, 13)
#define DIO_CHANNEL_E14     DIO_CHANNEL(DIO_PORT_E, 14)
#define DIO_CHANNEL_E15     DIO_CHANNEL(DIO_PORT_E, 15)

/* GPIO F Pins 0 to 15 */
#define DIO_CHANNEL_F0      DIO_CHANNEL(DIO_PORT_F, 0)
#define DIO_CHANNEL_F1      DIO_CHANNEL(DIO_PORT_F, 1)
#define DIO_CHANNEL_F2      DIO_CHANNEL(DIO_PORT_F, 2)
#define DIO_CHANNEL_F3      DIO_CHANNEL(DIO_PORT_F, 3)
#define DIO_CHANNEL_F4      DIO_CHANNEL(DIO_PORT_F, 4)
#define DIO_CHANNEL_F5      DIO_CHANNEL(DIO_PORT_F, 5)
#define DIO_CHANNEL_F6      DIO_CHANNEL(DIO_PORT_F, 6)
#define DIO_CHANNEL_F7      DIO_CHANNEL(DIO_PORT_F, 7)
#define DIO_CHANNEL_F8      DIO_CHANNEL(DIO_PORT_F, 8)
#define DIO_CHANNEL_F9      DIO_CHANNEL(DIO_PORT_F, 9)
#define DIO_CHANNEL_F10     DIO_CHANNEL(DIO_PORT_F, 10)
#define DIO_CHANNEL_F11     DIO_CHANNEL(DIO_PORT_F, 11)
#define DIO_CHANNEL_F12     DIO_CHANNEL(DIO_PORT_F, 12)
#define DIO_CHANNEL_F13     DIO_CHANNEL(DIO_PORT_F, 13)
#define DIO_CHANNEL_F14     DIO_CHANNEL(DIO_PORT_F, 14)
#define DIO_CHANNEL_F15     DIO_CHANNEL(DIO_PORT_F, 15)

/* GPIO G Pins 0 to 15 */
#define DIO_CHANNEL_G0      DIO_CHANNEL(DIO_PORT_G, 0)
#define DIO_CHANNEL_G1      DIO_CHANNEL(DIO_PORT_G, 1)
#define DIO_CHANNEL_G2      DIO_CHANNEL(DIO_PORT_G, 2)
#define DIO_CHANNEL_G3      DIO_CHANNEL(DIO_PORT_G, 3)
#define DIO_CHANNEL_G4      DIO_CHANNEL(DIO_PORT_G, 4)
#define DIO_CHANNEL_G5      DIO_CHANNEL(DIO_PORT_G, 5)
#define DIO_CHANNEL_G6      DIO_CHANNEL(DIO_PORT_G, 6)
#define DIO_CHANNEL_G7      DIO_CHANNEL(DIO_PORT_G, 7)
#define DIO_CHANNEL_G8      DIO_CHANNEL(DIO_PORT_G, 8)
#define DIO_CHANNEL_G9      DIO_CHANNEL(DIO_PORT_G, 9)
#define DIO_CHANNEL_G10     DIO_CHANNEL(DIO_PORT_G, 10)
#define DIO_CHANNEL_G11     DIO_CHANNEL(DIO_PORT_G, 11)
#define DIO_CHANNEL_G12     DIO_CHANNEL(DIO_PORT_G, 12)
#define DIO_CHANNEL_G13     DIO_CHANNEL(DIO_PORT_G, 13)
#define DIO_CHANNEL_G14     DIO_CHANNEL(DIO_PORT_G, 14)
#define DIO_CHANNEL_G15     DIO_CHANNEL(DIO_PORT_G, 15)

/* GPIO H Pins 0 to 1 */
#define DIO_CHANNEL_H0      DIO_CHANNEL(DIO_PORT_H, 0)
#define DIO_CHANNEL_H1      DIO_CHANNEL(DIO_PORT_H, 1)

/**
 * @typedef     Dio_ChannelType
 * @brief       Numeric ID of a DIO channel.
 * @details     Parameters of type Dio_ChannelType contain the numeric ID of a DIO channel
 */
typedef uint8       Dio_ChannelType;

/**
 * @typedef     Dio_PortType
 * @brief       Numeric ID of a DIO port.
 * @details     Parameters of type Dio_PortType contain the numeric ID of a DIO port.
 */
typedef uint16      Dio_PortType;

/**
 * @typedef     Dio_LevelType
 * @brief       These are the possible levels a DIO channel can have (input or output)
 * @details     Dio_LevelType is the type for the possible levels that a DIO channel can have (input or 
 *              output).
 */
typedef uint8       Dio_LevelType;

/*
 * @typedef     Dio_PortLevelType
 * @brief       Dio_PortLevelType is the type for the value of a DIO port
 * @details     If the µC owns ports of different port widths (e.g. 4, 8,16...Bit) Dio_PortLevelType inherits 
 *              the size of the largest port.
 */
typedef uint32      Dio_PortLevelType;

/**
 * @typedef     Dio_ChannelGroupType
 * @brief       Type for the definition of a channel group, which consists of several adjoining channels 
 *              within a port.
 * @details     Dio_ChannelGroupType is the type for the definition of a channel group, which consists of 
 *              several adjoining channels within a port.
 */
typedef struct
{
    uint16 mask;   /* This element mask which defines the positions of the channel group. */
    uint8 offset; /* This element shall be the position of the Channel Group on the port, counted from the LSB.*/
    Dio_PortType port; /* This shall be the port on which the Channel group is defined. */
} Dio_ChannelGroupType;

/*
 ************************************************************************************************************
 * Inline functions
 ************************************************************************************************************
 */

/**
 * @brief       Get the GPIO port pointer from the port index
 * @param       ChannelId: ID of DIO channel
 * @return      The GPIO port pointer
 */
static inline GPIO_TypeDef* DIO_GET_GPIO_PORT(Dio_ChannelType ChannelId)
{
    switch (ChannelId)
    {
        case DIO_PORT_A: return GPIOA;
        case DIO_PORT_B: return GPIOB;
        case DIO_PORT_C: return GPIOC;
        case DIO_PORT_D: return GPIOD;
        case DIO_PORT_E: return GPIOE;
        case DIO_PORT_F: return GPIOF;
        case DIO_PORT_G: return GPIOG;
        case DIO_PORT_H: return GPIOH;
        default: return NULL_PTR; // Invalid port index
    }
}

/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */

/**
 * @brief       Returns the value of the specified DIO channel.
 * @param       ChannelId: ID of DIO channel
 * @return      Dio_LevelType:  value of the specified DIO channel
 *              - STD_HIGH The physical level of the corresponding Pin is STD_HIGH
 *              - STD_LOW The physical level of the corresponding Pin is STD_LOW
 */
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId);

/**
 * @brief       Service to set a level of a channel
 * @param       ChannelId: ID of DIO channel
 * @param       Level: Value to be written 
 * @return      void
 */
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level);

/**
 * @brief       Returns the level of all channels of that port
 * @param       PortId: ID of DIO Port
 * @return      Dio_PortLevelType: Level of all channels of that port
 */
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId);

/**
 * @brief       Service to set a value of the port
 * @param       PortId: ID of DIO Port
 * @param       Level: Value to be written
 * @return      void
 */
void Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level);

/**
 * @brief       This Service reads a subset of the adjoining bits of a port
 * @param       ChannelGroupIdPtr: Pointer to ChannelGroup
 * @return      Dio_PortLevelType: Level of a subset of the adjoining bits of a port
 */
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr);

/**
 * @brief       Service to set a subset of the adjoining bits of a port to a specified level.
 * @param       ChannelGroupIdPtr: Pointer to ChannelGroup
 * @param       Level: Value to be written
 * @return      void
 */
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);

/**
 * @brief       Service to get the version information of this module
 * @param       VersionInfo: Pointer to where to store the version information of this module.
 * @return      void
 */
void Dio_GetVersionInfo (Std_VersionInfoType* VersionInfo);

/**
 * @brief       Service to flip (change from 1 to 0 or from 0 to 1) the level of a channel and return the level of
 *              the channel after flip.
 * @param       ChannelId: ID of DIO channel
 * @return      Dio_LevelType:  value of the specified DIO channel
 *              - STD_HIGH The physical level of the corresponding Pin is STD_HIGH
 *              - STD_LOW The physical level of the corresponding Pin is STD_LOW
 */
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId);


#endif /* DIO_H */