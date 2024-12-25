# Bài 12 - CAN Software

Vì Low Layer (LL) API không hỗ trợ thư viện cho CAN, nên chúng ta sẽ viết phần mêm giao tiếp CAN sử dụng trực tiếp các thanh ghi. STM32L476 sẽ đóng vai trò truyền dữ liệu và STM32F446 sẽ nhận dữ liệu. Với mỗi ID trong message nhận được từ STM32L476 thì LED trên STM32F446 sẽ nhấp nháy với thời gian khác nhau, cụ thể như sau:
- 0x1AD : LED nhấp nháy 2s
- 0x4A3 : LED nhấp nháy 1s
- 0x5A1 : LED nhấp nháy 0.5s

## 1. Cấu hình thiết bị truyền dữ liệu STM32L476
### GPIO Initialization
Tạo 2 file đặt tên là CANBus_L476.c và CANBus_L476.h, thêm 2 file này vào KeilC project. Trong file CANBus_L476.c, thêm 2 dòng code sau:

```c
#include "CANBus_L476.h"
#include "stm32l476xx.h"
```

Sau đó, khai báo hàm ```CANBus_L476_Pin_Init(void)```.

Trên STM32L476, chúng ta sẽ sử dụng 2 chân PB8 và PB9 tương ứng với CANRX và CANTX. Vậy nên chúng ta cần cấu hình xung clock cho GPIOB.

![alt text](_assets/GPIOB_Clock.png)

GPIOB được kết nối với AHB2, nên chúng ta cần set bit tương ứng trên register AHB2ENR

![alt text](_assets/AHB2ENR.png)

Chúng ta có hàm Init như sau:
```c
	/*Enable clock access to GPIOB*/
	RCC->AHB2ENR|=RCC_AHB2ENR_GPIOBEN;
```

Sau đó, chúng ta cần cấu hình 2 chân PB8 và PB2 để sử dụng alternate function:

![alt text](_assets/GPIOB_MODER.png)

```c
	GPIOB->MODER |=   GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	GPIOB->MODER &=~ (GPIO_MODER_MODE8_0|GPIO_MODER_MODE9_0);
```

Chúng ta cần tìm xem AF nào được sử dụng cho CANBus. Từ hình sau chúng ta thấy AF09 được dùng cho CANBus trên chân PB8 và PB9

![alt text](_assets/CANBus_AF09.png)

Do đó, trong file CANBus_L476.h, chúng ta tạo 1 macro như sau:

```c
#define CAN_AF 0x09
```

Tiếp đó, chúng ta cấu hình chân PB8 và PB9 để sử dụng CAN Bus alternate function.
```c
	GPIOB->AFR[1] |= (CAN_AF<<GPIO_AFRH_AFSEL8_Pos)|(CAN_AF<<GPIO_AFRH_AFSEL9_Pos);
```

Cuối cùng, chúng ta có hàm sau:
```c
void CANBus_L476_Pins_Init(void)
{
	/*Enable clock access to GPIOB*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		
	GPIOB->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk);
	
	GPIOB->MODER |=   GPIO_MODER_MODE8_1|GPIO_MODER_MODE9_1;
	
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk | GPIO_PUPDR_PUPD9_Msk);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED8_Msk | GPIO_OSPEEDR_OSPEED9_Msk);
	
	/*Configure PB8 and PB9 to use CAN Bus AF*/
	GPIOB->AFR[1] &= ~((0xF << GPIO_AFRH_AFSEL8_Pos) | (0xF << GPIO_AFRH_AFSEL9_Pos));
	GPIOB->AFR[1] |= (CAN_AF << GPIO_AFRH_AFSEL8_Pos) | (CAN_AF << GPIO_AFRH_AFSEL9_Pos);
}
```

### CANBus Initialization
Khai báo thêm 1 hàm có tên ```void CANBus_L476_Init(void)``` dùng để init CANBus. Để sử dụng CAN Bus trên STM32L476, chúng ta cần kích hoạt xung clock cho CAN.

![alt text](_assets/bxCAN1_Clock.png)

Vì CAN1 được kết nối với APB1 nên chúng ta cần set bit CAN1EN trong register APB1ENR1.

![alt text](_assets/APB1ENR1.png)

Do đó trong hàm ```void CANBus_L476_Init(void)``` chúng ta thêm dòng code sau:

```c
	/*Enable Clock access to CAN1*/
	RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;
```

Để khởi tạo CANBus, chúng ta cần set nó thành Initialization mode, bằng cách set INRQ bit trong CAN_MCR register (CAN master control register) thành 1.

![alt text](_assets/CAN_MCR_INRQ.png)

```c
  /* Enter Initialization mode*/
  CAN1->MCR |= CAN_MCR_INRQ;
  /*Wait until CANBus peripheral is in initialization mode*/
  while (!(CAN1->MSR & CAN_MSR_INAK));
```

Cấu hình tiếp CAN_MCR register để thoát khỏi sleep mode và kích hoạt CAN1.

![alt text](_assets/CAN_MCR_SLEEP.png)

```c
  // Enable CAN1 peripheral
  CAN1->MCR &= ~CAN_MCR_SLEEP;
  while ((CAN1->MSR & CAN_MSR_SLAK)!=0U){}
```

Tiếp sao chúng ta cần cấu hình timing cho CANBus sử dụng CAN_BTR register (CAN bit timing register).

![alt text](_assets/CAN_BTR.png)

```c
  /*Reset the non-zero initial values*/
  CAN1->BTR = (CAN_MODE_NORMAL)|(CAN_SJW_1TQ)|(CAN_BS1_15TQ)|(CAN_BS2_2TQ)|4U;
```

Với các macro được cấu hình như sau:

```c
#define CAN_SJW_1TQ                 (0x00000000U) 

#define CAN_BS1_1TQ                 (0x00000000U)                                                /*!< 1 time quantum  */
#define CAN_BS1_2TQ                 ((uint32_t)CAN_BTR_TS1_0)                                    /*!< 2 time quantum  */
#define CAN_BS1_3TQ                 ((uint32_t)CAN_BTR_TS1_1)                                    /*!< 3 time quantum  */
#define CAN_BS1_4TQ                 ((uint32_t)(CAN_BTR_TS1_1 | CAN_BTR_TS1_0))                  /*!< 4 time quantum  */
#define CAN_BS1_5TQ                 ((uint32_t)CAN_BTR_TS1_2)                                    /*!< 5 time quantum  */
#define CAN_BS1_6TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_0))                  /*!< 6 time quantum  */
#define CAN_BS1_7TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1))                  /*!< 7 time quantum  */
#define CAN_BS1_8TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 8 time quantum  */
#define CAN_BS1_9TQ                 ((uint32_t)CAN_BTR_TS1_3)                                    /*!< 9 time quantum  */
#define CAN_BS1_10TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_0))                  /*!< 10 time quantum */
#define CAN_BS1_11TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1))                  /*!< 11 time quantum */
#define CAN_BS1_12TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 12 time quantum */
#define CAN_BS1_13TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2))                  /*!< 13 time quantum */
#define CAN_BS1_14TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_0))  /*!< 14 time quantum */
#define CAN_BS1_15TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_1))  /*!< 15 time quantum */
#define CAN_BS1_16TQ                ((uint32_t)CAN_BTR_TS1) /*!< 16 time quantum */

#define CAN_BS2_1TQ                 (0x00000000U)                                /*!< 1 time quantum */
#define CAN_BS2_2TQ                 ((uint32_t)CAN_BTR_TS2_0)                    /*!< 2 time quantum */
#define CAN_BS2_3TQ                 ((uint32_t)CAN_BTR_TS2_1)                    /*!< 3 time quantum */
#define CAN_BS2_4TQ                 ((uint32_t)(CAN_BTR_TS2_1 | CAN_BTR_TS2_0))  /*!< 4 time quantum */
#define CAN_BS2_5TQ                 ((uint32_t)CAN_BTR_TS2_2)                    /*!< 5 time quantum */
#define CAN_BS2_6TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_0))  /*!< 6 time quantum */
#define CAN_BS2_7TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_1))  /*!< 7 time quantum */
#define CAN_BS2_8TQ                 ((uint32_t)CAN_BTR_TS2)   
```

Dựa theo trang web [CAN Bit Time Calculation](http://www.bittiming.can-wiki.info/), với Tần số clock là 45MHz và SJW là 1, ta có input như sau:

![alt text](_assets/CANBitTimeCalc.png)

Từ đó, ta có kết quả là các bit time cần chọn để phù hợp với baud rate tương ứng mà chúng ta mong muốn. Ở đây với tốc độ baud là 500Kbps, ta có thể chọn từ bảng như sau:

![alt text](_assets/CAN_BitTimeOutput.png)

- ```CAN_SJW_1TQ``` : SJW=1, Sync Segment luôn là 1 TQ
- ```CAN_BS1_15TQ```: TS1=15, số time quanta của Time Segment 1 = 15.
- ```CAN_BS2_2TQ```: TS2=2, số time quanta của Time Segment 2 = 2.
- Prescaler = 4+1=5

Tần số clock của CAN là: 

$$Clock_{CAN}={{Clock_{APB1}} \over {BRP+1}}$$

Với BRP = 4, clock của CAN sẽ giảm đi 5 lần vì BRP+1=5.

Baudrate là tốc độ truyền dữ liệu trong giao tiếp CAN, được tính theo công thức:

$$Baudrate={{Clock_{CAN}} \over {\text{Tổng số time quanta trong mỗi bit}}}$$

Ở đây, Clock của APB1 là 40MHz, với BRP là 9, ta có

$$Clock_{CAN}={{45MHz} \over {4+1}}=9MHz$$

Tổng số time quanta trong một bit:

$$TQ_{total}=\text{Sync Segment} + \text{Time Segment 1 (TS1)} + \text{Time Segment 2 (TS2)}$$

Trong đó:
- Sync Segment: Luôn là 1 TQ.
- TS1 = 2
- TS2 = 1

Vậy 

$$TQ_{total}=1 + 15 + 2 = 18TQ$$

$$Baudrate={{9MHz} \over {18}} = 500Kbps$$

### Filter configuration

Trước hết, cần set filter mode cho CAN thành init mode.

![alt text](_assets/CAN_FMR_FINIT.png)

Tắt filter bank 0 trước khi cấu hình

![alt text](_assets/CAN_FA1R_FACT0.png)

Cấu hình filter bank 0 ở chế độ 32-bit

![alt text](_assets/CAN_FS1R_FSC0_32bit.png)

Đặt filter bank 0 ở chế độ identifier mask mode

![alt text](_assets/CAN_FM1R_FBM0_MaskMode.png)

Thiết lập giá trị filter và mask

![alt text](_assets/CAN_FiRx.png)

Gán filter bank 0 cho FIFO0

![alt text](_assets/CAN_FFA1R.png)

Cuối cùng là kích hoạt filter bank 0 và thoát khỏi chế độ cấu hình Filter

Với ```CAN1->sFilterRegister[0].FR1``` và ```CAN1->sFilterRegister[0].FR2``` ta cần chọn thông số phù hợp để chấp nhận ID mà mình mong muốn. Ở đây ta cần receiver chấp nhận 3 ID là 0x1AD, 0x4A3 và 0x5A1. Do đó ta rút ra được 2 giá trị mask là 0x0F0 và filter là 0x0A0

```
0x1AD: 0001 1010 1101
0x4A3: 0100 1010 0011
0x5A1: 0101 1010 0001
mask : xxxx 1010 xxxx -> 0x0F0
filter 0000 1010 0000 -> 0x0A0
```

Vì chúng ta cấu hình cho filter bank 0 ở chế độ mask mode, nên FR1 sẽ nhận giá trị là filter và FR2 sẽ là mask. Vì ở đây chúng ta sử dụng standard ID, nên chúc ta cần dịch 2 giá trị filter và mask vào vị trí STID từ bit 31 đến bit 21, tương ứng với 11 bit của standard ID.

![alt text](_assets/CAN_FB0_Rx.png)

```c
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
	// mask : xxxx 1010 xxxx -> 0x0F0
	// filter 0000 1010 0000 -> 0x0A0
	CAN1->sFilterRegister[0].FR1 = ((0x0000FFFFU & (uint32_t)0x00A1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);
	CAN1->sFilterRegister[0].FR2 = ((0x0000FFFFU & (uint32_t)0x00F1<<5) << 16U) | (0x0000FFFFU & (uint32_t)0x0000);

    /* Assign filter bank 0 to FIFO0 */
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    /* Activate filter bank 0 */
    CAN1->FA1R |= CAN_FA1R_FACT0;

    /* Exit filter initialization mode */
    CAN1->FMR &= ~CAN_FMR_FINIT;
}
```
### CANBus Start

Để khởi động CAN Bus, chúng ta cần thoát khỏi chế độ init mode và chờ đến khi việc thoát ra này hoàn tất.

![alt text](_assets/CAN_MCR_INRQ.png)

```c
void CANBus_L476_Start(void)
{
	// Leave Initialization mode
  CAN1->MCR &= ~CAN_MCR_INRQ;
  while (CAN1->MSR & CAN_MSR_INAK);
}
```

### Transmit function

Trước hết, chúng ta cần khai báo 2 struct chứa dữ liệu truyền và nhận trong CAN.

```c
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
```

Trong đó:
- Identifier: là ID của CAN node.
- Length: độ dài của data cần gửi hoặc nhận (Maximum 8).
- data array: chứa data buffer cần gửi hoặc nhận.

Sau đó, khai báo hàm ```void CANBus_SendMessage(CANBusTxFrameDef *TXFrame )```. Trong hàm này, chúng ta đợi cho tới khi mailbox trống bằng cách kiểm tra TME0 (Transmit Mailbox 0) được set thành 1 hay không.

![alt text](_assets/CAN_TSR_TME0.png)

```c
  // Wait until the transmit mailbox is empty
  while ((CAN1->TSR & CAN_TSR_TME0) == 0){}
```

Đặt identifier mode thành standard identifier

![alt text](_assets/CAN_TIxR_Standard.png)

```c
 // Set the standard identifier and data length
  CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
```

Set mailbox ID

```c
  // Configure the transmit mailbox identifier
  CAN1->sTxMailBox[0].TIR |= (TXFrame->identifier << 21);
```

Set độ dài của data cần gửi

![alt text](_assets/CAN_TDTxR_DLC.png)

Set buff vào CANBus FIFO:

![alt text](_assets/CAN_TDLxR.png)

![alt text](_assets/CAN_TDHxR.png)

```c
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
```

Cuối cùng là bắt đầu quá trình truyền dữ liệu

![alt text](_assets/CAN_TIxR_TXRQ.png)

```c
  // Set the TXRQ bit to request transmission
  CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
```

### Receive function

Chúng ta khai báo hàm ```void CANBus_L476_ReceiveMessage(CANBusRxFrameDef *RXFrame)``` để thực hiện việc nhận dữ liệu.

Trước hết, chờ cho đến khi hộp thư gửi (TX Mailbox) trống, tức là có thể gửi dữ liệu mới.

![alt text](_assets/CAN_RF0R_FMP0.png)

Nếu không có message pending, chúng ta bắt đầu lấy ID từ dữ liệu nhận được. Đảm bảo rằng các bit của standard identifier (ID tiêu chuẩn) trong thanh ghi TIR được reset trước khi cấu hình. 

![alt text](_assets/CAN_RIxR_EXID.png)

Tiếp theo, lấy độ dài của dữ liệu. 

![alt text](_assets/CAN_RDTxR_DLC.png)

Sau đó, xoá dữ liệu cũ đang lưu trong mảng data, đọc dữ liệu mới từ FIFO0 và copy dữ liệu mới vào mảng data 

![alt text](_assets/CAN_RDLxR.png)

![alt text](_assets/CAN_RDHxR.png)

Cuối cùng là giải phóng FIFO0 để nó có thể nhận thông điệp mới.

```c
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
```

Trong hàm main, ta viết một chương trình đơn giản để để gửi CAN frame mỗi khi nút bấm được nhấn:
```c
while (1)
  {
		if(ButtonPressed())
		{
			if (txFrame.identifier == 0x1A1)
			{
				txFrame.identifier = 0x4A3;
				CANBus_L476_SendMessage(&txFrame);
			}
			else if (txFrame.identifier == 0x4A3)
			{
				txFrame.identifier = 0x5A1;
				CANBus_L476_SendMessage(&txFrame);
			}
			else if (txFrame.identifier == 0x5A1 || txFrame.identifier == 0x00)
			{
				txFrame.identifier = 0x1A1;
				CANBus_L476_SendMessage(&txFrame);
			}
		}
  }
```

## 2. Cấu hình thiết bị nhận dữ liệu STM32F446
Ở thiết bị nhận là STM32F446, ta cấu hình tương tự với thiết bị truyền STM32L476, chỉ thay đổi ở thanh ghi RCC vì CAN1 trên STM32F446 sử dụng bus khác với STM32L476:

```c
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
```

Ngoài ra cấu hình cho filter trên STM32F446 cũng có chút khác biệt, vì nó có 2 CAN bus, nên vì chúng ta muốn sử dụng CAN1 với filter bank là 18, chúng ta cần cấu hình để mặc định CAN2 sẽ sử dụng filter bank từ 20 trở đi:

```c
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
```

Trong hàm main, ta viết một chương trình đơn giản để nhận dữ liệu từ transmitter, sau đó đọc ID của dữ liệu rồi nhấp nháy đèn LED với tốc độ tương ứng:
```c
while (1)
  {
		CANBus_F446_ReceiveMessage(&rxFrame);
		if (rxFrame.identifier == 0x1A1)
		{
			LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			delay_ms(2000);
		}
		else if (rxFrame.identifier == 0x4A3)
		{
			LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			delay_ms(1000);
		}
		else if (rxFrame.identifier == 0x5A1)
		{
			LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			delay_ms(500);
		}
  }
```
