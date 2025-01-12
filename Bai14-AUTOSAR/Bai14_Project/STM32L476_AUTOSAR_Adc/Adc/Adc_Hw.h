/**
 * @file        Adc_Hw.h
 * @author      Phuc
 * @brief       ADC Configuration setup for STM32L476
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef ADC_HW_H
#define ADC_HW_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_gpio.h"
#include "Adc.h"
#include "Adc_Cfg.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */


/*
 ************************************************************************************************************
 * Inline functions
 ************************************************************************************************************
 */
/**
 * @brief       Function to initialize the ADC hardware 
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_Init(const Adc_ConfigType* ConfigPtr)
{
    /* Enable ADC and GPIO clocks */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    /* Configure GPIO for ADC channels */
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG);

    /* Configure ADC for each group */
    for (uint8_t i = 0; i < ConfigPtr->NumGroups; i++)
    {
        ADC_TypeDef* adcInstance = ConfigPtr->Groups[i].AdcInstance;

        LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(adcInstance), ConfigPtr->ClockPrescaler);
        LL_ADC_SetResolution(adcInstance, ConfigPtr->Resolution);
        LL_ADC_SetDataAlignment(adcInstance, LL_ADC_DATA_ALIGN_RIGHT);
        LL_ADC_SetLowPowerMode(adcInstance, LL_ADC_LP_MODE_NONE);

        for (uint8_t j = 0; j < ConfigPtr->Groups[i].NumChannels; j++)
        {
            LL_ADC_REG_SetSequencerRanks(adcInstance, j + 1, ConfigPtr->Groups[i].Channels[j]);
            LL_ADC_SetChannelSamplingTime(adcInstance, ConfigPtr->Groups[i].Channels[j], LL_ADC_SAMPLINGTIME_2CYCLES_5);
        }

        /* Enable ADC */
        LL_ADC_Enable(adcInstance);
    }
}

/**
 * @brief       Setup GPIO
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_SetupGPIO(const Adc_ConfigType* ConfigPtr)
{
    /* Configure GPIO for ADC channels */
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG);
}

/**
 * @brief       ADC1 init
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_Init_ADC1(const Adc_ConfigType* ConfigPtr)
{
    /* Initialize ADC1 specific settings */
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), ConfigPtr->ClockPrescaler);
    LL_ADC_SetResolution(ADC1, ConfigPtr->Resolution);
    LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_MODE_NONE);
}

/**
 * @brief       ADC2 init
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_Init_ADC2(const Adc_ConfigType* ConfigPtr)
{
    /* Initialize ADC2 specific settings */
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC2), ConfigPtr->ClockPrescaler);
    LL_ADC_SetResolution(ADC2, ConfigPtr->Resolution);
    LL_ADC_SetDataAlignment(ADC2, LL_ADC_DATA_ALIGN_RIGHT);
    LL_ADC_SetLowPowerMode(ADC2, LL_ADC_LP_MODE_NONE);
}

/**
 * @brief       ADC3 init
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_Init_ADC3(const Adc_ConfigType* ConfigPtr)
{
    /* Initialize ADC3 specific settings */
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC3), ConfigPtr->ClockPrescaler);
    LL_ADC_SetResolution(ADC3, ConfigPtr->Resolution);
    LL_ADC_SetDataAlignment(ADC3, LL_ADC_DATA_ALIGN_RIGHT);
    LL_ADC_SetLowPowerMode(ADC3, LL_ADC_LP_MODE_NONE);
}

/**
 * @brief       Setup channels
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_SetupChannels(const Adc_ConfigType* ConfigPtr)
{
    /* Configure ADC channels for each group */
    for (uint8_t i = 0; i < ConfigPtr->NumGroups; i++)
    {
        ADC_TypeDef* adcInstance = ConfigPtr->Groups[i].AdcInstance;

        for (uint8_t j = 0; j < ConfigPtr->Groups[i].NumChannels; j++)
        {
            LL_ADC_REG_SetSequencerRanks(adcInstance, j + 1, ConfigPtr->Groups[i].Channels[j]);
            LL_ADC_SetChannelSamplingTime(adcInstance, ConfigPtr->Groups[i].Channels[j], LL_ADC_SAMPLINGTIME_2CYCLES_5);
        }
    }
}

/**
 * @brief       Enable ADC
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_EnableADC(const Adc_ConfigType* ConfigPtr)
{
    /* Enable ADC for each group */
    for (uint8_t i = 0; i < ConfigPtr->NumGroups; i++)
    {
        LL_ADC_Enable(ConfigPtr->Groups[i].AdcInstance);
    }
}

/**
 * @brief       Calibrate ADC
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_Calibrate(const Adc_ConfigType* ConfigPtr)
{
    /* Calibrate ADC for each group */
    for (uint8_t i = 0; i < ConfigPtr->NumGroups; i++)
    {
        ADC_TypeDef* adcInstance = ConfigPtr->Groups[i].AdcInstance;

        LL_ADC_StartCalibration(adcInstance, LL_ADC_SINGLE_ENDED);
        while (LL_ADC_IsCalibrationOnGoing(adcInstance));
    }
}

/**
 * @brief       Start ADC Conversion
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
inline static void Adc_Hw_StartSoftwareConversion(const Adc_ConfigType* ConfigPtr)
{
    /* Start software conversion for each group */
    for (uint8_t i = 0; i < ConfigPtr->NumGroups; i++)
    {
        LL_ADC_REG_StartConversion(ConfigPtr->Groups[i].AdcInstance);
    }
}

/**
 * @brief       Deinitialize ADC hardware
 * @param       void
 * @return      void
 */
inline static void Adc_Hw_Deinit(void)
{
    LL_ADC_Disable(ADC1);
    LL_ADC_Disable(ADC2);
    LL_ADC_Disable(ADC3);
}

/**
 * @brief       Start conversion for the specified group
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_StartConversion(Adc_GroupType Group)
{
    LL_ADC_REG_StartConversion(AdcGroupConfig[Group].AdcInstance);
}

/**
 * @brief       Stop conversion for the specified group
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_StopConversion(Adc_GroupType Group)
{
    LL_ADC_REG_StopConversion(AdcGroupConfig[Group].AdcInstance);
}

/**
 * @brief       Read conversion result for the specified group
 * @param       Group: Numeric ID of requested ADC channel group.
 * @param       DataBufferPtr: pointer to result data buffer
 * @return      void
 */
inline static Std_ReturnType Adc_Hw_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    for (uint8_t i = 0; i < AdcGroupConfig[Group].NumChannels; i++)
    {
        DataBufferPtr[i] = LL_ADC_REG_ReadConversionData12(AdcGroupConfig[Group].AdcInstance);
    }
    return E_OK;
}

/**
 * @brief       Enable hardware trigger for the specified group
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_EnableHardwareTrigger(Adc_GroupType Group)
{
    LL_ADC_REG_SetTriggerSource(AdcGroupConfig[Group].AdcInstance, AdcGroupConfig[Group].TriggerSource);
}

/**
 * @brief       Disable hardware trigger for the specified group
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_DisableHardwareTrigger(Adc_GroupType Group)
{
    LL_ADC_REG_SetTriggerSource(AdcGroupConfig[Group].AdcInstance, LL_ADC_REG_TRIG_SOFTWARE);
}

/**
 * @brief       Enables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_EnableNotification(Adc_GroupType Group)
{

}

/**
 * @brief       Disables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static void Adc_Hw_DisableNotification(Adc_GroupType Group)
{

}

/**
 * @brief       Returns the conversion status of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC channel group.
 * @return      void
 */
inline static Adc_StatusType Adc_Hw_GetGroupStatus(Adc_GroupType Group)
{
	return ADC_IDLE;
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
inline static Adc_StreamNumSampleType Adc_Hw_GetStreamLastPointer( Adc_GroupType Group, Adc_ValueGroupType** PtrToSamplePtr)
{
    return 0;
}



/*
 ************************************************************************************************************
 * Functions declaration
 ************************************************************************************************************
 */


#endif /* ADC_HW_H */
