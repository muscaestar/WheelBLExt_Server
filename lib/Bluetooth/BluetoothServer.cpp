#include "BluetoothServer.h"

BluetoothServer::BluetoothServer(const char* name, const char* serviceUUID, const char* characteristicUUID)
        :  _state(STATE_IDLE), name(), serviceUUID(serviceUUID), characteristicUUID(characteristicUUID)
{ }

void BluetoothServer::begin() {
    pServer = BLEDevice::createServer();
    pService = pServer->createService(serviceUUID);
    pCharacteristic = pService->createCharacteristic(
        characteristicUUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    pCharacteristic->setValue("");
    pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(serviceUUID);
    pService->start();
    pAdvertising->start();
    _state = STATE_BEGAN;
}

void BluetoothServer::close() {
    pAdvertising->stop();
    pServer->removeService(pService);
    delete pServer;
    _state = STATE_CLOSED;
}

void BluetoothServer::setCallbacks(BLECharacteristicCallbacks* callbacks) {
    pCharacteristic->setCallbacks(callbacks);
}

uint8_t BluetoothServer::getState() {
    return _state;
}
