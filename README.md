# Note cho Đức về hệ thống

## Cài đặt

### Điều kiên tiên quyết

- Có sẵn môi trường Git, ESP-IDF, Nodejs (Nodejs tôi đã cài cho ông hôm trước)

### Các bước cài 

- Tạo 1 folder tên `wsn`, mở terminal bên trong folder đó và **clone** (hoặc **fork**) 2 projects về:


Clone (hoặc fork) các repos sau:

**Source code cho gateway-ESP32**:

```shell
git clone https://github.com/sonnt0201/wsn-gateway
```

**Source code cho server-giao diện:**

```shell
git clone https://github.com/sonnt0201/wsn-server
```

clone xong thì xuất hiện 2 folder `wsn-gateway` và `wsn-server`.

### Khởi động

Nạp code esp32:

- Vào folder `wsn-gateway/lib`, mở file `config.h`
- Thay giá trị `WIFI_SSID_DEFAULT` và `WIFI_PASSWORD_DEFAULT` cũ bằng tên wifi và mật khẩu wifi mình muốn kết nối. Đảm bảo máy tính chạy server và ESP32 kết nối cùng 1 wifi.
- Thay địa chỉ IPv4 của máy tính chứa server vào giá trị của `WEB_SERVER`. Lưu lại file.
- Build, flash và monitor source code đã config vào ESP32.

 Khởi động server:

 - Mở terminal trong thư mục `wsn-server`

 - Chạy lệnh 
 
 ```shell
 npm install
 ```

 để cài các thư viện cần thiết

 - build bản product:

 ```shell
npm run build
 ```
- chạy server và giao diện:

```shell
npm run start
```

Lưu ý disable firewall để gateway truyền tin được tới server.

Sau khi server khởi động, để mở giao diện, truy cập vào đường link [http://localhost:3000/dev](http://localhost:3000/dev)

## Hướng dẫn về gateway

### Cấu trúc source code

Folder `lib`: 
- Chứa file `config` để cấu hình khi build.
- Các file `easywifi`, `easytcp`,  `encoder` và `queue_service` là các thư viện tự build để cung cấp các hàm gọi tiện dụng: hàm kết nối wifi nhanh, mở cổng socket, mã hóa và vận chuyển gói tin giữa các `RTOS tasks`.

Folder `main`: Chứa hàm `main` để setup và gán các **task** vào các **core**:

- Code sử dụng FreeRTOS để phân chia 2 task vào 2 core. Task `lora_listener` được gán vào `core 0` để nghe bản tin lora gửi đến từ các node. Task `th_sender` được gắn vào `core 1` gửi dữ liệu nhiệt độ, độ ẩm và địa chỉ MAC tương ứng tới server. (TH có nghĩa là temperature-humidity)
- 2 core giao tiếp với nhau thống qua `queue_services` (`lib/queue_services.h`)

### Triển khai task `lora_listener`

Gần như Đức chỉ cần quan tâm 2 file:

- File `lib/config.h` để cài đặt wifi cũng như địa chỉ server để gateway gửi dữ liệu tới.

- File `main/main.c` trong đó:
    - Mình có thể thêm các setup tùy ý trong file `main.c` ở hàm `main` phần **OTHER SETUP** (Thường thì ông cũng không cần).
    - Mình triển khai hàm `lora_listener` (ở phía dưới hàm main). Đây là task được gắn vào core 0, sẽ có nhiệm vụ nghe bản tin lora gửi đến, giải mã (nếu cần) để lấy được MAC của node gửi, nhiệt độ và độ ẩm tại node đó, đông thời truyền giá trị này vào hàm `quick_send`.
    - Hàm `quick_send` gửi dữ liệu vừa đọc được tới server

```cpp
// Quickest way to send TH data to server
void quick_send(
    char* device_id, // 48 bits mac address as string with length of 12
    double temperature,
    int humidity
);
```
Các file còn lại tôi đã dựng sẵn và đủ chức năng.

---
9/2024 by Thai-Son Nguyen.

🧑‍💻🧑‍💻🧑‍💻 Happy coding !!! 🧑‍💻🧑‍💻🧑‍💻

```brainfuck
⣏⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⣩⣭⣽⣿⣿⣿⣿⣿⣿⣯⣭⣭⣭⣭⣭⣭⣉⠉⠉⠉⠉⠉⠉⠉⢹⡟⠛⠻⣭⣍⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⣩
⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣝⢿⣿⣷⣤⣄⠀⠀⠀⣸⠃⠀⠀⠀⠈⠛⡆⠀⠀⠀⠀⠀⠀⣠⠞⠁
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⡿⢿⣻⣉⣩⣭⣍⣙⣛⡿⢿⣿⣿⣿⣿⣷⣝⢿⣿⣿⣷⣄⠀⡿⠀⠀⠀⠀⢀⡼⠃⠀⠀⠀⠀⡠⠞⠁⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⡿⣛⣥⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣮⣟⡿⣿⣿⣿⣎⢿⣿⣿⣿⣿⢁⡀⠀⠀⣠⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⡿⣵⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣮⣿⣿⣿⡜⣿⣿⣿⠟⠋⠀⣠⡞⠁⠀⠀⠀⠀⠀⣀⣀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⢸⣿⣿⣿⡿⠟⢻⠽⠛⠛⠉⠉⠉⠉⠉⠉⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣟⠁⢀⣤⣾⣿⣷⠀⠀⣠⡤⠖⠛⢉⡏⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⡿⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠻⠿⣿⣿⣤⠇⢈⡏⠉⠀⠉⠉⠛⠛⠉⠁⠀⠀⠀⢸⠇⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⠏⠀⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⢽⣿⣿⣿⢿⣷⣦⣤⣤⣀⣀⣀⣤⠼⠃⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⠃⠀⠀⠀⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠞⠁⠀⠀⠀⠀⠈⢿⣿⣿⣏⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣟⣿⡇⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠖⠁⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⢸⣿⣿⣿⣽⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⡿⣽⣿⠀⠀⢀⣀⠤⠠⠤⢤⣙⡶⣄⠀⠀⡀⠀⡀⠀⠀⢀⣠⣴⡟⠁⠠⠦⢤⣀⡀⠀⠀⠀⠀⢸⣿⣿⣿⡇⣿⣿⣿⣧⢻⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⢷⣿⠇⠀⠀⢉⣀⣤⣶⣶⣦⣄⣉⠀⠀⠀⠁⠀⠀⠀⠀⢘⣻⣷⣶⢶⣾⣶⣤⣀⣉⠳⢤⡀⠀⢸⣿⣿⣿⡇⣿⣿⣿⣿⡟⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢠⣿⡟⣾⣿⠀⢠⣾⠿⠛⠉⣀⢠⣄⡉⠛⠳⢄⠀⠀⠀⠀⠀⠠⠞⠉⢀⣠⠄⠠⢤⣉⠛⠿⣿⣦⡀⠀⢸⣿⣿⣿⢷⣿⣿⣿⣿⣷⢻⣿⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣿⢯⣟⣻⠲⡿⠁⣠⡶⠊⠉⣀⠈⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⢠⡶⢋⡥⠠⣤⠀⠈⠳⣄⠈⢻⣿⠒⢾⣯⣽⣶⣿⢿⣿⣿⣿⣿⡼⣿⡇⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣏⣾⣿⣿⣼⠁⢠⡏⠀⣎⠁⢸⠃⠀⠈⢳⠀⠀⠀⠀⠀⠀⠀⣾⠁⢸⣄⣤⣼⠀⠀⠀⢸⡆⠀⢹⡇⢸⣿⣿⣿⣿⡀⠘⣿⣿⣿⣇⢻⡇⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⢿⣿⣿⣿⣷⠀⢻⣇⠀⠈⠹⣿⠇⠀⠀⣾⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⠈⠙⠛⠁⠀⠀⣼⠃⠀⠀⠀⢸⣿⣿⣿⣿⡙⠆⠘⣿⣿⣿⡼⣧⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣾⣿⣿⣿⣿⡄⢤⣻⣆⠀⠀⠀⠀⠀⢰⠏⠀⠀⠀⠀⠀⠀⠀⠘⣆⡀⠀⠀⠀⠀⠀⣠⠏⠀⠀⠀⠀⢸⣿⣿⣿⣿⠛⠁⠀⣿⣿⣿⣇⣿⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⡇⢮⣧⣌⡁⠀⠤⠤⠾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠐⠦⠤⠴⢚⣥⡾⠃⠀⠀⠀⣸⣿⣿⣿⣿⠀⠀⣼⣿⣿⣿⣿⢸⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⠋⠉⣽⢧⡾⠛⠻⣅⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⣿⣉⣭⠤⠤⣤⠀⠀⣿⣿⣿⣿⡿⠀⣰⣿⣿⣿⣿⣿⡿⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢰⣿⣿⠃⠀⣸⠁⢸⠁⠀⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉⢉⡟⠁⢠⠟⢸⠃⠀⡶⠘⡇⢸⣿⣿⣿⣿⣷⣶⣿⣿⣿⣿⣿⣿⣿⡁⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⣾⣿⡇⠀⢰⡇⠀⡏⠀⢠⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡾⠀⢀⡎⠀⡏⠀⢰⣷⣾⣿⣿⣿⣿⣿⣿⡏⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣿⣿⡇⠀⠸⣇⣀⣳⠀⠸⡇⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⢀⣀⣀⢸⡇⠀⢸⡇⢸⡇⠀⢸⣿⣝⣭⣾⣿⣿⣿⣿⣇⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣼⣿⣿⣿⣦⣀⠙⢿⣫⣧⣀⣹⣿⣿⣿⣒⣒⣒⣀⣀⣈⣉⣀⣀⣀⣀⠀⠘⣇⠀⠸⣿⣿⣷⠀⠸⡛⣻⣿⣹⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⢛⣿⣿⣿⣿⠉⠙⠻⣿⠛⠉⠉⠙⠛⠛⠛⠛⢙⣶⠤⠟⠚⠚⣷⣤⣿⣿⣏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡽⣿⣿⣿⣿⡄⠀⠀⠀⠀
⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣯⣿⣿⣋⣷⣻⣿⣿⣿⣿⠃⠀⠀⠀⠈⢷⡀⠀⠀⠀⠀⣀⠴⠋⠀⠀⠀⠀⣰⢿⣿⣿⣿⣹⣿⣿⢸⣿⡿⣿⣮⢻⣿⣿⣿⣇⢹⣿⣿⣿⣿⣄⠀⠀⠀
⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⡾⣫⣾⢟⣶⣿⣿⣿⠟⢹⡀⠀⠀⠀⠀⠀⠹⣆⢀⣠⠞⠁⠀⠀⠀⠀⣠⠞⠋⠀⣿⣿⡇⣿⣿⣿⣿⣿⢳⣿⣿⣷⣝⢿⣿⣿⡄⢿⣿⣿⣿⣿⣆⠀⠀
⠀⠀⠀⢠⣿⣿⣿⣿⣟⣠⣾⣿⣋⣿⣿⣿⡿⠃⡠⠈⢧⠀⠀⠀⠀⠀⢀⡠⠛⠁⠀⠀⠀⠀⢀⠜⠁⢀⡠⠞⢻⣿⢻⣿⣿⣿⡏⣿⣸⣿⣿⣿⣿⣧⡹⣿⡇⠘⣿⣿⣿⣿⣿⣧⠀
⠀⠀⣴⣿⣿⣿⣿⣟⣽⣿⣿⣳⣿⣿⣿⡏⣠⠎⠀⣠⢼⣧⠀⠀⣀⡴⠋⠀⠀⠀⠀⠀⢀⡴⠃⣠⠔⠋⠀⠀⣿⣿⢸⣿⣿⣿⣿⣸⣿⣿⣿⣿⣿⣿⣷⣽⡇⠀⢹⣿⣿⣿⣿⣿⡆
⣠⣾⣿⣿⣿⣿⢫⣾⣿⣿⣿⣿⣿⣿⣷⡷⢃⡴⠋⡵⢹⠘⣧⠖⠁⠀⠀⠀⠀⠀⢀⣴⠋⠉⠉⠹⡄⠀⠀⢠⠻⣿⣸⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠈⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⡿⣳⣿⣿⣿⣿⣿⣿⣿⣿⣟⡴⠋⢀⡼⠁⢸⠀⠈⢦⡀⠀⠀⠀⣠⡶⡻⡌⢳⡀⠀⠀⢳⠀⠀⣼⠀⣿⣇⢿⣿⣿⣿⣿⣦⢻⣿⣿⣿⣿⣿⣿⣿⣷⡄⢹⣿⣿⣿⣿⣿
⣿⣿⣿⠟⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢠⠏⠀⠀⢸⠀⠀⠀⠳⣀⡴⢻⠏⠀⡇⠹⣄⠙⣦⠀⠈⡆⠀⡟⠀⣿⣿⡞⣿⣿⣿⣿⣿⣇⢿⣿⣿⣿⣿⣿⣿⣿⣷⣤⣿⣿⣿⣿⣿
⣿⣿⢫⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡧⠴⣇⠀⠀⠀⡞⠀⣀⣴⢟⢁⣰⠃⠀⢸⠀⠀⠈⢧⡈⢳⡄⢳⢀⠃⠀⣿⣿⣿⡜⣿⣿⣿⣿⣿⣆⣿⣿⣿⣿⣿⣿⣿⣿⣿⡝⣿⣿⣿⣿
```





