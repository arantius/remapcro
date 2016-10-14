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

  if (Usb.Init() == -1) {
    Serial.println(F("USB Init() fail!"));
  }

  Serial.println(F("Remapcro starting..."));

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
}

