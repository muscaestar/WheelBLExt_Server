#include <Arduino.h>
#include <esp_sleep.h>
#include <BluetoothServer.h>
#include <LatchSwitch.h>
#include <ToggleButton.h>

#define CLEAR_PIN 5
#define BLE_SLP_PIN 4
#define TEST_BTN_PIN 21
#define EN_PIN 3 
#define S0_PIN 0
#define S1_PIN 1
#define S2_PIN 2
#define SS0_PIN 9
#define SS1_PIN 10
#define SS2_PIN 20

#define BTN_NULL 0x88
#define BTN_RB   0x20
#define BTN_MENU 0x30
#define BTN_LB   0x00
#define BTN_VIEW 0x10

#define BTN_B    0x07
#define BTN_Y    0x06
#define BTN_X    0x05
#define BTN_A    0x04
#define BTN_LEFT 0x03
#define BTN_UP   0x02
#define BTN_RIGT 0x01
#define BTN_DOWN 0x00

#define BLE_NAME "WheelBLExt_Server"
#define SERVICE_UUID        "854c4424-7dab-4ddc-8c35-5bebb61ef54f"
#define CHARACTERISTIC_UUID "cca12476-18c6-4001-b0f5-6fe1841d862e"

#define PRINT_DEBUG false

const uint8_t buttonValues[] = {
    BTN_NULL, BTN_RB, BTN_MENU, BTN_LB, BTN_VIEW,
    BTN_B, BTN_Y, BTN_X, BTN_A,
    BTN_LEFT, BTN_UP, BTN_RIGT, BTN_DOWN
};
uint8_t currTestChannel = BTN_NULL;

ToggleButton clearBtn(CLEAR_PIN);
LatchSwitch bleSwitch(BLE_SLP_PIN);
BluetoothServer bluetoothServer(BLE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID);

void processTestBtn();
void selectChannel(uint8_t channel);
void enableMultiplexer();
void disableMultiplexer();
void blueLed(bool on);

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
  pinMode(SS0_PIN, OUTPUT);
  pinMode(SS1_PIN, OUTPUT);
  pinMode(SS2_PIN, OUTPUT);
  disableMultiplexer();

  selectChannel(BTN_NULL);
  if (PRINT_DEBUG) {
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO) {
      Serial.println("Wake up from gpio");
    } else {
      Serial.println("Normal startup");
    }
  }
  
  BLEDevice::init(BLE_NAME);

  esp_deep_sleep_enable_gpio_wakeup(BIT64(BLE_SLP_PIN), ESP_GPIO_WAKEUP_GPIO_LOW);
  
}

void loop() {
    // 收集新一轮信号
    bleSwitch.update();

    processTestBtn();

    if (bleSwitch.isPressed() && bluetoothServer.getState() != STATE_BEGAN) {
      bluetoothServer.begin();
      bluetoothServer.setCallbacks(new MyCallbacks());
      blueLed(true);
    }
    if (bleSwitch.isReleased() && bluetoothServer.getState() == STATE_BEGAN) {
      disableMultiplexer();
      bluetoothServer.close();
      delay(1000);
      esp_deep_sleep_start();
    }
    if (clearBtn.isPressed()) {
      selectChannel(BTN_NULL);
    }
}

void enableMultiplexer() {
  digitalWrite(EN_PIN, LOW);
}

void disableMultiplexer() {
  digitalWrite(EN_PIN, HIGH);
}

void blueLed(bool on) {
    digitalWrite(BUILTIN_LED, on ? HIGH : LOW); // gpio 8
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
