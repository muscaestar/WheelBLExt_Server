# pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define STATE_IDLE 0
#define STATE_BEGAN 1
#define STATE_CLOSED 2

class BluetoothServer {
public:
    BluetoothServer(const char* name, const char* serviceUUID, const char* characteristicUUID);

    void begin();
    void close();
    void setCallbacks(BLECharacteristicCallbacks* callbacks);
    uint8_t getState();
    void writeValue(uint8_t* value, size_t length);
    
private:
    const char* name;
    const char* serviceUUID;
    const char* characteristicUUID;
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristic;
    BLEAdvertising *pAdvertising;
    uint8_t _state;
};