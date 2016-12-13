#include <EEPROM.h>
#include <hidboot.h>
#include <hidcomposite.h>

#include "Flash.h"
#include "KbPlus.h"
#include "Matrix.h"
#include "UsbDev.h"
#include "UsbHost.h"


USB Usb;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void setup() {
  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
  UsbDEBUGlvl = 0xFF;

  Serial.begin(74880);
  while (!Serial) { }

  Serial.println(F("Remapcro starting..."));

  if (Usb.Init() == -1) {
    Serial.println(F("USB Init() fail!"));
  }

  initFlash();
  initUsbDev();
  initUsbHost();

  delay(200);

  Serial.println(F("...started!"));
}


void loop() {
  static uint32_t tmPollMatrix = 0;
  static uint32_t tmBlinkMacroLed = 0;

  uint32_t now = millis();
  if (now > tmPollMatrix) {
    keyMatrixRead();
    tmPollMatrix = now + MATRIX_POLL_PERIOD;
  }

  if (macroLedBlinking) {
    if (now > tmBlinkMacroLed) {
      digitalWrite(MACRO_LED_PIN, !digitalRead(MACRO_LED_PIN));
      tmBlinkMacroLed = now + MACRO_LED_BLINK_PERIOD;
    }
  }

  Usb.Task();

  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
    case 'e': {
      Serial.println(F("ERASING ALL permanent storage!"));
      flashEraseChip();
      uint8_t i = 0;
      do {
        EEPROM.write(EEPROM_MACRO_SECTORS_BASE + i++, 0x00);
      } while (i > 0);
      Serial.println(F("Done erasing."));
      break;
    } case 'm': {
      char a[3];
      a[0] = Serial.read();
      a[1] = Serial.read();
      a[2] = 0x00;
      uint8_t k = strtol(a, NULL, 16);
      Serial.print(F("Dumping macro on key "));
      Serial.print(k, HEX);
      uint8_t sector = EEPROM.read(EEPROM_MACRO_SECTORS_BASE + k);
      uint32_t addr = (sector << 12);
      uint16_t size;
      uint8_t size8[2];
      flashRead(addr, 2, size8);
      size = (size8[0]<<8) + size8[1];
      addr += 2;

      if (size == 0xFFFF) {
        Serial.println(F("Error, flash sector empty."));
        break;
      } else if (size > 0x1000 - 2) {
        Serial.print(F("Error, impossibly large size: 0x"));
        Serial.println(size, HEX);
        break;
      }

      uint8_t i = 16, buf[16] = {0};
      while (size) {
        if (i == 16) {
          i = 0;
          Serial.println("");
          flashRead(addr, 16, buf);
          addr += 16;
        }
        Serial.print(buf[i++], HEX);
        Serial.print(" ");
        size--;
      } while (i > 0);
      Serial.println("");
      break;
    } case 's': {
      Serial.print("Dumping macro sector data:");
      uint8_t i = 0;
      do {
        if (0 == i % 16) Serial.println("");
        Serial.print(EEPROM.read(EEPROM_MACRO_SECTORS_BASE + i++), HEX);
        Serial.print(" ");
      } while (i > 0);
      Serial.println("");
      break;
    } }
  }
}

