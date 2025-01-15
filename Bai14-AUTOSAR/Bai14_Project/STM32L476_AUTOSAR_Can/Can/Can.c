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
    /* CAN GPIO Init */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
	GPIOB->MODER |=   GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8_Msk | GPIO_OSPEEDR_OSPEED9_Msk);

	/*Configure PB8 and PB9 to use CAN Bus AF*/
	GPIOB->AFR[1] &= ~((0xF << GPIO_AFRH_AFSEL8_Pos) | (0xF << GPIO_AFRH_AFSEL9_Pos));
	GPIOB->AFR[1] |= (CAN_AF << GPIO_AFRH_AFSEL8_Pos) | (CAN_AF << GPIO_AFRH_AFSEL9_Pos);

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
    CAN1->BTR = Config->CAN_Mode | Config->CAN_Prescaler | Config->CAN_SJW | Config->CAN_BS1 | Config->CAN_BS2;

    CAN1->MCR &= ~Config->CAN_TTCM ;    // Disable Time Triggered Communication Mode
	CAN1->MCR &= ~Config->CAN_ABOM;     // Disables automatic bus-off management.
	CAN1->MCR |= Config->CAN_AWUM;      // Enable Automatic Wake-Up
    CAN1->MCR |= Config->CAN_NART;      // Disable Automatic Retransmission
	CAN1->MCR &= ~Config->CAN_RFLM;     // Disables automatic retransmission of messages in case of transmission errors.
    CAN1->MCR &= ~Config->CAN_TXFP;     // Disable Transmit FIFO Priority

    // Leave Initialization mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
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
    /*Disable Clock access to CAN1*/
	RCC->APB1ENR1 &= ~RCC_APB1ENR1_CAN1EN;

    /* Disable the selected CANx interrupt */
    CAN1->IER &= ~(CAN_IT_FMP0 | CAN_IT_TME | CAN_IT_ERR);

    /* Disable Clock access to GPIO CAN */
    RCC->AHB2ENR &= ~RCC_AHB2ENR_GPIOBEN;
}

/**
 * @brief       This service shall set the baud rate configuration of the CAN controller. Depending on
 *              necessary baud rate modifications the controller might have to reset.
 * @param       Controller: CAN controller, whose baud rate shall be set
 * @param       BaudRateConfigID: references a baud rate configuration by ID (see CanController BaudRateConfig
 * @retval      Std_ReturnType: 
 *              E_OK: Service request accepted, setting of (new) baud rate started
 *              E_NOT_OK: Service request not accepted
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /**< Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, return error */
    }

    /* Disable the CAN controller before configuring */
    CANx->MCR |= CAN_MCR_INRQ;

    switch (BaudRateConfigID) 
    {
        case 125:  // 125 kbps
            CAN1->BTR = (20-1) | CAN_SJW_1TQ | CAN_BS1_15TQ | CAN_BS2_2TQ;
            break;

        case 250:  // 250 kbps
            CAN1->BTR = (10-1) | CAN_SJW_1TQ | CAN_BS1_15TQ | CAN_BS2_2TQ;
            break;

        case 500:  // 500 kbps
            CAN1->BTR = (5-1) | CAN_SJW_1TQ | CAN_BS1_15TQ | CAN_BS2_2TQ;
            break;

        case 1000:  // 1000 kbps (1 Mbps)
            CAN1->BTR = (3-1) | CAN_SJW_1TQ | CAN_BS1_12TQ | CAN_BS2_2TQ;
            break;

        default:
            return E_NOT_OK; 
    }

    // Leave Initialization mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);

    return E_OK;
}

/**
 * @brief       This function performs software triggered state transitions of the CAN controller State machine.
 * @param       Controller: CAN controller for which the status shall be changed
 * @param       Transition: Transition value to request new CAN controller state
 * @retval      Std_ReturnType: 
 *              E_OK: request accepted
 *              E_NOT_OK: request not accepted, a development error occurred
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */
    Std_ReturnType status = E_OK; /* Initialize the return status */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /* Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, return error */
    }

    /* Switch based on the requested controller mode transition */
    switch (Transition)
    {
        case CAN_CS_STARTED: /* Normal mode */
            /* Enter Initialization Mode */
            CANx->MCR |= CAN_MCR_INRQ; /**< Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /* Wait for INAK flag */
            
            /* Exit Initialization Mode to start CAN operation */
            CANx->MCR &= ~CAN_MCR_INRQ; /* Clear INRQ bit to leave initialization mode */
            
            /* Wait until CAN leaves Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) != 0); /* Wait for INAK flag to be cleared */
            break;

        case CAN_CS_SLEEP: /* Sleep mode */
            /* Request Sleep Mode */
            CANx->MCR |= CAN_MCR_SLEEP; /* Set the SLEEP bit to enter Sleep mode */
            
            /* Wait for CAN to enter Sleep mode */
            while ((CANx->MSR & CAN_MSR_SLAK) == 0); /* Wait for SLAK flag to be set */
            break;

        case CAN_CS_STOPPED: /* Stop mode */
            /* Enter Initialization Mode first */
            CANx->MCR |= CAN_MCR_INRQ; /* Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /* Wait for INAK flag */
            
            /* Request Stop Mode */
            CANx->MCR |= CAN_MCR_SLEEP; /* Set the SLEEP bit to stop CAN operation */
            
            /* Wait for CAN to enter Stop Mode */
            while ((CANx->MSR & CAN_MSR_SLAK) == 0); /* Wait for SLAK flag */
            break;

        case CAN_CS_UNINIT: /* Uninitialized state */
            /* Disable the CAN controller */
            CANx->MCR |= CAN_MCR_INRQ; /* Request Initialization mode */
            
            /* Wait until CAN is in Initialization Mode */
            while ((CANx->MSR & CAN_MSR_INAK) == 0); /* Wait for INAK flag */
            
            /* Reset the CAN controller to uninitialized state */
            CANx->MCR |= CAN_MCR_RESET; /* Reset CAN controller */
            
            /* Wait for CAN to reset */
            while ((CANx->MSR & CAN_MSR_INAK) != 0); /* Wait for INAK flag to be cleared */
            break;

        default:
            status = E_NOT_OK; /* Invalid transition state */
            break;
    }

    return status; /* Return the operation status (E_OK or E_NOT_OK) */
}

/**
 * @brief       This function disables all interrupts for this CAN controller.
 * @param       Controller: CAN controller for which interrupts shall be disabled.
 * @retval      Void
 */
void Can_DisableControllerInterrupts(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /* Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return; /* Invalid controller, do nothing or handle error */
    }

    // Disable CAN1 interrupts by clearing the relevant bits in the CAN_IER register
    CANx->IER &= ~CAN_IT_FMP0;      // FIFO 0 message pending interrupt
    CANx->IER &= ~CAN_IT_FMP1;      // FIFO 1 message pending interrupt
    CANx->IER &= ~CAN_IT_TME;       // Transmit mailbox empty interrupt
    CANx->IER &= ~ CAN_IT_ERR;      // Error interrupt
    CANx->IER &= ~CAN_IT_WKU;       // Wakeup interrupt
    CANx->IER &= ~CAN_IT_SLK;       // Sleep interrupt

    /* Clear the pending interrupt flags */
    CANx->RF0R = CAN_RF0R_FULL0;                                // FIFO 0 message pending interrupt
    CANx->RF1R = CAN_RF1R_FULL1;                                // FIFO 1 message pending interrupt

    CANx->TSR = CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2;      // Transmit mailbox empty interrupt

    /*Clear LEC bits */
    CANx->ESR = 0; 
    /* Clear CAN_MSR_ERRI (rc_w1) */
    CANx->MSR = CAN_MSR_ERRI; ;                                 // Error interrupt

    CANx->MSR = CAN_MSR_WKUI;                                   // Wakeup interrupt
    CANx->MSR = CAN_MSR_SLAKI;                                  // Sleep interrupt
}

/**
 * @brief       This function enables all allowed interrupts.
 * @param       Controller: CAN controller for which interrupts shall be re-enabled
 * @retval      Void
 */
void Can_EnableControllerInterrupts(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /* Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return; /* Invalid controller, do nothing or handle error */
    }

    // Enable CAN1 interrupts by setting the relevant bits in the CAN_IER register
    CANx->IER |= CAN_IT_FMP0;      // FIFO 0 message pending interrupt
    CANx->IER |= CAN_IT_FMP1;      // FIFO 1 message pending interrupt
    CANx->IER |= CAN_IT_TME;       // Transmit mailbox empty interrupt
    CANx->IER |=  CAN_IT_ERR;      // Error interrupt
    CANx->IER |= CAN_IT_WKU;       // Wakeup interrupt
    CANx->IER |= CAN_IT_SLK;       // Sleep interrupt
}

/**
 * @brief       This function checks if a wakeup has occurred for the given controller
 * @param       Controller: Controller to be checked for a wakeup.
 * @retval      Std_ReturnType: 
 *              E_OK: API call has been accepted
 *              E_NOT_OK: API call has not been accepted
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */
    Std_ReturnType status = E_NOT_OK; /* Initialize the return status to E_NOT_OK */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (Controller == 0)
    {                
        CANx = CAN1; /* Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, do nothing or handle error */
    }

    /* Check if the CAN controller is awake (SLAK bit in MSR should be cleared) */
    if ((CANx->MSR & CAN_MSR_SLAK) == 0) /* SLAK = 0 means CAN is awake */
    {
        /* Return E_OK if the CAN controller is awake */
        status = E_OK; 
        /* Clear Wake-up interrupt flag */
        CANx->MSR = CAN_MSR_WKUI;
		
        /* Return E_OK if the CAN controller is awake */
		status = E_OK; 
    }

    return status; /* Return the status (E_OK if awake, E_NOT_OK if in sleep) */
}

/**
 * @brief     This service obtains the error state of the CAN controller.
 * @param     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for ErrorState.
 * @param     ErrorStatePtr: Pointer to a memory location, where the error state of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Error state request has been accepted.
 *            E_NOT_OK: Error state request has not been accepted
 */
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType *ErrorStatePtr)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */
    Std_ReturnType status = E_NOT_OK; /* Initialize the return status to E_NOT_OK */

    /* Select the appropriate CAN controller based on the 'Controller' parameter */
    if (ControllerId == 0)
    {                
        CANx = CAN1; /* Assign CAN1 to CANx (0 -> CAN1) */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller, do nothing or handle error */
    }

    /* Check if the ErrorStatePtr is valid */
    if (ErrorStatePtr == NULL)
    {
        return E_NOT_OK; /**< Invalid pointer, return error */
    }

    /* Check the error flags in the CAN controller status register */
    if (CANx->ESR & CAN_ESR_BOFF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF; /* Bus-off error state */
    }
    else if (CANx->ESR & CAN_ESR_EPVF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /* Error passive state */
    }
    else if (CANx->ESR & CAN_ESR_EWGF)
    {
        *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE; /* Error warning state */
    }
    else
    {
        return E_NOT_OK; /* Invalid, return error */
    }

    return E_OK; /* Return success */
}

/**
 * @brief     This service reports about the current status of the requested CAN controller.
 * @param     Controller: CAN controller for which the status shall be requested.
 * @param     ControllerModePtr: Pointer to a memory location, where the current mode of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Controller mode request has been accepted.
 *            E_NOT_OK: Controller mode request has not been accepted.
 */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType *ControllerModePtr)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Check if the ControllerModePtr is valid */
    if (ControllerModePtr == NULL)
    {
        return E_NOT_OK; /* Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the Controller parameter */
    if (Controller == 0)
    {
        CANx = CAN1; /* CAN1 selected */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller ID, return error */
    }

    /* Check the current mode of the selected CAN controller */
    if (CANx->MCR & CAN_MCR_INRQ) /* Initialization request flag */
    {
        *ControllerModePtr = CAN_CS_UNINIT; /* Controller is uninitialized */
    }
    else if (CANx->MSR & CAN_MSR_SLAK) /* Sleep acknowledge flag */
    {
        *ControllerModePtr = CAN_CS_SLEEP; /* Controller is in sleep mode */
    }
    else if (CANx->MSR & CAN_MSR_TXM) /* Transmit mode flag */
    {
        *ControllerModePtr = CAN_CS_STARTED; /* Controller is operational */
    }
    else
    {
        *ControllerModePtr = CAN_CS_STOPPED; /* Controller is stopped */
    }

    return E_OK; /* Return success */
}

/**
 * @brief     Returns the Rx error counter for a CAN controller. 
 * @param     ControllerId: CAN controller, whose current Rx error counter shall be acquired.
 * @param     RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Rx error counter available.
 *            E_NOT_OK: Wrong ControllerId, or Rx error counter not available.
 */
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8 *RxErrorCounterPtr)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Check if the RxErrorCounterPtr is valid */
    if (RxErrorCounterPtr == NULL)
    {
        return E_NOT_OK; /* Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the ControllerId */
    if (ControllerId == 0)
    {
        CANx = CAN1; /* CAN1 selected */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller ID, return error */
    }

    /* Read the receive error counter (REC) from ESR register */
    *RxErrorCounterPtr = (uint8)((CANx->ESR & CAN_ESR_REC) >> 24); /* Mask and shift to get REC-Bits 31:24 */

    return E_OK; /* Return success */
}

/**
 * @brief     Returns the Tx error counter for a CAN controller. 
 * @param     ControllerId: CAN controller, whose current Rx error counter shall be acquired.
 * @param     RxErrorCounterPtr: Pointer to a memory location, where the current Rx error counter of the CAN controller will be stored.
 * @retval    Std_ReturnType: 
 *            E_OK: Rx error counter available.
 *            E_NOT_OK: Wrong ControllerId, or Rx error counter not available.
 */
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8 *TxErrorCounterPtr)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Check if the TxErrorCounterPtr is valid */
    if (TxErrorCounterPtr == NULL)
    {
        return E_NOT_OK; /* Invalid pointer, return error */
    }

    /* Select the appropriate CAN controller based on the ControllerId */
    if (ControllerId == 0)
    {
        CANx = CAN1; /* CAN1 selected */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller ID, return error */
    }

    /* Read the transmit error counter (TEC) from ESR register */
    *TxErrorCounterPtr = (uint8)((CANx->ESR & CAN_ESR_TEC) >> 16); /* Mask and shift to get TEC-Bits 23:16 */

    return E_OK; /**< Return success */
}

/**
 * @brief     This function is called by CanIf to pass a CAN message to CanDrv for transmission
 * @param     Hth: information which HW-transmit handle shall be used for transmit
 * @param     PduInfo: Pointer to SDU user memory, Data Length and Identifier.
 * @retval    Std_ReturnType: 
 *            E_OK: Write command has been accepted
 *            E_NOT_OK: development error occurred
*/
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
    CAN_TypeDef *CANx = NULL; /* Declare pointer for CAN controller */

    /* Select the appropriate CAN controller based on the Hth */
    if (Hth == 0)
    {
        CANx = CAN1; /* CAN1 selected */
    }
    else
    {
        return E_NOT_OK; /* Invalid controller ID, return error */
    }

    /* Check for free mailbox */
    if ((CAN1->TSR & CAN_TSR_TME0) == 0) 
    {  
        // Check if Mailbox 0 is free
        return E_NOT_OK;  
    }

    /* Configure the Mailbox with the message data */
   // Clear all previous settings in TIR
    CAN1->sTxMailBox[0].TIR = 0;
        
    // Set the standard identifier
    CAN1->sTxMailBox[0].TIR |= (PduInfo->id & 0x7FF) << 21;
        
    // Clear the IDE bit to ensure Standard ID is used
    CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE; // IDE = 0 (Standard ID)
            
    // Configure data length
    CAN1->sTxMailBox[0].TDTR = 0; // Clear TDTR
    CAN1->sTxMailBox[0].TDTR |= (PduInfo->length & 0x0F); // Set length (0-8 bytes)

	CAN1->sTxMailBox[0].TDLR=
				((uint32_t)PduInfo->sdu[3] << CAN_TDL0R_DATA3_Pos) |
				((uint32_t)PduInfo->sdu[2] << CAN_TDL0R_DATA2_Pos) |
				((uint32_t)PduInfo->sdu[1] << CAN_TDL0R_DATA1_Pos) |
				((uint32_t)PduInfo->sdu[0] << CAN_TDL0R_DATA0_Pos);

	CAN1->sTxMailBox[0].TDHR=
				((uint32_t)PduInfo->sdu[7] << CAN_TDH0R_DATA7_Pos) |
				((uint32_t)PduInfo->sdu[6] << CAN_TDH0R_DATA6_Pos) |
				((uint32_t)PduInfo->sdu[5] << CAN_TDH0R_DATA5_Pos) |
				((uint32_t)PduInfo->sdu[4] << CAN_TDH0R_DATA4_Pos);
	
	// Set the TXRQ bit to request transmission
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;

    return E_OK;  
}
