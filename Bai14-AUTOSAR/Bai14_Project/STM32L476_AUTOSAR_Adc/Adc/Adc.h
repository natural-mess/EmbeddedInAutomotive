/**
 * @file        Adc.h
 * @author      Phuc
 * @brief       ADC driver header file in AUTOSAR
 * @version     1.0
 * @date        2025-01-12
 * 
 * @copyright   Copyright (c) 2025
 * 
 */

#ifndef ADC_H
#define ADC_H

/*
 ************************************************************************************************************
 * Includes
 ************************************************************************************************************
 */
#include "Std_Types.h"
#include "stm32l476xx.h"

/*
 ************************************************************************************************************
 * Types and Defines
 ************************************************************************************************************
 */

/**
 * @typedef     Adc_ChannelType
 * @brief       Numeric ID of an ADC channel.
 */
typedef uint8 Adc_ChannelType;

/**
 * @typedef     Adc_GroupType
 * @brief       Numeric ID of an ADC channel group.
 */
typedef uint8 Adc_GroupType;

/**
 * @typedef     Adc_ValueGroupType
 * @brief       Type for reading the converted values of a channel group (raw, without further scaling, alignment
 *              according precompile switch ADC_RESULT_ALIGNMENT).
 */
typedef uint16 Adc_ValueGroupType;

/**
 * @typedef     Adc_PrescaleType
 * @brief       Type of clock prescaler factor. (This is not an API type).
 */
typedef uint32 Adc_PrescaleType;

/**
 * @typedef     Adc_ConversionTimeType
 * @brief       Type of conversion time, i.e. the time during which the sampled analogue value is converted into 
 *              digital representation. (This is not an API type).
 */
typedef uint32 Adc_ConversionTimeType;

/**
 * @typedef     Adc_SamplingTimeType
 * @brief       Type of sampling time, i.e. the time during which the value is sampled, (in clock-cycles). (This is
 *              not an API type).
 */
typedef uint32 Adc_SamplingTimeType;

/**
 * @typedef     Adc_ResolutionType
 * @brief       Type of channel resolution in number of bits. (This is not an API type).
 */
typedef uint8 Adc_ResolutionType;

/**
 * @typedef     Adc_StatusType
 * @brief       Current status of the conversion of the requested ADC Channel group.
 */
typedef enum 
{
    ADC_IDLE = 0x00u,                   /* The conversion of the specified group has not been started */
                                        /* No result is available. */
    ADC_BUSY = 0x01u,                   /* The conversion of the specified group has been started and is still going on. */
                                        /* So far no result is available */
    ADC_COMPLETED = 0x02u,              /* A conversion round (which is not the final one) of the specified group has been finished. */
                                        /* A result is available for all channels of the group. */
    ADC_STREAM_COMPLETED = 0x03u        /* The result buffer is completely filled */
                                        /* For each channel of the selected group the number of samples to be acquired is available */
} Adc_StatusType;

/**
 * @typedef     Adc_TriggerSourceType
 * @brief       Type for configuring the trigger source for an ADC Channel group.
 */
typedef enum 
{
    ADC_TRIGG_SRC_SW = 0x00u,           /* Group is triggered by a software API call. */
    ADC_TRIGG_SRC_HW  = 0x01u           /* Group is triggered by a hardware event. */
} Adc_TriggerSourceType;

/**
 * @typedef     Adc_GroupConvModeType
 * @brief       Type for configuring the conversion mode of an ADC Channel group.
 */
typedef enum 
{
    ADC_CONV_MODE_ONESHOT = 0x00u,      /* Exactly one conversion of each channel in an
                                        ADC channel group is performed after the
                                        configured trigger event. In case of ’group
                                        trigger source software’, a started One-Shot
                                        conversion can be stopped by a software API
                                        call. In case of ’group trigger source
                                        hardware’, a started One-Shot conversion
                                        can be stopped by disabling the trigger event
                                        (if supported by hardware). */
    ADC_CONV_MODE_CONTINUOUS  = 0x01u   /* Repeated conversions of each ADC channel
                                        in an ADC channel group are performed.
                                        'Continuous conversion mode' is only
                                        available for 'group trigger source software'.
                                        A started 'Continuous conversion' can be
                                        stopped by a software API call. */
} Adc_GroupConvModeType;

/**
 * @typedef     Adc_GroupPriorityType
 * @brief       Priority level of the channel. Lowest priority is 0.
 */
typedef uint8 Adc_GroupPriorityType;

/**
 * @typedef     Adc_GroupDefType
 * @brief       Type for assignment of channels to a channel group (this is not an API type).
 */
//typedef struct
//{
//    Adc_ChannelType Channels[16];       /* Contains configured ADC in group */
//    Adc_GroupPriorityType Priority;     /* Priority of group */
//    uint8 NumChannels;                  /* Number of channels in group */
//} Adc_GroupDefType;

/**
 * @typedef     Adc_StreamNumSampleType
 * @brief       Type for configuring the number of group conversions in streaming access mode (in single access mode, parameter is 1).
 */
typedef uint16 Adc_StreamNumSampleType;

/**
 * @typedef     Adc_StreamBufferModeType
 * @brief       Type for configuring the streaming access mode buffer type.
 */
typedef enum 
{
    ADC_STREAM_BUFFER_LINEAR = 0x00u,      /* The ADC Driver stops the conversion as soon
                                            as the stream buffer is full (number of
                                            samples reached). */
    ADC_STREAM_BUFFER_CIRCULAR  = 0x01u,   /* The ADC Driver continues the conversion
                                            even if the stream buffer is full (number of
                                            samples reached) by wrapping around the
                                            stream buffer itself. */
} Adc_StreamBufferModeType;

/**
 * @typedef     Adc_GroupAccessModeType
 * @brief       Type for configuring the access mode to group conversion results.
 */
typedef enum 
{
    ADC_ACCESS_MODE_SINGLE = 0x00u,         /* Single value access mode. */
    ADC_ACCESS_MODE_STREAMING  = 0x01u,     /* Streaming access mode. */
} Adc_GroupAccessModeType;

/**
 * @typedef     Adc_HwTriggerSignalType
 * @brief       Type for configuring on which edge of the hardware trigger signal the driver should react, i.e. start
 *              the conversion (only if supported by the ADC hardware).
 */
typedef enum 
{
    ADC_HW_TRIG_RISING_EDGE  = 0x00u,       /* React on the rising edge of the hardware trigger signal (only if supported by the ADC
                                            hardware). */
    ADC_HW_TRIG_FALLING_EDGE = 0x01u,       /* React on the falling edge of the hardware trigger signal (only if supported by the ADC
                                            hardware). */
    ADC_HW_TRIG_BOTH_EDGES = 0x02u          /* React on both edges of the hardware trigger signal (only if supported by the ADC
                                            hardware). */
} Adc_HwTriggerSignalType;

/**
 * @typedef     Adc_HwTriggerTimerType
 * @brief       Type for the reload value of the ADC module embedded timer (only if supported by the ADC hardware).
 */
typedef uint16 Adc_HwTriggerTimerType;

/**
 * @typedef     Adc_PriorityImplementationType
 * @brief       Type for configuring the prioritization mechanism.
 */
typedef enum 
{
    ADC_PRIORITY_NONE = 0x00u,              /* priority mechanism is not available */
    ADC_PRIORITY_HW = 0x01u,                /* Hardware priority mechanism is available only */
    ADC_PRIORITY_HW_SW = 0x02u              /* Hardware and software priority mechanism is available */
} Adc_PriorityImplementationType;

/**
 * @typedef     Adc_GroupReplacementType
 * @brief       Replacement mechanism, which is used on ADC group level, if a group conversion is interrupted by a group which has a higher priority.
 */
typedef enum 
{
    ADC_GROUP_REPL_ABORT_RESTART = 0x00u,   /*Abort/Restart mechanism is used on group
                                            level, if a group is interrupted by a higher
                                            priority group. The complete conversion
                                            round of the interrupted group (all group
                                            channels)is restarted after the higher priority
                                            group conversion is finished. If the group is
                                            configured in streaming access mode, only
                                            the results of the interrupted conversion
                                            round are discarded. Results of previous
                                            conversion rounds which are already written
                                            to the result buffer are not affected. */
    ADC_GROUP_REPL_SUSPEND_RESUME = 0x01u,  /* Suspend/Resume mechanism is used on
                                            group level, if a group is interrupted by a
                                            higher priority group. The conversion round of
                                            the interrupted group is completed after the
                                            higher priority group conversion is finished.
                                            Results of previous conversion rounds which
                                            are already written to the result buffer are not
                                            affected. */
} Adc_GroupReplacementType;

/**
 * @typedef     Adc_ChannelRangeSelectType
 * @brief       In case of active limit checking: defines which conversion values are taken into account related to
 *              the boardes defineed with AdcChannelLowLimit and AdcChannelHighLimit.
 */
typedef enum 
{
    ADC_RANGE_UNDER_LOW = 0x00u,            /* Range below low limit - low limit value included */
    ADC_RANGE_BETWEEN = 0x01u,              /* Range between low limit and high limit - high limit value included */
    ADC_RANGE_OVER_HIGH = 0x02u,            /* Range above high limit */
    ADC_RANGE_ALWAYS = 0x03u,               /* Complete range - independent from channel limit settings */
    ADC_RANGE_NOT_UNDER_LOW = 0x04u,        /* Range above low limit */
    ADC_RANGE_NOT_BETWEEN = 0x05u,          /* Range above high limit or below low limit - low limit value included */
    ADC_RANGE_NOT_OVER_HIGH = 0x06          /* Range below high limit - high limit value included */
} Adc_ChannelRangeSelectType;

/**
 * @typedef     Adc_ResultAlignmentType
 * @brief       Type for alignment of ADC raw results in ADC result buffer (left/right alignment).
 */
typedef enum 
{
    ADC_ALIGN_LEFT = 0x00u,                 /* left alignment */
    ADC_ALIGN_RIGHT = 0x01u,                /* right alignment */
} Adc_ResultAlignmentType;

/**
 * @typedef     Adc_GroupDefType
 * @brief       Data structure containing the set of configuration parameters required for initializing the ADC Driver and ADC HW Unit(s).
 */
typedef struct 
{
    Adc_ChannelType Channels[16];
    uint8 NumChannels;
    ADC_TypeDef* AdcInstance;
    uint32 TriggerSource;
    uint8 Priority;
} Adc_GroupDefType;

/**
 * @typedef     Adc_ConfigType
 * @brief       Data structure containing the set of configuration parameters required for initializing the ADC Driver and ADC HW Unit(s).
 */
typedef struct 
{
    uint32 ClockPrescaler;
    uint32 Resolution;
    uint8 NumGroups;
    const Adc_GroupDefType* Groups;
    void (*InitCallback)(void);
} Adc_ConfigType;



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
 * @brief       Initializes the ADC hardware units and driver.
 * @param       ConfigPtr: Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).
 * @return      void
 */
void Adc_Init (const Adc_ConfigType* ConfigPtr);

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
Std_ReturnType Adc_SetupResultBuffer (Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/**
 * @brief       Returns all ADC HW Units to a state comparable to their power on reset state.
 * @param       void
 * @return      void
 */
void Adc_DeInit (void);

/**
 * @brief       Starts the conversion of all channels of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_StartGroupConversion (Adc_GroupType Group);

/**
 * @brief       Stops the conversion of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_StopGroupConversion (Adc_GroupType Group);

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
Std_ReturnType Adc_ReadGroup (Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/**
 * @brief       Enables the hardware trigger for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_EnableHardwareTrigger (Adc_GroupType Group);

/**
 * @brief       Disables the hardware trigger for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_DisableHardwareTrigger (Adc_GroupType Group);

/**
 * @brief       Enables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_EnableGroupNotification (Adc_GroupType Group);

/**
 * @brief       Disables the notification mechanism for the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      void
 */
void Adc_DisableGroupNotification (Adc_GroupType Group);

/**
 * @brief       Returns the conversion status of the requested ADC Channel group.
 * @param       Group: Numeric ID of requested ADC Channel group.
 * @return      Adc_StatusType: Conversion status for the requested group.
 */
Adc_StatusType Adc_GetGroupStatus (Adc_GroupType Group);

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
Adc_StreamNumSampleType Adc_GetStreamLastPointer ( Adc_GroupType Group, Adc_ValueGroupType** PtrToSamplePtr);

/**
 * @brief       Returns the version information of this module.
 * @param[out]  versioninfo: Pointer to where to store the version information of this module.
 * @return      void
 */
void Adc_GetVersionInfo (Std_VersionInfoType* versioninfo);

#endif /* ADC_H */