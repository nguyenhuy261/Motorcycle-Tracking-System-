#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "BluetoothSerial.h"

Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

float Lat = 21.0285;
float Lng = 105.8542;

void setup(void) {
    Serial.begin(115200);
    
    // Khởi tạo Bluetooth
    if (!SerialBT.begin("ESP32_MPU6050")) {
        Serial.println("Khởi tạo Bluetooth không thành công");
        while (1);
    }
    Serial.println("Bluetooth đã sẵn sàng để kết nối!");

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
    delay(100);
}

void loop() {
    // Lấy các sự kiện cảm biến mới với các giá trị
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // Tạo chuỗi dữ liệu gia tốc để gửi
    String Data = String(a.acceleration.x) + "," + String(a.acceleration.y) + "," + String(a.acceleration.z) + "," + String(Lat) + "," + String(Lng);
    
    // Gửi dữ liệu gia tốc qua Bluetooth
    SerialBT.println(Data);
    
    // In ra dữ liệu lên Serial Monitor
    Serial.print("Sending: ");
    Serial.println(Data);
    
    delay(20); // Gửi dữ liệu mỗi giây
}
