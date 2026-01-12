#include "config.h"
#include "data_sender.h"

void sendDataToMaster() {
    StaticJsonDocument<256> doc;
    doc["temp"] = sensorData.temperature;
    doc["humi"] = sensorData.humidity;
    doc["ldr"] = sensorData.ldr;
    doc["mq2"] = sensorData.mq2;
    doc["mq135"] = sensorData.mq135;
    doc["timestamp"] = millis();

    String json;
    serializeJson(doc, json);
    json += "\n";

    if (tcpClient.connected()) {
        tcpClient.print(json);
    }
}
