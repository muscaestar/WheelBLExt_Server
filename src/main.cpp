#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define TEST_BTN_PIN 5
#define EN_PIN 4
#define S0_PIN 0
#define S1_PIN 1
#define S2_PIN 2
#define S3_PIN 3
#define SS0_PIN 10
#define SS1_PIN 20
#define SS2_PIN 21

#define BTN_NULL 0x00
#define BTN_RB   0x20
#define BTN_MENU 0x30
#define BTN_LB   0x40
#define BTN_VIEW 0x50

#define BTN_B    0x03
#define BTN_Y    0x04
#define BTN_X    0x05
#define BTN_A    0x06
#define BTN_LEFT 0x0C
#define BTN_UP   0x0D
#define BTN_RIGT 0x0E
#define BTN_DOWN 0x0F

#define SERVICE_UUID        "854c4424-7dab-4ddc-8c35-5bebb61ef54f"
#define CHARACTERISTIC_UUID "cca12476-18c6-4001-b0f5-6fe1841d862e"

#define PRINT_DEBUG false

const uint8_t buttonValues[] = {
    BTN_NULL, BTN_RB, BTN_MENU, BTN_LB, BTN_VIEW,
    BTN_B, BTN_Y, BTN_X, BTN_A,
    BTN_LEFT, BTN_UP, BTN_RIGT, BTN_DOWN
};
uint8_t currTestChannel = BTN_NULL;


void processTestBtn();
void selectChannel(uint8_t channel);
void enableMultiplexer();
void disableMultiplexer();

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      uint8_t* data = pCharacteristic->getData();
      // if data is empty
      if (data == NULL) { return; }
      uint8_t firstByte = data[0];
      selectChannel(firstByte);
    }
};

void setup() {
  if (PRINT_DEBUG) {
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Starting Arduino BLE Server application...");
  }
  // Set pin modes
  pinMode(TEST_BTN_PIN, INPUT_PULLDOWN); // Set TEST_BTN_PIN as input with internal pull-down resistor
  pinMode(EN_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(SS0_PIN, OUTPUT);
  pinMode(SS1_PIN, OUTPUT);
  pinMode(SS2_PIN, OUTPUT);
  disableMultiplexer();
  
  BLEDevice::init("WheelBLExt_Server");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("");
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  selectChannel(BTN_NULL);
}

void loop() {
  processTestBtn();
}

void enableMultiplexer() {
  digitalWrite(EN_PIN, LOW);
}

void disableMultiplexer() {
  digitalWrite(EN_PIN, HIGH);
}

void selectChannel(uint8_t channel) {
  if (PRINT_DEBUG) {
    uint8_t c1Channel = channel & 0xF0;
    uint8_t c2Channel = channel & 0x0F;
    Serial.print("Selecting channel: ");
    Serial.print(channel, HEX);
    Serial.print(" (c1: ");
    Serial.print(c1Channel, HEX);
    Serial.print(" (c2: ");
    Serial.print(c2Channel, HEX);
    Serial.println("");
  }
  if (channel == BTN_NULL) {
    disableMultiplexer();
  } else {
    enableMultiplexer();
  }
  // for the channel value, first 4 bits are c1 channel number, last 4 bits are c2 channel number
  // Set the channel for the first multiplexer (c1)
  digitalWrite(SS0_PIN, bitRead(channel, 4));
  digitalWrite(SS1_PIN, bitRead(channel, 5));
  digitalWrite(SS2_PIN, bitRead(channel, 6));

  // Set the channel for the second multiplexer (c2)
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));
  digitalWrite(S3_PIN, bitRead(channel, 3));
}

void processTestBtn() {
  if (digitalRead(TEST_BTN_PIN) == HIGH) { // Check if the button is pressed
    for (uint8_t i = 0; i < sizeof(buttonValues) / sizeof(buttonValues[0]); i++) {
      if (currTestChannel == buttonValues[i]) {
        currTestChannel = buttonValues[(i + 1) % (sizeof(buttonValues) / sizeof(buttonValues[0]))];
        break;
      }
    }
    selectChannel(currTestChannel);
    delay(500);
  }
}
