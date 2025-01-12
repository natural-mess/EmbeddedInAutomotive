/**
 * @file        Adc_Cfg.h
 * @author      Phuc
 * @brief       Configuration of ADC
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef ADC_CFG_H
#define ADC_CFG_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Adc.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */
/* Define ADC channels */
#define ADC_CHANNEL_0 0u
#define ADC_CHANNEL_1 1u
#define ADC_CHANNEL_2 2u
#define ADC_CHANNEL_4 4u
#define ADC_CHANNEL_7 7u
#define ADC_CHANNEL_9 9u

/* Define ADC groups */
#define ADC_GROUP_0 0u
#define ADC_GROUP_1 1u

/* ADC Channel Configuration */
const Adc_ChannelType AdcChannelConfig[] = {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_4,
    ADC_CHANNEL_7,
    ADC_CHANNEL_9
};

/* ADC Group Configuration */
const Adc_GroupDefType AdcGroupConfig[] = 
{
    /* Group 0 */
    {
        .Channels = {ADC_CHANNEL_4, ADC_CHANNEL_7, ADC_CHANNEL_9},
        .NumChannels = 3,
        .AdcInstance = ADC1,
        .TriggerSource = ADC_TRIGG_SRC_SW,
        .Priority = 0
    },
    /* Group 1 */
    {
        .Channels = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2},
        .NumChannels = 3,
        .AdcInstance = ADC2,
        .TriggerSource = ADC_TRIGG_SRC_SW,
        .Priority = 1
    }
};

/* ADC Configuration */
const Adc_ConfigType AdcConfig = 
{
    .ClockPrescaler = LL_ADC_CLOCK_SYNC_PCLK_DIV4,
    .Resolution = LL_ADC_RESOLUTION_12B,
    .NumGroups = 2,
    .Groups = AdcGroupConfig,
    .InitCallback = NULL_PTR
};

#endif /* ADC_CFG_H */
