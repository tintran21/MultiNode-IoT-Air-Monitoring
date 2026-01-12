#include "config.h"
#include "sensors.h"

sensor_data_t sensorData = {0};

void readAllSensors() {
    sensorData.temperature = dht.readTemperature();
    sensorData.humidity = dht.readHumidity();
    sensorData.ldr = analogRead(LDR_PIN);
    sensorData.mq2 = analogRead(MQ2_PIN);
    sensorData.mq135 = analogRead(MQ135_PIN);
}
