#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Thông tin mạng Wi-Fi
const char* ssid = "TP-Link_3C11"; // Thay bằng tên Wi-Fi của bạn
const char* password = "00099769"; // Thay bằng mật khẩu Wi-Fi của bạn

// Cổng và địa chỉ IP của thiết bị Android
const char* androidIp = "192.168.0.107"; // Thay bằng địa chỉ IP của thiết bị Android
const int androidPort = 2205; // Cổng mà Android lắng nghe

WiFiUDP udp;
Adafruit_MPU6050 mpu;


// Biến để lưu trữ các giá trị cảm biến
float Lat = 20.9716; // Giá trị latitude
float Lng = 105.7870; // Giá trị longitude

void setup() {
    Serial.begin(115200);
    
    // Kết nối đến Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Thử khởi tạo MPU6050
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");
    
    // Đặt dải gia tốc là +-8G
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    // Đặt dải gyro là +- 500 deg/s
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
    // Lấy các sự kiện cảm biến mới với các giá trị
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Tạo thông điệp dưới dạng chuỗi
     String Data = String(a.acceleration.x) + "," + String(a.acceleration.y) + "," + String(a.acceleration.z) + "," + String(Lat) + "," + String(Lng);

    // Gửi dữ liệu đến ứng dụng Android qua UDP
    udp.beginPacket(androidIp, androidPort);
    udp.print(Data);
    udp.endPacket();


    Serial.println("Data sent: " + Data);
    delay(20);
}
