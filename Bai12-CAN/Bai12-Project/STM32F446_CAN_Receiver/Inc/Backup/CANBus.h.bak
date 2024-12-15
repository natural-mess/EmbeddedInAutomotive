/*
 * CANBus.h
 *
 *  Created on: Dec 29, 2023
 *      Author: hussamaldean
 */

#ifndef CANBUS_H_
#define CANBUS_H_

#include "stdint.h"


/*
 * @brief This data structure holds the CANBus
 * message to be send.
 * */

typedef struct CANBusTxFrame
{
	uint32_t identifier; /*@brief The identifier of the CANBus Message*/
	uint8_t length;      /*@brief The length of the CANBus Message*/
	uint8_t data[8];     /*@brief The buffer which hold the data to be sent*/


}CANBusTxFrameDef;


/*
 * @brief This data structure holds the CANBus
 * message to be received.
 * */

typedef struct CANBusRxFrame
{
	uint32_t identifier; /*@brief The identifier of the CANBus Message*/
	uint8_t length;      /*@brief The length of the CANBus Message*/
	uint8_t data[8];     /*@brief The buffer which hold the data to be received*/

}CANBusRxFrameDef;


/*
 * @brief This function will initialize the CANBus pins (PB8 and PB9) .
 * @return nothing.
 * */
void CANBus_Pins_Init(void);


/*
 * @brief This function will initialize the CANBus.
 * @return nothing.
 * @see source file for configuration parameters
 * */
void CANBus_Init(void);

/*
 * @brief This function will start the CANBus Peripheral.
 * @return nothing.
 * */
void CANBus_Start(void);


/*
 * @brief This function will configure the filter of CANBus.
 * @return nothing.
 * @see source file for filter parameters
 * */
void Filter_Configuration(void);


/*
 * @brief This function will transmit data over CANBus.
 * @param pointer to CANBusTxFrame data structure.
 * @return nothing.
 * */
void CANBus_SendMessage(CANBusTxFrameDef *TXFrame);


/*
 * @brief This function will receive data over CANBus.
 * @param pointer to CANBusRxFrame data structure.
 * @return nothing.
 * */
void CANBus_ReceiveMessage(CANBusRxFrameDef *RXFrame);



#endif /* CANBUS_H_ */
