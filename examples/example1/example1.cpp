#include <Arduino.h>
#include <HADiscovery.h>

Olon::HADevice haDevice;
Olon::HADiscovery haDiscovery;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;
  Serial.println();

  haDevice
    .addKey("name", "My anemometer device")
    .addKey("model", "ESP32")
    .addKey("identifiers", "ESP32-anemometer") // unique name, can be the chip id
    .addKey("sw_version", "1.0.0");

  haDiscovery
    .setDiscoveryPrefix("homeassistant")
    .setComponent("sensor")
    .setNodeId("home-roof-anemometer")
    .setPublisher([](const String& topic, const String& payload) {
      // Here, you would call your mqttClient.publish() code
      Serial.println(topic);
      Serial.println(payload);
    });

  haDiscovery
    .clear()
    .addDevice(haDevice)
    .setObjectId("wind_speed")
    .addKey("~", "olonsoft/home/roof/anemometer-east")
    .addKey("name", "Home Wind Speed") // Required. The human-friendly name displayed in the Home Assistant UI.
    .addKey("unique_id", "myhome_wind_speed_1") // Required. A unique identifier for the entity. This is required for Home Assistant to let you modify the entity later.
    // .addKey("object_id", "home_wind_speed1") // Optional. This can help guide the creation of the entity_id.
    .addKey("state_topic", "~/data") // The MQTT topic where the sensor's state is published.
    .addKey("value_template", "{{ value_json.speed }}") // This can be used to parse or format the value from the MQTT message
    .addKey("unit_of_measurement","kn") // The unit in which the sensor reports its data
    .addKey("availability_topic", "~/LWT") // An MQTT topic to report the availability of the device (online/offline)

    // Use device_class if your entity fits into a standard category like temperature, humidity, power, etc.
    // It is recommended to set this if your entity fits a standard device class (e.g., temperature, power, motion, etc.). It helps Home Assistant present and interpret the data correctly.
    .addKey("device_class", "??????")

    // You only need to set this if the sensor is involved in long-term statistics, like energy or power tracking. It is crucial for sensors providing historical or cumulative data (e.g., energy monitoring).
    // options are :
    // measurement: Use this when the sensor represents instantaneous measurements (e.g., current temperature, humidity, power).
    // total_increasing: Use this when the sensor's value represents a cumulative total that only increases (e.g., total energy consumption).
    // total: Represents a total value that can either increase or reset (e.g., water usage)
    .addKey("state_class", "measurement")

    .addKey("icon", "mdi:weather-windy")
    .addKey("number", 1.24)
    // .addKey("device_class", "enum")
    // .addKey("options", "[\"Empty\",\"Low\", \"Full\"]")
    .publish();
}

void loop() {

}