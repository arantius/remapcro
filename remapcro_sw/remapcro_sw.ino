#include <hidboot.h>
#include <hidcomposite.h>

#include "KbPlus.h"
#include "Matrix.h"
#include "UsbDev.h"
#include "UsbHost.h"


USB Usb;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
  UsbDEBUGlvl = 0xFF;

  Serial.begin(74880);

  if (Usb.Init() == -1) {
    Serial.println("USB Init() fail!");
    digitalWrite(13, HIGH);
  }

  while (!Serial) { }
  Serial.println("Remapcro start.");

  initUsbHost();
  initUsbDev();

  delay( 200 );
}


void loop() {
  static uint32_t tmPollMatrix = 0;
  uint32_t now = millis();
  if (now > tmPollMatrix) {
    keyMatrixRead();
    tmPollMatrix = now + MATRIX_POLL_PERIOD;
  }

  Usb.Task();
}

