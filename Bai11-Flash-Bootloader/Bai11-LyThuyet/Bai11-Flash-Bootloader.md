# Bài 11 - Flash - Bootloader

### 1. Flash
Flash là loại bộ nhớ không mất dữ liệu khi mất nguồn, có nghĩa là khi tắt nguồn, dữ liệu vẫn được giữ lại. Đây là nơi lưu trữ chương trình (firmware) và các dữ liệu cần thiết để thiết bị hoạt động. Flash trên STM32 được chia thành nhiều page nhỏ (mỗi page có kích thước 1KB), giúp dễ dàng quản lý việc lưu trữ. 

Bộ nhớ Flash không giống như RAM, nơi bạn có thể ghi đè dữ liệu mà không cần xóa trước. Flash được tổ chức thành các ô nhớ mà chỉ có thể được ghi từ trạng thái "trống" (0xFF). Vì vậy, nếu bạn muốn thay đổi dữ liệu, bạn phải xoá cả trang (page) đó trước khi ghi lại dữ liệu mới.

Flash hoạt động như sau:
- Trước khi ghi dữ liệu vào Flash, phải xoá toàn bộ page đó. Khi xoá, toàn bộ dữ liệu trong page sẽ được đặt về giá trị 0xFF (dạng nhị phân là tất cả các bit bằng 1).
- Ghi dữ liệu vào Flash chỉ có thể thực hiện theo từng khối nhỏ như Halfword (16-bit) hoặc Word (32-bit).
- Flash có giới hạn số lần xóa/ghi (thường là vài chục ngàn lần).

Ví Dụ: Lưu Trữ Biến: Nếu muốn lưu thông số của cảm biến mà không muốn mất dữ liệu khi thiết bị mất điện, có thể ghi dữ liệu đó vào bộ nhớ Flash.

```c
// Hàm để mở khoá bộ nhớ Flash
void Flash_Unlock(void) {
    if (LL_FLASH_IsLocked()) {
        LL_FLASH_Unlock();
    }
}

// Hàm để khoá lại bộ nhớ Flash
void Flash_Lock(void) {
    if (!LL_FLASH_IsLocked()) {
        LL_FLASH_Lock();
    }
}

// Hàm xóa một page trong Flash
void Flash_ErasePage(uint32_t PageAddress) {
    Flash_Unlock(); // Mở khóa Flash
    while (LL_FLASH_IsActiveFlag_BSY()); // Chờ nếu Flash đang bận
    
    // Kích hoạt chức năng xóa page
    LL_FLASH_ClearFlag_EOP(); // Xóa cờ EOP (End of Operation)
    LL_FLASH_EnableErase(); // Kích hoạt chức năng xóa
    LL_FLASH_SetPageAddress(PageAddress); // Đặt địa chỉ page cần xóa
    LL_FLASH_StartErase(); // Bắt đầu quá trình xóa
    
    while (LL_FLASH_IsActiveFlag_BSY()); // Chờ xóa hoàn thành
    LL_FLASH_DisableErase(); // Tắt chức năng xóa
    Flash_Lock(); // Khóa Flash lại
}

// Hàm ghi dữ liệu 16-bit vào Flash
void Flash_WriteHalfWord(uint32_t Address, uint16_t Data) {
    Flash_Unlock(); // Mở khóa Flash
    while (LL_FLASH_IsActiveFlag_BSY()); // Chờ nếu Flash đang bận
    
    LL_FLASH_ProgramHalfWord(Address, Data); // Ghi dữ liệu
    while (LL_FLASH_IsActiveFlag_BSY()); // Chờ ghi hoàn tất
    
    Flash_Lock(); // Khóa Flash lại
}
```

- ```Flash_Unlock()``` và ```Flash_Lock()```: Các hàm này sử dụng thư viện LL để mở khóa và khóa bộ nhớ Flash, cho phép hoặc ngăn chặn thao tác ghi/xóa.
- ```LL_FLASH_IsActiveFlag_BSY()```: Kiểm tra xem Flash có đang bận không. Nếu cờ này được bật, tức là có một quá trình ghi/xóa đang diễn ra, và phải chờ đến khi hoàn thành.
- ```LL_FLASH_EnableErase()``` và ```LL_FLASH_DisableErase()```: Kích hoạt hoặc tắt tính năng xóa page. Phải gọi LL_FLASH_StartErase() để thực hiện quá trình xóa.
- ```LL_FLASH_ProgramHalfWord()```: Ghi dữ liệu vào bộ nhớ Flash theo khối 16-bit (Halfword).

### 2. Bootloader
- Bootloader là chương trình đầu tiên chạy sau khi vi điều khiển khởi động. Nó giúp tìm và nhảy vào ứng dụng chính (firmware) lưu trữ trong Flash.
- Bootloader có nhiệm vụ chọn và chạy ứng dụng chính (firmware) từ bộ nhớ Flash.

Qúa trình khởi động:
- Khi STM32 được khởi động, nó kiểm tra giá trị của các chân BOOT0 và BOOT1 để xác định nơi khởi động chương trình.
- Giá trị khởi động này sẽ được lưu vào thanh ghi PC (Program Counter), từ đó STM32 biết địa chỉ để đọc và chạy lệnh.
- Vi xử lý sẽ tìm đến một vị trí được gọi là Reset_Handler, nơi chương trình khởi động hệ thống, sao chép dữ liệu từ Flash vào RAM, và cuối cùng chạy hàm main().

Bootloader có thể do nhà sản xuất cung cấp (giống như một BIOS trên máy tính), hoặc do người dùng tự viết để thực hiện các chức năng đặc biệt, như:
- Cập nhật firmware từ cổng UART (giao tiếp nối tiếp).
- Tìm và chạy các ứng dụng khác được lưu ở một địa chỉ cụ thể.

Ví Dụ Cụ Thể Với Bootloader: Giả sử có hai chương trình: một chương trình nhấp nháy đèn LED ("BlinkLed") và một bootloader. Bootloader sẽ được đặt tại địa chỉ 0x08000000 (địa chỉ khởi động mặc định của STM32). "BlinkLed" được lưu ở địa chỉ khác, chẳng hạn 0x08008000. MCU sẽ không tự động chạy BlinkLed, mà cần viết Bootloader để chỉ đến và khởi chạy chương trình đó.

```c
#define APPLICATION_ADDRESS 0x08008000

void JumpToApplication(void) 
{
    uint32_t appJumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
    void (*appResetHandler)(void) = (void (*)(void)) appJumpAddress;
    
    // Thiết lập lại Stack Pointer
    __set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);
    appResetHandler(); // Nhảy vào Reset Handler của ứng dụng chính
}
```
- ```APPLICATION_ADDRESS```: Địa chỉ nơi ứng dụng chính được lưu trong Flash.
- ```appJumpAddress```: Lấy địa chỉ của Reset Handler từ bảng vector ngắt của chương trình chính. Đây là nơi chương trình chính sẽ bắt đầu thực thi.
- ```__set_MSP()```: Cập nhật MSP bằng giá trị từ chương trình chính. Điều này đảm bảo rằng chương trình có một vùng nhớ ngăn xếp chính xác để hoạt động.
- ```appResetHandler()```: Đây là một con trỏ hàm được tạo ra để gọi Reset Handler của chương trình chính, bắt đầu thực thi chương trình chính.
