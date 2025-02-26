# Flappy Bird trên ESP32 và OLED

Chào mừng bạn đến với dự án Flappy Bird đơn giản được xây dựng trên nền tảng ESP32 và màn hình OLED 128x64, sử dụng thư viện U8g2!


## Mô tả dự án

Dự án này là phiên bản thu nhỏ của trò chơi Flappy Bird kinh điển, được lập trình cho vi điều khiển ESP32 và hiển thị trên màn hình OLED đơn sắc. Mục tiêu của dự án là cung cấp một ví dụ code dễ hiểu, thú vị để học tập và thực hành về:

*   Lập trình vi điều khiển ESP32 với Arduino IDE.
*   Sử dụng thư viện U8g2 để điều khiển màn hình OLED.
*   Tạo trò chơi đồ họa đơn giản trên phần cứng nhúng.
*   Cơ bản về lập trình game logic (trọng lực, va chạm, di chuyển đối tượng).

Dự án này phù hợp cho:

*   Những người mới bắt đầu học lập trình ESP32 và Arduino.
*   Học sinh, sinh viên muốn tìm hiểu về lập trình game nhúng.
*   Những người yêu thích DIY điện tử và muốn tạo ra các dự án cá nhân thú vị.

## Tính năng chính

*   **Hiển thị đồ họa trên màn hình OLED 128x64**: Sử dụng màn hình OLED đơn sắc để hiển thị trò chơi một cách rõ ràng và tiết kiệm năng lượng.
*   **Đồ họa đơn giản**: Các đối tượng trong game (chim, ống) được vẽ bằng các hình dạng cơ bản, dễ hiểu và dễ tùy chỉnh.
*   **Điều khiển bằng nút nhấn**:  Chơi game chỉ với một nút nhấn đơn giản để điều khiển chim bay lên.
*   **Tính điểm**: Theo dõi điểm số khi chim vượt qua các ống, tạo động lực cạnh tranh.
*   **Màn hình Bắt đầu và Game Over**: Giao diện trực quan, dễ dàng bắt đầu và kết thúc trò chơi.
*   **Độ khó có thể tùy chỉnh**: Dễ dàng thay đổi các hằng số trong code để điều chỉnh độ khó của trò chơi (tốc độ ống, khoảng trống, lực nhảy, ...).

## Phần cứng cần thiết

*   **ESP32 DevKitC V4 hoặc board ESP32 tương tự**: Vi điều khiển chính để chạy chương trình.
*   **Màn hình OLED 0.96 inch, 128x64 pixel, giao tiếp I2C**: Màn hình hiển thị trò chơi. (Ví dụ:  [Ví dụ màn hình OLED 0.96 inch I2C](https://www.google.com/search?q=oled+0.96+inch+i2c))
*   **Nút nhấn**: Để điều khiển chim vỗ cánh/nhảy lên.
*   **Dây nối**: Để kết nối các module với ESP32.

## Phần mềm cần thiết

*   **Arduino IDE**: Môi trường lập trình để viết và tải code lên ESP32. ([Tải Arduino IDE](https://www.arduino.cc/en/software))
*   **Thư viện U8g2 cho Arduino**: Thư viện đồ họa để điều khiển màn hình OLED.

    **Hướng dẫn cài đặt thư viện U8g2 trong Arduino IDE:**
    1.  Mở Arduino IDE.
    2.  Vào menu `Sketch` -> `Include Library` -> `Manage Libraries...`
    3.  Tìm kiếm "U8g2" trong Library Manager.
    4.  Chọn thư viện `U8g2 by olikraus` và nhấn `Install`.
    5.  Đợi quá trình cài đặt hoàn tất.

## Kết nối phần cứng

Kết nối màn hình OLED và nút nhấn với ESP32 theo sơ đồ sau. **Lưu ý**: Các chân GPIO có thể khác nhau tùy theo board ESP32 bạn sử dụng. Hãy kiểm tra sơ đồ chân (pinout diagram) của board ESP32 của bạn để xác định chính xác các chân SDA, SCL và chân GPIO phù hợp cho nút nhấn.

**Kết nối màn hình OLED (I2C):**

| Màn hình OLED | ESP32 (Ví dụ: ESP32 DevKitC V4) |
| :------------ | :--------------------------------- |
| VCC          | 3.3V                               |
| GND          | GND                                |
| SDA          | GPIO21 (SDA)                         |
| SCL          | GPIO22 (SCL)                         |

**Kết nối nút nhấn:**

*   Một chân của nút nhấn kết nối với **GPIO [Bạn chọn chân GPIO, ví dụ GPIO0 hoặc GPIO2, thay đổi `#define BUTTON_PIN` trong code cho phù hợp]** trên ESP32.
*   Chân còn lại của nút nhấn kết nối với **GND**.
*   Sử dụng **điện trở kéo lên nội bộ** cho chân GPIO nút nhấn trong code Arduino ( `pinMode(BUTTON_PIN, INPUT_PULLUP);`).

## Hướng dẫn sử dụng

1.  **Cài đặt thư viện U8g2** vào Arduino IDE (theo hướng dẫn ở trên).
2.  **Kết nối phần cứng** theo sơ đồ kết nối.
3.  **Mở file code Arduino (`.ino`)** trong Arduino IDE.
4.  **Chọn board ESP32** của bạn trong menu `Tools` -> `Board` -> `ESP32 Arduino` (hoặc tương tự).
5.  **Chọn cổng COM** mà ESP32 của bạn đang kết nối trong menu `Tools` -> `Port`.
6.  **Nhấn nút `Upload`** để tải code lên ESP32.
7.  **Chờ quá trình upload hoàn tất**.
8.  **Nhấn nút nhấn** để bắt đầu chơi game Flappy Bird trên màn hình OLED!

## Tùy chỉnh độ khó và các thông số

Bạn có thể dễ dàng tùy chỉnh độ khó và một số yếu tố khác của trò chơi bằng cách thay đổi các hằng số được định nghĩa ở đầu code Arduino:

*   `#define PIPE_SPEED 2`: Tốc độ di chuyển của ống (tăng giá trị để ống di chuyển nhanh hơn, game khó hơn).
*   `#define PIPE_GAP 25`: Khoảng trống giữa ống trên và ống dưới (giảm giá trị để khoảng trống hẹp hơn, game khó hơn).
*   `#define GRAVITY 1`:  Trọng lực tác động lên chim (tăng giá trị để chim rơi nhanh hơn).
*   `#define JUMP_STRENGTH -7`: Lực nhảy của chim khi nhấn nút (giảm giá trị tuyệt đối để chim nhảy lên ít hơn).
*   `#define BIRD_SIZE 8`: Kích thước của chim.
*   `#define PIPE_WIDTH 15`: Chiều rộng của ống.

Hãy thử thay đổi các giá trị này và xem trò chơi thay đổi như thế nào!

## Các cải tiến có thể

Đây là một phiên bản Flappy Bird rất cơ bản. Bạn có thể cải tiến dự án này theo nhiều hướng:

*   **Thêm đồ họa chi tiết hơn**: Vẽ chim và ống bằng các bitmap hoặc hình dạng phức tạp hơn, thay vì chỉ hình tròn và hình hộp chữ nhật.
*   **Hiệu ứng âm thanh**: Thêm âm thanh khi chim vỗ cánh, va chạm, ghi điểm để tăng tính hấp dẫn.
*   **Sử dụng module RTC (Real-Time Clock)**:  Tích hợp module RTC để hiển thị thời gian thực trên màn hình khi không chơi game.
*   **Thêm các chế độ chơi khác nhau**: Ví dụ: chế độ chơi khó hơn với tốc độ ống tăng dần, hoặc chế độ chơi "endless" không có màn hình Game Over.
*   **Bảng xếp hạng trực tuyến (nâng cao)**: Nếu muốn thử thách hơn, bạn có thể tìm cách kết nối ESP32 với internet và tạo bảng xếp hạng điểm số trực tuyến.



## Tác giả

[**Kiều Văn Sơn**]

Chúc bạn có thời gian vui vẻ với dự án Flappy Bird ESP32 OLED này! Hãy thoải mái chia sẻ, tùy chỉnh và phát triển nó thêm nữa!
