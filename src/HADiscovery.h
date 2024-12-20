#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Olon {

class HADevice {
 private:
  JsonDocument doc;

 public:
  HADevice& addKey(const String& key, const String& value) {
    doc[key] = value;
    return *this;
  }

  JsonObject get() { return doc.as<JsonObject>(); }
};

// ----------------------

typedef std::function<void(const String& topic, const String& payload)>
    PublisherCallback;

class HADiscovery {
 private:
  String _discoveryPrefix = "homeassistant";
  String _component = "";
  String _nodeId = "";
  String _objectId = "";
  JsonDocument doc;
  PublisherCallback _publisher = nullptr;

 public:
  HADiscovery& setDiscoveryPrefix(const String& discoveryPrefix) {
    _discoveryPrefix = discoveryPrefix;
    return *this;
  };

  HADiscovery& setComponent(const String& component) {
    _component = component;
    return *this;
  };

  HADiscovery& setNodeId(const String& nodeId) {
    _nodeId = filterString(nodeId);
    return *this;
  };

  HADiscovery& setObjectId(const String& objectId) {
    _objectId = filterString(objectId);
    return *this;
  };

  void setPublisher(PublisherCallback publisher) { _publisher = publisher; };

  HADiscovery& addDevice(const JsonObject deviceJson) {
    doc["device"] = deviceJson;
    return *this;
  }

  HADiscovery& addDevice(HADevice& device) {
    doc["device"] = device.get();
    return *this;
  }

  HADiscovery& addKey(const String& key, const String& value) {
    doc[key] = value;
    return *this;
  }

  HADiscovery& addKey(const String& key, const float& value) {
    doc[key] = value;
    return *this;
  }

  HADiscovery& addKey(const String& key, const double& value) {
    doc[key] = value;
    return *this;
  }

  // Clear all elements
  HADiscovery& clear() {
    doc.clear();
    return *this;
  };

  void publish() {
    size_t _size = measureJson(doc);
    String payload;
    payload.reserve(_size);
    serializeJsonPretty(doc, payload);  // todo change to serializeJson
    String topic = addTrailingSlash(_discoveryPrefix) +
                   addTrailingSlash(_component) +
                   addTrailingSlash(_nodeId) +
                   addTrailingSlash(_objectId) + "config";
    _publisher(topic, payload);
  }

  String addTrailingSlash(String str) {
    // Check if the last character is not a slash
    if (str.length() > 0 && str.charAt(str.length() - 1) != '/') {
      // Add a slash to the end of the string
      str += '/';
    };
    return str;
  };

  String filterString(String input) {
    String result = "";
    for (unsigned int i = 0; i < input.length(); i++) {
      char c = input.charAt(i);
      if (isAlphaNumeric(c) || c == '_' || c == '-') {
        result += c;
      } else {
        if (c == ' ')
          result += '_';
        // else
        //   result += '-';
      }
    }
    return result;
  }

};

}  // namespace Olon