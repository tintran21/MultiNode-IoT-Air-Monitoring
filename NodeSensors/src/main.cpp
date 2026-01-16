#include "config.h"
#include "network_manager.h"
#include "sensors.h"
#include "data_sender.h"

/* GLOBAL VARIABLES */
unsigned long lastSensorRead = 0;
unsigned long lastReconnect = 0;
bool isConnectedToMaster = false;

void setup() 
{
    Serial.begin(115200);
    delay(1000);

    dht.begin();

    pinMode(LDR_PIN, INPUT);
    pinMode(MQ2_PIN, INPUT);
    pinMode(MQ135_PIN, INPUT);
    analogReadResolution(12);

    connectWiFi();
}

void loop() 
{
    if (WiFi.status() != WL_CONNECTED) 
    {
        WiFi.reconnect();
        delay(5000);
        return;
    }

    if (!tcpClient.connected()) {
        if (millis() - lastReconnect >= RECONNECT_INTERVAL) 
        {
            lastReconnect = millis();
            connectToMaster();
        }
    }

    if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) 
    {
        lastSensorRead = millis();
        readAllSensors();
        sendDataToMaster();
    }

    delay(10);
}










































// #include <Arduino.h>
// #include <WiFi.h>
// #include <ArduinoJson.h>
// #include <DHT.h>


// #define LDR_PIN         0       
// #define MQ2_PIN         1       
// #define MQ135_PIN       4       
// #define DHT_PIN         5       // DHT22 data pin
// #define DHT_TYPE        DHT22


// // Connect to Master AP
// const char* WIFI_SSID = "ESP32_MASTER_AP";
// const char* WIFI_PASS = "12345678";

// // Master TCP Server
// const char* SERVER_IP = "192.168.4.1";      // Master AP default IP
// const uint16_t SERVER_PORT = 8888;

// #define SENSOR_READ_INTERVAL    2000    // Read sensors every 2s
// #define RECONNECT_INTERVAL      5000    // Try reconnect every 5s

// DHT dht(DHT_PIN, DHT_TYPE);
// WiFiClient tcpClient;

// unsigned long lastSensorRead = 0;
// unsigned long lastReconnect = 0;
// bool isConnectedToMaster = false;

// // Sensor data
// typedef struct {
//     float temperature;
//     float humidity;
//     uint16_t ldr;
//     uint16_t mq2;
//     uint16_t mq135;
// } sensor_data_t;

// sensor_data_t sensorData = {0};

// void connectToMaster();
// void readAllSensors();
// void sendDataToMaster();

// void setup()
// {
//     Serial.begin(115200);
//     delay(1000);
    
//     Serial.println("\n========================================");
//     Serial.println("   ESP32-C3 Node Sensors");
//     Serial.println("   Air Quality Monitoring System");
//     Serial.println("========================================\n");
    
//     // Init DHT22
//     dht.begin();
//     Serial.println("✓ DHT22 initialized on GPIO 4");
    
//     // Init ADC pins (analog inputs)
//     pinMode(LDR_PIN, INPUT);
//     pinMode(MQ2_PIN, INPUT);
//     pinMode(MQ135_PIN, INPUT);
    
//     // Set ADC resolution to 12-bit (0-4095)
//     analogReadResolution(12);
    
//     Serial.println("✓ ADC pins initialized:");
//     Serial.println("  - LDR (GPIO 1)");
//     Serial.println("  - MQ2 (GPIO 2)");
//     Serial.println("  - MQ135 (GPIO 3)");
    
//     // Connect to Master WiFi AP
//     Serial.print("\nConnecting to Master AP: ");
//     Serial.println(WIFI_SSID);
    
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(WIFI_SSID, WIFI_PASS);
    
//     int attempts = 0;
//     while (WiFi.status() != WL_CONNECTED && attempts < 30) //max 30times try to connect
//     {
//         delay(500);
//         Serial.print(".");
//         attempts++;
//     }
    
//     if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("\n✓ WiFi connected to Master AP");
//         Serial.print("✓ Local IP: ");
//         Serial.println(WiFi.localIP());
//     } else {
//         Serial.println("\n✗ WiFi connection failed!");
//         Serial.println("  Check if Master AP is running");
//     }
    
//     Serial.println("\n========================================");
//     Serial.println("✓ ESP32-C3 Sensor Node Ready!");
//     Serial.println("✓ Waiting to connect to Master...");
//     Serial.println("========================================\n");
// }

// void loop()
// {
//     // Check WiFi connection
//     if (WiFi.status() != WL_CONNECTED) {
//         Serial.println("✗ WiFi disconnected, reconnecting...");
//         WiFi.reconnect();
//         delay(5000);
//         return;
//     }
    
//     // Check TCP connection to Master
//     if (!tcpClient.connected()) 
//     {
//         isConnectedToMaster = false;
        
//         // Try to reconnect every 5 seconds
//         if (millis() - lastReconnect >= RECONNECT_INTERVAL) 
//         {
//             lastReconnect = millis();
//             connectToMaster();
//         }
//     } 
//     else 
//     {
//         isConnectedToMaster = true;
//     }
    
//     // Read sensors and send data every 2 seconds
//     if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
//         lastSensorRead = millis();
        
//         readAllSensors();
        
//         if (isConnectedToMaster) {
//             sendDataToMaster();
//         } else {
//             Serial.println("⚠ Not connected to Master - skipping send");
//         }
//     }
    
//     delay(10);
// }

// void connectToMaster()
// {
//     Serial.print("Connecting to Master TCP Server (");
//     Serial.print(SERVER_IP);
//     Serial.print(":");
//     Serial.print(SERVER_PORT);
//     Serial.println(")...");
    
//     if (tcpClient.connect(SERVER_IP, SERVER_PORT)) {
//         Serial.println("✓ Connected to Master!");
//         isConnectedToMaster = true;
//     } else {
//         Serial.println("✗ Connection to Master failed");
//         isConnectedToMaster = false;
//     }
// }

// void readAllSensors()
// {
//     // Read DHT22
//     sensorData.temperature = dht.readTemperature();
//     sensorData.humidity = dht.readHumidity();
    
//     // Check DHT read errors
//     if (isnan(sensorData.temperature)) {
//         sensorData.temperature = 0.0;
//         Serial.println("✗ DHT22 temperature read error");
//     }
//     if (isnan(sensorData.humidity)) {
//         sensorData.humidity = 0.0;
//         Serial.println("✗ DHT22 humidity read error");
//     }
    
//     // Read analog sensors (12-bit ADC: 0-4095)
//     sensorData.ldr = analogRead(LDR_PIN);
//     sensorData.mq2 = analogRead(MQ2_PIN);
//     sensorData.mq135 = analogRead(MQ135_PIN);
    
//     // Debug output
//     Serial.printf("Sensors: T=%.1f°C H=%.1f%% LDR=%d MQ2=%d MQ135=%d\n",
//                   sensorData.temperature,
//                   sensorData.humidity,
//                   sensorData.ldr,
//                   sensorData.mq2,
//                   sensorData.mq135);
// }

// void sendDataToMaster()
// {
//     // Create JSON document
//     StaticJsonDocument<256> doc;
    
//     doc["temp"] = sensorData.temperature;
//     doc["humi"] = sensorData.humidity;
//     doc["ldr"] = sensorData.ldr;
//     doc["mq2"] = sensorData.mq2;
//     doc["mq135"] = sensorData.mq135;
//     doc["timestamp"] = millis();
    
//     // Serialize to string
//     String jsonString;
//     serializeJson(doc, jsonString);
//     jsonString += "\n";  // Add newline for easy parsing on Master
    
//     // Send via TCP
//     if (tcpClient.connected()) {
//         tcpClient.print(jsonString);
//         Serial.print("✓ Sent to Master: ");
//         Serial.println(jsonString);
//     } else {
//         Serial.println("✗ TCP not connected, cannot send");
//         isConnectedToMaster = false;
//     }
// }


















