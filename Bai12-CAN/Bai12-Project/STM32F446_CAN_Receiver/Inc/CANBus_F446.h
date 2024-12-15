#ifndef __CANBUS_F446_H
#define __CANBUS_F446_H

#include "stdint.h"

#define CAN_AF 0x09

typedef struct CANBusTxFrame
{
	uint32_t identifier;
	uint8_t length;
	uint8_t data[8];
}CANBusTxFrameDef;

typedef struct CANBusRxFrame
{
	uint32_t identifier;
	uint8_t length;
	uint8_t data[8];
}CANBusRxFrameDef;

/*
 * @brief This function will initialize the CANBus pins (PB8 and PB9) .
 * @return nothing.
 * */
void CANBus_F446_Pins_Init(void);

/*
 * @brief This function will initialize the CANBus.
 * @return nothing.
 * @see source file for configuration parameters
 * */
void CANBus_F446_Init(void);

/*
 * @brief This function will configure the filter of CANBus.
 * @return nothing.
 * @see source file for filter parameters
 * */
void CANBus_F446_Filter_Configuration(void);

/*
 * @brief This function will start the CANBus Peripheral.
 * @return nothing.
 * */
void CANBus_F446_Start(void);

/*
 * @brief This function will transmit data over CANBus.
 * @param pointer to CANBusTxFrame data structure.
 * @return nothing.
 * */
void CANBus_F446_SendMessage(CANBusTxFrameDef *TXFrame);


/*
 * @brief This function will receive data over CANBus.
 * @param pointer to CANBusRxFrame data structure.
 * @return nothing.
 * */
void CANBus_F446_ReceiveMessage(CANBusRxFrameDef *RXFrame);

#endif /* __CANBUS_F446_H */