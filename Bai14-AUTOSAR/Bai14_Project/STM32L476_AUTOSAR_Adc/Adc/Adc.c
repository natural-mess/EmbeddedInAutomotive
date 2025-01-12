/**
 * @file        Adc.c
 * @author      Phuc
 * @brief       ADC driver source file in AUTOSAR
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
#include "Adc.h"
#include "Adc_Hw.h"
#include "Adc_Cfg.h"

/*
 ************************************************************************************************************
 * Static variables
 ************************************************************************************************************
 */
static const Adc_ConfigType* Adc_ConfigPtr = NULL_PTR;

/*
 ************************************************************************************************************
 * Function definition
 ************************************************************************************************************
 */
/**
 * @brief       Initializes the ADC hardware units and driver.
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
void Adc_Init (const Adc_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        return;
    }

    /* Configure GPIO for ADC pins based on channel */
    Adc_Hw_SetupGPIO(ConfigPtr);

    /* Initialize parameters for ADC1, ADC2 and ADC3 */
    // switch (ConfigPtr->AdcInstance)
    // {
    // case ADC_INSTANCE_1:
    //     Adc_Hw_Init_ADC1(ConfigPtr);
    //     break;
    
    // case ADC_INSTANCE_2:
    //     Adc_Hw_Init_ADC2(ConfigPtr);
    //     break;

    // case ADC_INSTANCE_3:
    //     Adc_Hw_Init_ADC3(ConfigPtr);
    //     break;

    // default:
    //     break;
    // }

    /* Configure ADC Channels */
    Adc_Hw_SetupChannels(ConfigPtr);

    /* Turn on ADC */
    Adc_Hw_EnableADC(ConfigPtr);

    /* Verify and activiate calibration */
    Adc_Hw_Calibrate(ConfigPtr);

    /* Start conversion by software if needed */
    Adc_Hw_StartSoftwareConversion(ConfigPtr);

    /* Call callback function if configured */
    if (ConfigPtr->InitCallback != NULL_PTR)
    {
        ConfigPtr->InitCallback();
    }
}

/**
 * @brief       Initializes ADC driver with the group specific result buffer start address where the conversion
 *              results will be stored. The application has to ensure that the application buffer, where Data
 *              BufferPtr points to, can hold all the conversion results of the specified group. The initialization
 *              with Adc_SetupResultBuffer is required after reset, before a group conversion can be started.
 * @param       Group: Numeric ID of requested ADC channel group.
 * @param       DataBufferPtr: pointer to result data buffer
 * @return      Std_ReturnType: 
 *              E_OK: result buffer pointer initialized correctly
 *              E_NOT_OK: operation failed or development error occured
 */
Std_ReturnType Adc_SetupResultBuffer (Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    if (DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    // switch (Group)
    // {
    // case ADC_INSTANCE_1:
    //     /* Setup conversion result of ADC1 */
    //     *DataBufferPtr = LL_ADC_REG_ReadConversionData12(ADC1);
    //     break;
    
    // case ADC_INSTANCE_2:
    //     /* Setup conversion result of ADC2 */
    //     *DataBufferPtr = LL_ADC_REG_ReadConversionData12(ADC2);
    //     break;

    // case ADC_INSTANCE_3:
    //     /* Setup conversion result of ADC3 */
    //     *DataBufferPtr = LL_ADC_REG_ReadConversionData12(ADC3);
    //     break;

    // default:
    //     return E_NOT_OK;
    //     break;
    // }

    return E_OK;
}

/**
 * @brief       Returns all ADC HW Units to a state comparable to their power on reset state.
 * @param       void
 * @return      void
 */
void Adc_DeInit (void)
{
    /* Deinitialize hardware config */
    Adc_Hw_Deinit();

    /* Remove config */
    Adc_ConfigPtr = NULL_PTR;
}

/**
 * @brief       Starts the conversion of all channels of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_StartGroupConversion (Adc_GroupType Group)
{
    if (Adc_ConfigPtr != NULL_PTR)
    {
        Adc_Hw_StartConversion(Group);
    }
}

/**
 * @brief       Stops the conversion of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_StopGroupConversion (Adc_GroupType Group)
{
    /* Stop conversion */
    Adc_Hw_StopConversion(Group);
}

/**
 * @brief       Reads the group conversion result of the last completed conversion round of the requested
 *              group and stores the channel values starting at the DataBufferPtr address. The group channel
 *              values are stored in ascending channel number order ( in contrast to the storage layout of the
 *              result buffer if streaming access is configured).
 * @param       Group: Numeric ID of requested ADC channel group.
 * @param       DataBufferPtr: ADC results of all channels of the selected group are stored in the data buffer addressed with the pointer.
 * @return      Std_ReturnType: 
 *              E_OK: results are available and written to the data buffer
 *              E_NOT_OK: no results are available or development error occured
 */
Std_ReturnType Adc_ReadGroup (Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    if (DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }

    /* Read result from ADC */
    return Adc_Hw_ReadGroup(Group, DataBufferPtr);
}

/**
 * @brief       Enables the hardware trigger for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_EnableHardwareTrigger (Adc_GroupType Group)
{
    Adc_Hw_EnableHardwareTrigger(Group);
}

/**
 * @brief       Disables the hardware trigger for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_DisableHardwareTrigger (Adc_GroupType Group)
{
    Adc_Hw_DisableHardwareTrigger(Group);
}

/**
 * @brief       Enables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_EnableGroupNotification (Adc_GroupType Group)
{
    Adc_Hw_EnableNotification(Group);
}

/**
 * @brief       Disables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_DisableGroupNotification (Adc_GroupType Group)
{
    Adc_Hw_DisableNotification(Group);
}

/**
 * @brief       Returns the conversion status of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      Adc_StatusType: Conversion status for the requested group.
 */
Adc_StatusType Adc_GetGroupStatus (Adc_GroupType Group)
{
    return Adc_Hw_GetGroupStatus(Group);
}

/**
 * @brief       Returns the number of valid samples per channel, stored in the result buffer. Reads a pointer,
 *              pointing to a position in the group result buffer. With the pointer position, the results of all group
 *              channels of the last completed conversion round can be accessed. With the pointer and the
 *              return value, all valid group conversion results can be accessed (the user has to take the layout
 *              of the result buffer into account).
 * @param[in]   Group: Numeric ID of requested ADC Channel group.
 * @param[out]  PtrToSamplePtr: Pointer to result buffer pointer.
 * @return      Adc_StreamNumSampleType: Number of valid samples per channel.
 */
Adc_StreamNumSampleType Adc_GetStreamLastPointer ( Adc_GroupType Group, Adc_ValueGroupType** PtrToSamplePtr)
{
    return Adc_Hw_GetStreamLastPointer(Group, PtrToSamplePtr);
}

/**
 * @brief       Returns the version information of this module.
 * @param       versioninfo: Pointer to where to store the version information of this module.
 * @return      void
 */
void Adc_GetVersionInfo (Std_VersionInfoType* versioninfo)
{
    /* Check if VersionInfo is null */
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

