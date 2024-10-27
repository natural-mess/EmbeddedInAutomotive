# Bài 5 - I2C

### 1. I2C Software
Bài này sử dụng STM32L476 để giao tiếp với LCD 16x2 thông qua giao tiếp I2C. Sơ đồ chân kết nối giữa MCU và LCD như sau:
```c
STM32L476                LCD 16x2 (I2C Module)
+-------------+          +-----------------+
|             |          |                 |
|   GPIOB6    +---------->   SDA           |
|   GPIOB7    +---------->   SCL           |
|             |          |                 |
|    GND      +---------->   GND           |
|    VCC      +---------->   VCC           |
+-------------+          +-----------------+
```
I2C là giao thức truyền thông hai chiều và cho phép nhiều thiết bị chia sẻ cùng một bus SDA và SCL. Với cấu hình "open-drain", mỗi thiết bị chỉ kéo đường bus xuống mức thấp (0V) khi cần thiết và để nó "float" (ở trạng thái không kéo xuống mức thấp) trong các trường hợp khác. Do đó, cả hai đường SDA và SCL đều cần pull-up để giữ mức cao khi không có thiết bị nào đang truyền dữ liệu. Nếu các chân GPIO được cấu hình là "push-pull" (push-pull sẽ có cả hai trạng thái cao và thấp), khi hai thiết bị cùng kéo mức điện áp về hai mức khác nhau sẽ dễ gây ra xung đột và hỏng thiết bị. Các điện trở kéo lên (pull-up resistors) sẽ kéo cả hai đường SDA và SCL lên mức logic cao (thường là VCC) khi không có thiết bị nào đang truyền dữ liệu. Các thiết bị sẽ chỉ kéo mức điện áp xuống thấp khi cần.

**Data frame của I2C**
**1. Bắt đầu Khung Dữ liệu (Start Condition)** \
Start Condition: Bắt đầu của mỗi giao dịch trên bus I2C được xác định khi master kéo chân SDA (data line) xuống thấp trong khi SCL (clock line) vẫn ở mức cao. Đây là tín hiệu để báo hiệu bắt đầu một khung truyền dữ liệu mới và giúp đồng bộ các thiết bị I2C trên cùng bus.
**2. Địa chỉ của Slave (Address Byte)**\
- Sau tín hiệu bắt đầu, master sẽ gửi byte địa chỉ (7-bit hoặc 10-bit) của thiết bị slave mà nó muốn giao tiếp.
- 1 bit đọc/ghi (R/W) sẽ đi sau địa chỉ:\
	- 0: Chế độ ghi - master muốn gửi dữ liệu đến slave.\
	- 1: Chế độ đọc - master muốn nhận dữ liệu từ slave.\
- Như vậy, byte địa chỉ luôn gồm 7 (hoặc 10) bit địa chỉ cộng với 1 bit R/W, tổng cộng là 8 bit.
**3. Bit Acknowledge (ACK/NACK)**
- Sau khi nhận byte địa chỉ, thiết bị slave có địa chỉ trùng khớp sẽ phản hồi lại bằng cách kéo chân SDA xuống thấp để gửi ACK (bit xác nhận) khi nhận dữ liệu thành công.
- Nếu không có ACK (thiết bị giữ SDA ở mức cao), master sẽ hiểu rằng không có thiết bị slave nào phản hồi và sẽ dừng hoặc hủy giao dịch.
**4. Byte Dữ liệu (Data Byte)**
- Sau khi master nhận được ACK từ slave, byte dữ liệu đầu tiên sẽ được gửi. Mỗi byte dữ liệu gồm 8 bit và được gửi tuần tự từ bit MSB đến bit LSB.
- Sau mỗi byte dữ liệu, thiết bị nhận (có thể là master hoặc slave, tùy thuộc vào chiều giao tiếp) phải gửi một ACK nếu nhận thành công, hoặc NACK nếu có lỗi trong quá trình nhận dữ liệu hoặc nếu không muốn nhận thêm dữ liệu.
**5. Kết thúc Khung Dữ liệu (Stop Condition)**\
Khi master hoàn tất quá trình truyền dữ liệu, nó sẽ tạo ra Stop Condition bằng cách kéo SDA lên cao trong khi SCL đang ở mức cao. Đây là tín hiệu báo hiệu kết thúc giao dịch I2C trên bus.


#### Cấu hình GPIO
Cấu hình PB6 và PB7 là output với chế độ open-drain và sử dụng chế độ pull-up.
```c
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6|LL_GPIO_PIN_7);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}
```

#### Cấu hình Timer2
Chúng ta cần tạo delay 1us nên với Internal Clock là 4MHz, Prescaler có thể set thành 4-1 tức là 3. Khi đó, tần số cho mỗi clock tick là:
$${4MHz \over 4}=1MHz$$

Điều này có nghĩa là timer đếm được 1000000 tick mỗi giây. Thời gian của mỗi tick sẽ là:
$${1 \over 1MHz}=1us$$

Khi đó, với Autoreload bằng 0 có nghĩa là tạo ra 1 tick. Với tần số 1 MHz, delay mỗi lần gọi delay_us(1) sẽ là: 
$$Delay=(0+1)×1us=1us$$

```c
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  TIM_InitStruct.Prescaler = 3;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}
```

#### Start và Stop condition của I2C
Start Condition: Master kéo đường SDA từ mức cao xuống mức thấp trong khi SCL vẫn cao, báo hiệu bắt đầu của quá trình truyền dữ liệu. 
```c
// Bat dau giao tiep I2C
void I2C_Start(void)
{
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5); //delay 1us
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
    delay_us(5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
}
```
Stop Condition: Master kéo đường SDA từ mức thấp lên mức cao trong khi SCL vẫn cao, báo hiệu kết thúc của quá trình truyền dữ liệu. 
```c
// Dung giao tiep I2C
void I2C_Stop(void)
{
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    delay_us(5);
}
```

#### Truyền nhận dữ liệu
- Master Phát Địa Chỉ Slave: Sau khi Start Condition, Master gửi địa chỉ 7 bit của Slave lên bus kèm theo một bit đọc/ghi (R/W).
	- Nếu bit R/W là 0, Master yêu cầu ghi (truyền dữ liệu tới Slave).
	- Nếu bit R/W là 1, Master yêu cầu đọc (nhận dữ liệu từ Slave).
- Acknowledge (ACK): Mỗi khi một byte (8 bit) được truyền đi, thiết bị Slave sẽ phản hồi bằng cách kéo đường SDA xuống thấp trong 1 bit thời gian để cho biết nó đã nhận được byte. Đây được gọi là bit ACK.
- Truyền Dữ Liệu:
	- Viết dữ liệu: Master gửi từng byte đến Slave, và Slave sẽ phản hồi lại bằng một bit ACK sau mỗi byte.
	- Đọc dữ liệu: Master đọc từng byte từ Slave, và sau mỗi byte, Master phát lại bit ACK để báo rằng dữ liệu đã được nhận.
- Kết thúc Giao tiếp: Khi quá trình truyền dữ liệu hoàn tất, Master phát Stop Condition để ngắt kết nối với Slave.
```c
// Gui mot byte qua I2C
void I2C_SendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) 
		{
        if ((byte & 0x80) != 0) 
				{
            LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
        } else 
				{
            LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0
        }
        delay_us(5);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
        delay_us(5);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
        byte <<= 1; // Shift byte sang trái
    }
    // Nhan ACK tu Slave
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
}
```
```c
// Ham doc 1 byte tu I2C
uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t receivedByte = 0;

    // Dat SDA o trang thai dau vao
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (trang thái cao)
    
    for (int i = 0; i < 8; i++) 
    {
        receivedByte <<= 1; // Dich sang trai de chuan bi nhan bit tiep theo
        
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
        delay_us(5);
        
        if (LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7))  // Kiem tra trang thai SDA
        {
            receivedByte |= 0x01; // Neu SDA = 1, set bit LSB cua receivedByte
        }
        
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
        delay_us(5);
    }
    
    // Gui ACK hoac NACK cho Slave
    if (ack) 
    {
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 0 (ACK)
    } 
    else 
    {
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (NACK)
    }
    
    delay_us(5);
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 1
    delay_us(5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6); // SCL = 0
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7); // SDA = 1 (phuc hoi trang thai cao cho SDA)
    
    return receivedByte;
}
```
### 1. I2C Hardware
