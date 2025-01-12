/**
 * @file        Dio.c
 * @author      Phuc
 * @brief       Dio driver source file in AUTOSAR
 * @version     1.0
 * @date        2024-12-29
 * 
 * @copyright   Copyright (c) 2024
 * 
 */

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Dio.h"

/*
 ************************************************************************************************************
 * Function definition
 ************************************************************************************************************
 */
/**
 * @brief       Returns the value of the specified DIO channel.
 * @param       ChannelId: ID of DIO channel
 * @return      Dio_LevelType:  value of the specified DIO channel
 *              - STD_HIGH The physical level of the corresponding Pin is STD_HIGH
 *              - STD_LOW The physical level of the corresponding Pin is STD_LOW
 */
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId)
{
    uint8_t GPIOPort_Index = DIO_GET_PORT(ChannelId);
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(GPIOPort_Index);

    if (GPIOPort == NULL_PTR)
    {
        return STD_LOW;
    }

    uint8_t GPIOPin = DIO_GET_PIN(ChannelId);

    if (LL_GPIO_IsInputPinSet(GPIOPort, GPIOPin) == 1u)
    {
        return STD_HIGH;
    }
    else
    {
        return STD_LOW;
    }
}

/**
 * @brief       Service to set a level of a channel
 * @param       ChannelId: ID of DIO channel
 * @param       Level: Value to be written 
 * @return      void
 */
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    int8_t GPIOPort_Index = DIO_GET_PORT(ChannelId);
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(GPIOPort_Index);

    if (GPIOPort == NULL_PTR)
    {
        return;
    }

    uint8_t GPIOPin = DIO_GET_PIN(ChannelId);

    if(Level == STD_HIGH)
    {
        LL_GPIO_SetOutputPin(GPIOPort, GPIOPin);
    }
    else
    {
        LL_GPIO_ResetOutputPin(GPIOPort, GPIOPin);
    }
}

/**
 * @brief       Returns the level of all channels of that port
 * @param       PortId: ID of DIO Port
 * @return      Dio_PortLevelType: Level of all channels of that port
 */
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId)
{
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(PortId);

    if (GPIOPort == NULL_PTR)
    {
        return 0;
    }

    return ((Dio_PortLevelType)LL_GPIO_ReadInputPort(GPIOPort));
}

/**
 * @brief       Service to set a value of the port
 * @param       PortId: ID of DIO Port
 * @param       Level: Value to be written
 * @return      void
 */
void Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(PortId);

    if (GPIOPort == NULL_PTR)
    {
        return;
    }

    LL_GPIO_WriteOutputPort(GPIOPort, Level);
}

/**
 * @brief       This Service reads a subset of the adjoining bits of a port
 * @param       ChannelGroupIdPtr: Pointer to ChannelGroup
 * @return      Dio_PortLevelType: Level of a subset of the adjoining bits of a port
 */
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(ChannelGroupIdPtr->port);

    if (GPIOPort == NULL_PTR)
    {
        return 0;
    }

    uint32_t portData = LL_GPIO_ReadInputPort(GPIOPort);

    return (Dio_PortLevelType) (((uint16)portData & ChannelGroupIdPtr->mask) >> (uint16)ChannelGroupIdPtr->offset);
}

/**
 * @brief       Service to set a subset of the adjoining bits of a port to a specified level.
 * @param       ChannelGroupIdPtr: Pointer to ChannelGroup
 * @param       Level: Value to be written
 * @return      void
 */
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level)
{
    GPIO_TypeDef* GPIOPort = DIO_GET_GPIO_PORT(ChannelGroupIdPtr->port);

    if (GPIOPort == NULL_PTR)
    {
        return;
    }

    uint32_t portData = LL_GPIO_ReadInputPort(GPIOPort);

    portData &= ~(ChannelGroupIdPtr->mask); /* Reset bits corresponding to mask */
    portData |= ((Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask); /* Write new value to channel group */

    LL_GPIO_WriteOutputPort(GPIOPort, portData);
}

/**
 * @brief       Service to get the version information of this module
 * @param       VersionInfo: Pointer to where to store the version information of this module.
 * @return      void
 */
void Dio_GetVersionInfo (Std_VersionInfoType* VersionInfo)
{
    VersionInfo->vendorID = 0x1234;
    VersionInfo->moduleID = 0x5678;
    VersionInfo->sw_major_version = 1;
    VersionInfo->sw_minor_version = 0;
    VersionInfo->sw_patch_version = 0;
}

/**
 * @brief       Service to flip (change from 1 to 0 or from 0 to 1) the level of a channel and return the level of
 *              the channel after flip.
 * @param       ChannelId: ID of DIO channel
 * @return      Dio_LevelType:  value of the specified DIO channel
 *              - STD_HIGH The physical level of the corresponding Pin is STD_HIGH
 *              - STD_LOW The physical level of the corresponding Pin is STD_LOW
 */
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId)
{
    Dio_LevelType currentLevel = Dio_ReadChannel(ChannelId);

    if (currentLevel == STD_HIGH)
    {
        Dio_WriteChannel(ChannelId, STD_LOW);
        return STD_LOW;
    }
    else
    {
        Dio_WriteChannel(ChannelId, STD_HIGH);
        return STD_HIGH;
    }
}

