# Bài 4: Communication Protocols 

### 1. Truyền nhận dữ liệu

Tất cả các hệ thống nhúng đều liên quan đến một số loại giao tiếp giữa các thành phần - để lập trình, cảm biến hoặc trao đổi dữ liệu. Các quy tắc để giao tiếp được gọi là giao thức giao tiếp.

Truyền nhận dữ liệu trong vi điều khiển (MCU) là quá trình trao đổi tín hiệu điện áp giữa các chân (pin) của MCU

### 2. SPI
SPI (Serial Peripheral Interface), là chuẩn giao tiếp được phát triển bởi Motorola vào giữa những năm 1980, là một giao thức truyền thông nối tiếp tốc độ cao dùng để kết nối vi điều khiển với các thiết bị ngoại vi. SPI sử dụng nhiều dây hơn I2C, bao gồm: MOSI (Master Out Slave In), MISO (Master In Slave Out), SCK (Serial Clock), và SS/CS (Slave Select/Chip Select).

- Truyền dữ liệu đồng bộ: Dữ liệu được truyền đồng bộ với tín hiệu đồng hồ (SCK).
- Full-duplex: Có thể truyền và nhận dữ liệu đồng thời qua các dây riêng biệt (MOSI và MISO).
- Chế độ đa slave: Có thể kết nối nhiều slave với một master, nhưng cần sử dụng các chân CS/SS riêng biệt cho mỗi slave để chọn lựa.
- Tốc độ cao: Có thể đạt đến hàng chục MHz, nhanh hơn nhiều so với I2C.

#### Ưu điểm:
- Tốc độ cao: Phù hợp cho các ứng dụng yêu cầu tốc độ truyền dữ liệu lớn.
- Full-duplex: Có thể truyền và nhận dữ liệu đồng thời, tăng hiệu suất truyền thông.
- Đơn giản trong giao thức: Không cần địa chỉ hóa thiết bị, không phức tạp như I2C.

#### Nhược điểm:
- Sử dụng nhiều dây dẫn hơn: Cần tối thiểu 4 dây, và thêm dây cho mỗi slave khi kết nối nhiều thiết bị.
- Khả năng kết nối hạn chế: Mỗi slave cần một chân CS riêng, do đó số lượng slave bị giới hạn bởi số chân của master.
- Không hỗ trợ nhiều master: SPI chỉ hoạt động với một master trên bus, không hỗ trợ nhiều master như I2C.

SPI thường yêu cầu bốn chân cho mỗi thiết bị - một chân để thiết bị chính gửi dữ liệu đến thiết bị phụ, một chân để thiết bị phụ gửi dữ liệu đến thiết bị chính, một chân để chọn thiết bị phụ mà thiết bị chính muốn giao tiếp và một chân để đồng bộ hóa truyền dữ liệu. Các chân này được trình bày cụ thể như sau:

- SCK (Serial Clock): Thiết bị Master tạo xung tín hiệu SCK và cung cấp cho Slave.
- MISO (Master Input Slave Output): Tín hiệu tạo bởi thiết bị Slave và nhận bởi thiết bị Master.
- MOSI (Master Output Slave Input): Tín hiệu tạo bởi thiết bị Master và nhận bởi thiết bị Slave. 
- SS (Đôi khi CS- Slave Select/Chip Select): Chọn thiết bị Slave cụ thể để giao tiếp. Để chọn Slave giao tiếp thiết bị Master chủ động kéo đường SS tương ứng xuống mức 0 (Low). 

#### Quá trình giao tiếp SPI
- Bắt đầu quá trình, master sẽ kéo chân CS của slave muốn giao tiếp xuống 0 để báo hiệu muốn truyền nhận.
- Clock sẽ được cấp bởi master, tùy vào chế độ được cài, với mỗi xung clock,  1 bit sẽ được truyền từ master đến slave và slave cũng truyền 1 bit cho master.
- Các thanh ghi cập nhật giá trị và dịch 1 bit.
- Lặp lại quá trình trên đến khi truyền xong 8 bit trong thanh ghi.

### 2. I2C
I2C (Inter-Integrated Circuit), được phát triển bởi Philips Semiconductor, là một giao thức truyền thông nối tiếp được sử dụng để kết nối các thiết bị ngoại vi với vi điều khiển. Nó chỉ sử dụng hai dây dẫn: SDA (dữ liệu) và SCL (đồng hồ) để truyền dữ liệu giữa các thiết bị.

- Đa thiết bị (Multi-Master/Slave): Có thể kết nối nhiều master và nhiều slave trên cùng một bus.
- Tốc độ: Hỗ trợ nhiều tốc độ truyền (100 kHz, 400 kHz, 1 MHz, hoặc cao hơn).
- Truyền dữ liệu 2 chiều: Dữ liệu được truyền đi và nhận lại qua cùng một dây (SDA).
- Địa chỉ hóa thiết bị: Mỗi thiết bị trên bus có địa chỉ riêng để master nhận diện và giao tiếp.

#### Ưu điểm:
- Tiết kiệm chân: Chỉ cần 2 dây dẫn cho cả dữ liệu và đồng hồ.
- Đơn giản và dễ kết nối: Phù hợp cho các thiết bị có khoảng cách ngắn và yêu cầu kết nối đơn giản.
- Khả năng giao tiếp đa thiết bị: Cho phép nhiều thiết bị giao tiếp trên cùng một bus.

#### Nhược điểm:
- Khoảng cách giới hạn: Chỉ hiệu quả với các khoảng cách ngắn (thường dưới 1 mét).
- Tốc độ giới hạn: Không nhanh như các giao thức khác như SPI, đặc biệt ở các ứng dụng yêu cầu tốc độ cao.
- Xung đột dữ liệu: Có thể xảy ra xung đột khi có nhiều master trên bus (nếu không được xử lý tốt).

I2C sử dụng hai dây chính để truyền và nhận dữ liệu giữa các thiết bị:
- SDA (Serial Data): Dây dữ liệu nối tiếp, dùng để truyền và nhận dữ liệu giữa các thiết bị.
- SCL (Serial Clock): Dây đồng hồ, tạo tín hiệu đồng hồ để đồng bộ hóa quá trình truyền dữ liệu.

Quá trình truyền nhận dữ liệu giữa MCU và thiết bị sử dụng I2C:
- Khởi động (Start Condition): Master kéo dây SDA từ mức cao xuống thấp trong khi SCL vẫn ở mức cao. Điều này báo hiệu bắt đầu một giao tiếp I2C.
- Địa chỉ hóa thiết bị (Addressing): Master gửi một byte địa chỉ (7 bit hoặc 10 bit) để xác định slave nào sẽ tham gia giao tiếp. Bit thứ 8 (bit cuối cùng) là bit Read/Write:
    - 0: Master sẽ ghi dữ liệu vào slave.
    - 1: Master sẽ đọc dữ liệu từ slave.

- Xác nhận (ACK/NACK):
    - Slave có địa chỉ trùng khớp sẽ kéo SDA xuống mức thấp (ACK) để xác nhận rằng nó đã nhận được địa chỉ.
    - Nếu không có slave nào phản hồi (SDA vẫn ở mức cao), master sẽ nhận biết đây là NACK (Negative Acknowledgement).

- Truyền dữ liệu:
    - Master và slave sẽ truyền dữ liệu theo các byte. Sau mỗi byte, thiết bị nhận (có thể là master hoặc slave) sẽ gửi một bit ACK để xác nhận đã nhận dữ liệu thành công.
    - Nếu thiết bị nhận không kéo SDA xuống thấp (NACK), giao tiếp sẽ dừng lại.

- Kết thúc (Stop Condition): Khi master kết thúc quá trình truyền, nó sẽ kéo SDA từ mức thấp lên mức cao trong khi SCL vẫn ở mức cao. Đây là tín hiệu kết thúc giao tiếp.

Tóm tắt các bước của quá trình truyền nhận I2C:
Start Condition -> Địa chỉ hóa thiết bị -> ACK từ slave -> Truyền/nhận dữ liệu -> ACK từ thiết bị nhận -> Stop Condition

### 3. UART

UART (Universal Asynchronous Receiver/Transmitter) là một giao thức truyền thông nối tiếp không đồng bộ, dùng để trao đổi dữ liệu giữa các thiết bị, thường là giữa vi điều khiển và các thiết bị ngoại vi hoặc máy tính.
- Không đồng bộ: Không cần tín hiệu đồng hồ chung giữa các thiết bị; tốc độ truyền (baud rate) của hai thiết bị phải được cài đặt giống nhau.
- Truyền dữ liệu nối tiếp: Dữ liệu được truyền bit từng bit qua một dây, thường bắt đầu với bit start, các bit dữ liệu, và kết thúc bằng bit stop.
- Full-duplex: Cho phép truyền và nhận dữ liệu đồng thời trên hai dây riêng biệt (TX và RX).

 

#### Ưu điểm:
- Đơn giản và phổ biến: Dễ thiết lập và sử dụng, có mặt trong hầu hết các vi điều khiển.
- Không cần đồng hồ chung: Giảm thiểu số dây kết nối.
- Truyền full-duplex: Có thể truyền và nhận dữ liệu đồng thời.

#### Nhược điểm:
- Tốc độ giới hạn: Không nhanh bằng các giao thức đồng bộ như SPI.
- Khoảng cách truyền giới hạn: Hiệu quả ở khoảng cách ngắn, thường là vài mét.
- Yêu cầu cài đặt tốc độ giống nhau: Cả hai thiết bị phải có cùng baud rate để giao tiếp thành công.

UART sử dụng hai dây chính để truyền và nhận dữ liệu giữa các thiết bị, thường là giữa một vi điều khiển (MCU) và các thiết bị ngoại vi hoặc một MCU khác.
- TX (Transmit): Dây truyền, gửi dữ liệu từ thiết bị này đến thiết bị khác.
- RX (Receive): Dây nhận, nhận dữ liệu từ thiết bị khác.

Quá trình truyền nhận dữ liệu giữa MCU dùng UART:
- Cấu hình tốc độ baud: Cả hai thiết bị phải được cấu hình cùng một baud rate (tốc độ truyền dữ liệu, ví dụ: 9600, 115200 bps) để đảm bảo dữ liệu được truyền và nhận đúng thời gian.
- Khởi động (Start Bit): Khi bắt đầu truyền dữ liệu, thiết bị gửi sẽ kéo tín hiệu TX từ mức cao (idle) xuống mức thấp (0). Bit này báo hiệu rằng một byte dữ liệu mới sắp được truyền.
- Truyền các bit dữ liệu: Dữ liệu được truyền đi theo từng bit, thường là 8 bit (có thể là 5, 6, 7 hoặc 9 bit tùy theo cấu hình). Dữ liệu được truyền theo thứ tự LSB (Least Significant Bit) trước.
- Bit chẵn lẻ (Parity Bit) (nếu có): Đây là bit tùy chọn để kiểm tra lỗi trong quá trình truyền. Có thể là odd parity (chẵn lẻ lẻ) hoặc even parity (chẵn lẻ chẵn). Nếu không sử dụng, quá trình truyền sẽ bỏ qua bit này.
- Bit dừng (Stop Bit): Sau khi truyền xong các bit dữ liệu và (nếu có) bit chẵn lẻ, một hoặc hai bit dừng (1 hoặc 2 bit) được gửi để báo hiệu kết thúc một byte dữ liệu. Stop bit đưa mức tín hiệu trở lại mức cao (idle) để cho biết dữ liệu đã hoàn thành.

- Quá trình nhận dữ liệu:
    - Thiết bị nhận theo dõi dây RX để phát hiện bit start (mức thấp), sau đó đọc các bit dữ liệu theo thời gian tương ứng với tốc độ baud đã cấu hình trước đó.
    - Dữ liệu được ghi nhận và kiểm tra lỗi (nếu sử dụng parity bit) trước khi tiếp tục đọc byte tiếp theo.