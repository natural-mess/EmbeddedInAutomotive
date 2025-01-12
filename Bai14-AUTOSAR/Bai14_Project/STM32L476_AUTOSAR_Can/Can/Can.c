/**
 * @file        Can.c
 * @author      Phuc
 * @brief       CAN driver source file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Can.h"

/*
 ************************************************************************************************************
 * Function definition
 ************************************************************************************************************
 */
/**
 * @brief       This function initializes the module.
 * @param       Config: Pointer to driver configuration.
 * @retval      void
 */
void Can_Init(const Can_ConfigType* Config)
{

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
 * @brief       This function de-initializes the module.
 * @param       void
 * @retval      void
 */
void Can_DeInit(void)
{
    
}

