#include <EEPROM.h>
#include <hidboot.h>
#include <hidcomposite.h>

#include "Flash.h"
#include "KbPlus.h"
#include "Matrix.h"
#include "UsbDev.h"
#include "UsbHost.h"


//#define DEBUG

USB Usb;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void setup() {
  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
#ifdef DEBUG
  UsbDEBUGlvl = 0xFF;
# else
  UsbDEBUGlvl = 0x00;
#endif

  Serial.begin(74880);
  for (uint8_t i = 0; i < 255; i++) {
    if (Serial) break;
    delay(2);
  }

  Serial.println(F("Remapcro starting..."));

  // Before starting SPI, be sure to disable both devices' chip enable pin.
  pinMode(FLASH_CE_PIN, OUTPUT);
  digitalWrite(FLASH_CE_PIN, HIGH);
  pinMode(USB_CE_PIN, OUTPUT);
  digitalWrite(USB_CE_PIN, HIGH);

  SPI.begin();

  initFlash();
  initUsbDev();
  initUsbHost();

  delay(200);

  Serial.println(F("...started!"));
}


void loop() {
  static uint32_t tmPollMatrix = 0;
  static uint32_t tmBlinkMacroLed = 0;
  static uint8_t macroBlinkState = 0;

  uint32_t now = millis();
  if (now > tmPollMatrix) {
    keyMatrixRead();
    tmPollMatrix = now + MATRIX_POLL_PERIOD;
  }

  if (ledBlinking) {
    if (now > tmBlinkMacroLed) {
      macroBlinkState = !macroBlinkState;
      lightLed(macroBlinkState ? ledColor : LedColor::off);
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
      Serial.print(F(", sector "));
      Serial.print(sector, HEX);
      uint32_t addr = (sector << 12);
      uint16_t size;
      uint8_t size8[2];
      flashRead(addr, 2, size8);
      size = (size8[0]<<8) + size8[1];
      addr += 2;
      Serial.print(F(", size "));
      Serial.print(size8[0], HEX); Serial.print(F(" "));
      Serial.print(size8[1], HEX); Serial.print(F(" "));

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
        Serial.print(buf[i], HEX);
        Serial.print(" ");
        size--;
        i++;
      }
      Serial.println(F("done."));
      break;
    } case 'r': {
      Serial.print(F("Current report: "));
      dumpReport();
      break;
    } case 's': {
      Serial.println(F("Dumping macro sector data:"));
      Serial.print(F("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F"));
      uint8_t i = 0, j = 0;
      do {
        if (0 == i % 16) {
          Serial.println(F(""));
          Serial.print(j++, HEX);
          Serial.print(F("  "));
        }
        uint8_t s = EEPROM.read(EEPROM_MACRO_SECTORS_BASE + i++);
        if (s == 0) {
          Serial.print(F("  "));
        } else {
          if (s <= 0xF) Serial.print(F("0"));
          Serial.print(s, HEX);
        }
        Serial.print(F(" "));
      } while (i > 0);
      Serial.println(F(""));
      break;
    } }
  }
}

