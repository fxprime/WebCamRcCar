//WWW.arduinoNa.com
#include "esp_camera.h"
#include <WiFi.h>


//ต่อ WiFi ไหน รหัสอะไรใส่ตรงนี้
const char* ssid = "LaVa@TOT";
const char* password = "0887882667";

//วิธีการ Upload
//1. ให้แน่ใจว่าเวอร์ชั่นของบอร์ด ESP32 เป็น 1.0.2 ไม่อย่างนั้นจะหา lib "esp_camera.h" ไม่เจอ
//โดยการคลิ๊ก Tools -> Board -> Board manager พิมพ์ esp32 ในช่องค้นหา
//เลือก esp32 version 1.0.2 แล้ว install
//2. เลือกบอร์ดเป็น ESP32 Wrover Module (Tools->Board->ESP32 Wrover Module)
//3. Partition scheme เลือกเป็น hugh app (Tools->Partition scheme->Hugh app)
//4. วิธีการ Upload เข้าโหมด Flash mode โดยการต่อพิน IO0 กับ GND เข้าด้วยกัน แล้วกดปุ่ม reset จากนั้นกด Upload 
//5. หลังอัพโหลดเสร็จ ให้เอาพิน IO0 ที่ต่อกับ GND ออก แล้วresetอีกครั้ง แล้วคลิ๊กที่ monitor ควร

//Pin ต่างๆที่ใช้กับกล้อง
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// GPIO สำหรับมอเตอร์ 
extern int gpLs =  16; // Left Wheel speed
extern int gpLb =  2; // Left Wheel Back
extern int gpLf = 14; // Left Wheel Forward
extern int gpRs = 15; // Right Wheel speed
extern int gpRb = 13; // Right Wheel Back
extern int gpRf = 12; // Right Wheel Forward
extern int gpLed =  4; // Light
extern String WiFiAddr ="";

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  
  pinMode(gpLs, OUTPUT); //Left Backward
  pinMode(gpRs, OUTPUT); //Left Forward
  digitalWrite(gpLs, HIGH);
  digitalWrite(gpRs, HIGH);

  pinMode(gpLb, OUTPUT); //Left Backward
  pinMode(gpLf, OUTPUT); //Left Forward
  pinMode(gpRb, OUTPUT); //Right Forward
  pinMode(gpRf, OUTPUT); //Right Backward
  pinMode(gpLed, OUTPUT); //Light

  //initialize
  digitalWrite(gpLb, LOW);
  digitalWrite(gpLf, LOW);
  digitalWrite(gpRb, LOW);
  digitalWrite(gpRf, LOW);
  digitalWrite(gpLed, LOW);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  //ต่อ WiFi 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //เปิดเซิฟเวอร์สำหรับฟีดภาพจากกล้อง
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  WiFiAddr = WiFi.localIP().toString();
  Serial.println("' to connect");
}

void loop() {
  // put your main code here, to run repeatedly:

}
