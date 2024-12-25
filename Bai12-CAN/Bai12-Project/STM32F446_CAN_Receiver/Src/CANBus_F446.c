#include "CANBus_F446.h"
#include "stm32f446xx.h"

void CANBus_F446_Pins_Init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	/* Configure PB8 as Pull-Up */
  //GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk);  // Clear PUPD bits for PB8
  //GPIOB->PUPDR |= (1U << GPIO_PUPDR_PUPD8_Pos);  // Set PULL-UP (01)
	
	//GPIOB->MODER &= ~GPIO_MODER_MODE8; // MODER8[1:0] = 00 (Input mode)
	//GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD8; // PUPD8[1:0] = 00 (No pull-up, no pull-down)
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
	
	GPIOB->MODER |=   GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	//GPIOB->MODER &=~ (GPIO_MODER_MODE8_0|GPIO_MODER_MODE9_0);
	
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
	GPIOB->PUPDR |= (1U << GPIO_PUPDR_PUPD8_Pos);  // Set PULL-UP (01)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8_Msk | GPIO_OSPEEDR_OSPEED9_Msk);
	
	//GPIOB->MODER |=   GPIO_MODER_MODE9_1;
	//GPIOB->MODER &=~ GPIO_MODER_MODE9_0;
	
	/*Configure PB8 and PB9 to use CAN Bus AF*/
	GPIOB->AFR[1] &= ~((0xF << GPIO_AFRH_AFSEL8_Pos) | (0xF << GPIO_AFRH_AFSEL9_Pos));
	GPIOB->AFR[1] |= (CAN_AF<<GPIO_AFRH_AFSEL8_Pos)|(CAN_AF<<GPIO_AFRH_AFSEL9_Pos);
	//GPIOB->AFR[1] |= CAN_AF<<GPIO_AFRH_AFSEL9_Pos;
}

void CANBus_F446_Init(void)
{
	/*Enable Clock access to CAN1*/
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
	
	/* Enter Initialization mode*/
  CAN1->MCR |= CAN_MCR_INRQ;
  /*Wait until CANBus peripheral is in initialization mode*/
  while ((CAN1->MSR & CAN_MSR_INAK)==0U){}
	
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
	//CAN1->BTR  =  (14<<CAN_BTR_TS1_Pos)|(1<<CAN_BTR_TS2_Pos)|(4U<<CAN_BTR_BRP_Pos)|(CAN_BTR_LBKM); // Enable Loopback Mode
		
	CAN1->MCR &= ~CAN_MCR_TTCM;
	CAN1->MCR &= ~CAN_MCR_ABOM;
	CAN1->MCR |= CAN_MCR_AWUM;  // Enable Automatic Wake-Up
  CAN1->MCR |= CAN_MCR_NART; // Disable Automatic Retransmission
	CAN1->MCR &= ~CAN_MCR_RFLM;
  CAN1->MCR &= ~CAN_MCR_TXFP;  // Disable Transmit FIFO Priority
	
	CAN1->BTR = (CAN_MODE_NORMAL)|(CAN_SJW_1TQ)|(CAN_BS1_15TQ)|(CAN_BS2_2TQ)|4U;
}

void CANBus_F446_Filter_Configuration()
{
    /* Set the filter initialization mode */
    CAN1->FMR |= CAN_FMR_FINIT;
	
		/*Set the slave Filter to start from 20*/
		CAN1->FMR &=~(CAN_FMR_CAN2SB_Msk);
		CAN1->FMR |=(20<<CAN_FMR_CAN2SB_Pos);

    /* Configure Filter Bank 18 */
    /* Deactivate filter bank 18 */
    CAN1->FA1R &= ~CAN_FA1R_FACT18;

    /* Set filter scale to 32-bit */
    CAN1->FS1R |= CAN_FS1R_FSC18;

    /* Set filter mode to Identifier List Mode */
    CAN1->FM1R &= ~CAN_FM1R_FBM18; // Set to 0 for identifier mask mode

	  // 0x1AD: 0001 1010 1101
	  // 0x4A3: 0100 1010 0011
		// 0x5A1: 0101 1010 0001
		// mask : xxxx 1010 xxxx -> 0x0F0
		// filter 0000 1010 0000 -> 0x0A0
    CAN1->sFilterRegister[18].FR1 = ((0x0000FFFFU & (uint32_t)0x00A1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);
		CAN1->sFilterRegister[18].FR2 = ((0x0000FFFFU & (uint32_t)0x00F1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);
		//CAN1->sFilterRegister[18].FR1 = ((uint32_t)0x000000A0 << 15U);
		//CAN1->sFilterRegister[18].FR2 = ((uint32_t)0x000000F0 << 15U); 
		
		//CAN1->sFilterRegister[18].FR1 = 0x00000000; // Identifier
    //CAN1->sFilterRegister[18].FR2 = 0x00000000; // Identifier mask
		
    /* Assign filter 18 to FIFO0 */
    CAN1->FFA1R &= ~CAN_FFA1R_FFA18;

    /* Activate filter bank 18 */
    CAN1->FA1R |= CAN_FA1R_FACT18;

    /* Exit filter initialization mode */
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

void CANBus_F446_Start(void)
{
	// Leave Initialization mode
  CAN1->MCR &= ~CAN_MCR_INRQ;
  while (CAN1->MSR & CAN_MSR_INAK);
}

void CANBus_F446_SendMessage(CANBusTxFrameDef *TXFrame)
{
	// Wait until the transmit mailbox is empty
  uint32_t timeout = 100000;
	
	// Wait until the transmit mailbox is empty
  while ((CAN1->TSR & CAN_TSR_TME0) == 0U && timeout--){}
  if (timeout == 0) {
    // Mailbox không kh? d?ng, x? lý l?i
    return;
  }
	
	// Set the standard identifier and data length
  //CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
	// Clear all previous settings in TIR
  CAN1->sTxMailBox[0].TIR = 0;
	
	// Set the standard identifier
  CAN1->sTxMailBox[0].TIR |= (TXFrame->identifier & 0x7FF) << 21;
	
	// Clear the IDE bit to ensure Standard ID is used
  CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE; // IDE = 0 (Standard ID)
		
	// Configure the transmit mailbox identifier
  //CAN1->sTxMailBox[0].TIR |= (TXFrame->identifier << 21);
		
  // Configure data length
  //CAN1->sTxMailBox[0].TDTR |= (TXFrame->length << 0);
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

void CANBus_F446_ReceiveMessage(CANBusRxFrameDef *RXFrame)
{
	if (CAN1->RF0R & CAN_RF0R_FMP0)
	{
		// Read the received identifier
    //RXFrame->identifier = (CAN1->sFIFOMailBox[0].RIR >> 3) & 0x1FFFFFFF;
		//RXFrame->identifier = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF;
		RXFrame->identifier = (CAN_RI0R_STID & CAN1->sFIFOMailBox[0].RIR) >> CAN_TI0R_STID_Pos;
		
		// Read the data length
    //RXFrame->length = CAN1->sFIFOMailBox[0].RDTR & 0x0F;
		RXFrame->length = (CAN_RDT0R_DLC & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_DLC_Pos;
		
		/*Clear old data*/
    for (int i=0;i<8;i++)
    {
    	RXFrame->data[i]=0;
    }
		
		/*
		RXFrame->data[0]=CAN1->sFIFOMailBox[0].RDLR >>CAN_RDL0R_DATA0_Pos;
		RXFrame->data[1]=CAN1->sFIFOMailBox[0].RDLR >>CAN_RDL0R_DATA1_Pos;
		RXFrame->data[2]=CAN1->sFIFOMailBox[0].RDLR >>CAN_RDL0R_DATA2_Pos;
		RXFrame->data[3]=CAN1->sFIFOMailBox[0].RDLR >>CAN_RDL0R_DATA3_Pos;

		RXFrame->data[4]=CAN1->sFIFOMailBox[0].RDHR >>CAN_RDH0R_DATA4_Pos;
		RXFrame->data[5]=CAN1->sFIFOMailBox[0].RDHR >>CAN_RDH0R_DATA5_Pos;
		RXFrame->data[6]=CAN1->sFIFOMailBox[0].RDHR >>CAN_RDH0R_DATA6_Pos;
		RXFrame->data[7]=CAN1->sFIFOMailBox[0].RDHR >>CAN_RDH0R_DATA7_Pos;
		*/
		
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
