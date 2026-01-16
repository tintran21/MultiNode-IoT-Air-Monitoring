#include "config.h"
#include "network_manager.h"

const char* WIFI_SSID = "ESP32_MASTER_AP"; // you can change the SSID and PASS
const char* WIFI_PASS = "12345678";        // but you have to change familiar to node master

const char* SERVER_IP = "192.168.4.1";
const uint16_t SERVER_PORT = 8888;

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient tcpClient;

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void connectToMaster() {
    tcpClient.connect(SERVER_IP, SERVER_PORT);
}
