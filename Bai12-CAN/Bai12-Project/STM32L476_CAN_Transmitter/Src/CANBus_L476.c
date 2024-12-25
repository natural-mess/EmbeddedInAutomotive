#include "stm32l476xx.h"
#include "CANBus_L476.h"

void CANBus_L476_Pins_Init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	/* Configure PB8 as Pull-Up */
  //GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk);  // Clear PUPD bits for PB8
  //GPIOB->PUPDR |= (1U << GPIO_PUPDR_PUPD8_Pos);  // Set PULL-UP (01)
	
	//GPIOB->MODER &= ~GPIO_MODER_MODE8; // MODER8[1:0] = 00 (Input mode)
	//GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD8; // PUPD8[1:0] = 00 (No pull-up, no pull-down)
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
	
	GPIOB->MODER |=   GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	//GPIOB->MODER &=~ (GPIO_MODER_MODE8_0|GPIO_MODER_MODE9_0);
	
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8_Msk | GPIO_OSPEEDR_OSPEED9_Msk);
	
	//GPIOB->MODER |=   GPIO_MODER_MODE9_1;
	//GPIOB->MODER &=~ GPIO_MODER_MODE9_0;
	
	/*Configure PB8 and PB9 to use CAN Bus AF*/
	//GPIOB->AFR[1] |= (CAN_AF<<GPIO_AFRH_AFSEL8_Pos)|(CAN_AF<<GPIO_AFRH_AFSEL9_Pos);
	//GPIOB->AFR[1] |= CAN_AF<<GPIO_AFRH_AFSEL9_Pos;
	GPIOB->AFR[1] &= ~((0xF << GPIO_AFRH_AFSEL8_Pos) | (0xF << GPIO_AFRH_AFSEL9_Pos));
	GPIOB->AFR[1] |= (CAN_AF << GPIO_AFRH_AFSEL8_Pos) | (CAN_AF << GPIO_AFRH_AFSEL9_Pos);
}

void CANBus_L476_Init(void)
{
	/*Enable Clock access to CAN1*/
	RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;
	
	/* Enter Initialization mode*/
  CAN1->MCR |= CAN_MCR_INRQ;
  /*Wait until CANBus peripheral is in initialization mode*/
  while (!(CAN1->MSR & CAN_MSR_INAK)){}
	
	// Enable CAN1 peripheral
  CAN1->MCR &= ~CAN_MCR_SLEEP;
  while ((CAN1->MSR & CAN_MSR_SLAK)!=0U){}
		
	/*Configure the timing with the following parameters
	 * Normal mode.
	 * Loop back mode is disabled.
	 * Resynchronization jump width to 1 (value - 1).
	 * Prescale of 5. (value - 1)
	 * Time quanta segment 1 is 1 (value - 1)
	 * Time quanta segment 2 is 15 (value - 1)
	 * Baud is 500kbps
	 * */
	/*Reset the non-zero initial values*/
  //CAN1->BTR &=~ (CAN_BTR_TS1_Msk|CAN_BTR_TS2_Msk|CAN_BTR_SJW_Msk);
  //CAN1->BTR  =  (14<<CAN_BTR_TS1_Pos)|(1<<CAN_BTR_TS2_Pos)|(4U<<CAN_BTR_BRP_Pos);
	
	CAN1->MCR &= ~CAN_MCR_TTCM;
	CAN1->MCR &= ~CAN_MCR_ABOM;
	CAN1->MCR |= CAN_MCR_AWUM;  // Enable Automatic Wake-Up
  CAN1->MCR |= CAN_MCR_NART; // Disable Automatic Retransmission
	CAN1->MCR &= ~CAN_MCR_RFLM;
  CAN1->MCR &= ~CAN_MCR_TXFP;  // Disable Transmit FIFO Priority
		
	//CAN1->BTR  =  (14<<CAN_BTR_TS1_Pos)|(1<<CAN_BTR_TS2_Pos)|(4U<<CAN_BTR_BRP_Pos)|(CAN_BTR_LBKM); // Enable Loopback Mode
	//CAN1->BTR = (CAN_BTR_LBKM)|(CAN_SJW_1TQ)|(CAN_BS1_15TQ)|(CAN_BS2_2TQ)|4U;
	CAN1->BTR = (CAN_MODE_NORMAL)|(CAN_SJW_1TQ)|(CAN_BS1_15TQ)|(CAN_BS2_2TQ)|4U;
}

//void CANBus_L476_Filter_Configuration(void)
//{
//	/* Set the filter initialization mode*/
//    CAN1->FMR |= CAN_FMR_FINIT;

//    /* Deactivate all filter banks */
//    //CAN1->FA1R &= ~(CAN_FA1R_FACT0);
//		
//		uint32_t filternbrbitpos = (uint32_t)1 << (0 & 0x1FU);
//		CAN1->FA1R &= ~filternbrbitpos;
//		
//    /* Configure filter 0 to accept all IDs (no filtering) */
//    //CAN1->FS1R |= CAN_FS1R_FSC0;  // 32-bit scale
//    //CAN1->FM1R &= ~CAN_FM1R_FBM0; // Identifier mask mode (0: Mask mode)
//    //CAN1->sFilterRegister[0].FR1 = 0x00000000; // Accept all IDs
//    //CAN1->sFilterRegister[0].FR2 = 0x00000000; // Mask all bits
//		
//		CAN1->FS1R |= filternbrbitpos;
//		//CAN1->sFilterRegister[0].FR1 = ((0x0000FFFFU & (uint32_t)0x0000) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);
//		//CAN1->sFilterRegister[0].FR2 = ((0x0000FFFFU & (uint32_t)0x0000) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);

//		
//		CAN1->FM1R &= ~filternbrbitpos;
//		CAN1->FFA1R &= ~filternbrbitpos;
//		CAN1->FA1R |= filternbrbitpos;

//    /* Assign filter 0 to FIFO0 */
//    //CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

//    /* Activate filter 0 */
//    //CAN1->FA1R |= CAN_FA1R_FACT0;

//    /* Exit filter initialization mode */
//    CAN1->FMR &= ~CAN_FMR_FINIT;
//}

void CANBus_L476_Filter_Configuration(void)
{
    /* Enable filter initialization mode */
    CAN1->FMR |= CAN_FMR_FINIT;

    /* Configure Filter Bank 0 */
    /* Deactivate filter bank 0 */
    CAN1->FA1R &= ~CAN_FA1R_FACT0;

    /* Set filter scale to 32-bit */
    CAN1->FS1R |= CAN_FS1R_FSC0;

    /* Set filter mode to Identifier List Mode */
    CAN1->FM1R &= ~CAN_FM1R_FBM0; // Set to 0 for identifier mask mode

		// 0x1AD: 0001 1010 1101
	  // 0x4A3: 0100 1010 0011
		// 0x5A1: 0101 1010 0001
		// mask : xxxx 1010 xxx1 -> 0x0F1
		// filter 0000 1010 0001 -> 0x0A1
		CAN1->sFilterRegister[0].FR1 = ((0x0000FFFFU & (uint32_t)0x00A1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);
		CAN1->sFilterRegister[0].FR2 = ((0x0000FFFFU & (uint32_t)0x00F1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);

    /* Assign filter bank 0 to FIFO0 */
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    /* Activate filter bank 0 */
    CAN1->FA1R |= CAN_FA1R_FACT0;

    /* Exit filter initialization mode */
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

void CANBus_L476_Start(void)
{
	// Leave Initialization mode
  CAN1->MCR &= ~CAN_MCR_INRQ;
  while (CAN1->MSR & CAN_MSR_INAK);
}

void CANBus_L476_SendMessage(CANBusTxFrameDef *TXFrame)
{
  uint32_t timeout = 100000;
	
	// Wait until the transmit mailbox is empty
  while ((CAN1->TSR & CAN_TSR_TME0) == 0U && timeout--){}
  if (timeout == 0) {
    // Mailbox không kh? d?ng, x? lý l?i
    return;
  }

	// Clear all previous settings in TIR
  CAN1->sTxMailBox[0].TIR = 0;
	
	// Set the standard identifier
  CAN1->sTxMailBox[0].TIR |= (TXFrame->identifier & 0x7FF) << 21;
	
	// Clear the IDE bit to ensure Standard ID is used
  CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE; // IDE = 0 (Standard ID)
		
  // Configure data length
	CAN1->sTxMailBox[0].TDTR = 0; // Clear TDTR
  CAN1->sTxMailBox[0].TDTR |= (TXFrame->length & 0x0F); // Set length (0-8 bytes)

	CAN1->sTxMailBox[0].TDLR=
				((uint32_t)TXFrame->data[3] << CAN_TDL0R_DATA3_Pos) |
				((uint32_t)TXFrame->data[2] << CAN_TDL0R_DATA2_Pos) |
				((uint32_t)TXFrame->data[1] << CAN_TDL0R_DATA1_Pos) |
				((uint32_t)TXFrame->data[0] << CAN_TDL0R_DATA0_Pos);

	CAN1->sTxMailBox[0].TDHR=
				((uint32_t)TXFrame->data[7] << CAN_TDH0R_DATA7_Pos) |
				((uint32_t)TXFrame->data[6] << CAN_TDH0R_DATA6_Pos) |
				((uint32_t)TXFrame->data[5] << CAN_TDH0R_DATA5_Pos) |
				((uint32_t)TXFrame->data[4] << CAN_TDH0R_DATA4_Pos);
	
	// Set the TXRQ bit to request transmission
  CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;

	// Wait for transmission to complete
  timeout = 100000;
  while ((CAN1->TSR & CAN_TSR_RQCP0) == 0U && timeout--);
  if (timeout == 0) {
     // Transmission failed
     return;
  }

  // Clear the request complete flag
  CAN1->TSR |= CAN_TSR_RQCP0;
}

void CANBus_L476_ReceiveMessage(CANBusRxFrameDef *RXFrame)
{
	if (CAN1->RF0R & CAN_RF0R_FMP0)
	{
		// Read the received identifier
		RXFrame->identifier = (CAN_RI0R_STID & CAN1->sFIFOMailBox[0].RIR) >> CAN_TI0R_STID_Pos;
		
		// Read the data length
		RXFrame->length = (CAN_RDT0R_DLC & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_DLC_Pos;
		
		/*Clear old data*/
    for (int i=0;i<8;i++)
    {
    	RXFrame->data[i]=0;
    }
		
		RXFrame->data[0] = (uint8_t)((CAN_RDL0R_DATA0 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA0_Pos);
    RXFrame->data[1] = (uint8_t)((CAN_RDL0R_DATA1 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA1_Pos);
    RXFrame->data[2] = (uint8_t)((CAN_RDL0R_DATA2 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA2_Pos);
    RXFrame->data[3] = (uint8_t)((CAN_RDL0R_DATA3 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA3_Pos);
    RXFrame->data[4] = (uint8_t)((CAN_RDH0R_DATA4 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA4_Pos);
    RXFrame->data[5] = (uint8_t)((CAN_RDH0R_DATA5 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA5_Pos);
    RXFrame->data[6] = (uint8_t)((CAN_RDH0R_DATA6 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA6_Pos);
    RXFrame->data[7] = (uint8_t)((CAN_RDH0R_DATA7 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA7_Pos);
		
		// Release the FIFO (not necessary for FIFO0)
    CAN1->RF0R |= CAN_RF0R_RFOM0;
	}
}
