// #include <Arduino.h>
// #include <Wire.h>          // 硬件IIC驱动库
// #include <WiFi.h>          // Wifi支持库
// #include <PubSubClient.h>  // MQTT支持库
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BMP280.h> // BMP280支持库

// /*********************************网络配置*********************************/
// const char* ssid = "zyq";                  // wifi名
// const char* password = "12345678";         // wifi密码
// const char* mqtt_server = "broker.emqx.io"; //公共
// const int port = 1883;                     // 端口号


// WiFiClient espClient;            // 创建一个WIFI连接客户端
// PubSubClient client(espClient);  // 创建一个PubSub客户端, 传入创建的WIFI客户端
// unsigned long lastMsg = 0;
// // MQTT主题
// const char* tempTopic = "esp32/temperature11";
// const char* pressureTopic = "esp32/pressure11";

// int postMsgId = 0;  // 记录已经post了多少条数据

// /*********************************BMP280配置*********************************/
// Adafruit_BMP280 bmp(&Wire1);
// float temperature;
// float pressure;

// void setupWifi() {
//     delay(10);
//     Serial.println();
//     Serial.print("连接到 ");
//     Serial.println(ssid);

//     WiFi.begin(ssid, password);

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println("");
//     Serial.println("WiFi 已连接");
//     Serial.println("IP 地址: ");
//     Serial.println(WiFi.localIP());
// }
// void reconnect() {
//     while (!client.connected()) {
//         Serial.print("尝试MQTT连接...");
//         String clientId = "ESP32Client-";
//         clientId += String(random(0xffff), HEX);

//         if (client.connect(clientId.c_str())) {
//             Serial.println("已连接");
//         }
//         else {
//             Serial.print("失败, rc=");
//             Serial.print(client.state());
//             Serial.println(" 5秒后重试");
//             delay(5000);
//         }
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     delay(1000);

//     // 初始化 BMP280
//     Wire1.begin(20, 19);
//     if (!bmp.begin(0x76)) {  // 默认 I2C 地址为 0x76
//         Serial.println("Could not find a valid BMP280 sensor, check wiring!");
//         while (1) delay(10);
//     }

//     setupWifi();
//     client.setServer(mqtt_server, port);
//     delay(1000);
// }

// void loop() {
//     if (!client.connected()) {
//         reconnect();
//     }
//     client.loop();

//     unsigned long now = millis();
//     if (now - lastMsg > 5000) {
//         lastMsg = now;

//         // 读取温度和湿度
//         float temp = bmp.readTemperature();
//         float press = bmp.readPressure() / 100;

//         if (isnan(temp) || isnan(press)) {
//             Serial.println(F("从BMP传感器读取失败!"));
//             return;
//         }

//         // 转换为字符串并发送
//         char tempString[8];
//         char pressString[8];
//         dtostrf(temp, 1, 2, tempString);
//         dtostrf(press, 1, 2, pressString);

//         Serial.print("温度: ");
//         Serial.print(tempString);
//         Serial.print(" C, 气压: ");
//         Serial.print(pressString);
//         Serial.println(" hpa");

//         client.publish(tempTopic, tempString);
//         client.publish(pressureTopic, pressString);

//     }
//     delay(3000);  // 每5秒上传一次数据
// }