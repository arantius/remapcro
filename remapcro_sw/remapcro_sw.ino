#include <hidboot.h>
#include <hidcomposite.h>

#include "KeyboardPlus.h"
#include "Matrix.h"


#define DEV
#define MATRIX_POLL_PERIOD 16


byte buf[8] = {0};
KeyboardReportParser keyboardReportParser;
KeyReport *report;
KeyReport *report_prev;
USB Usb;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
#ifdef DEV
  UsbDEBUGlvl = 0xFF;
#else
  UsbDEBUGlvl = 0x00;
#endif

  Serial.begin(74880);

  if (Usb.Init() == -1) {
#ifdef DEV
    Serial.println("USB Init() fail!");
#endif
    digitalWrite(13, HIGH);
  }

  report = (KeyReport*) malloc(sizeof(struct KeyReport));
  memcpy(report, buf, 8);
  report_prev = (KeyReport*) malloc(sizeof(struct KeyReport));
  memcpy(report_prev, buf, 8);

  KeyboardPlus.begin();
  while (!Serial) { }

#ifdef DEV
  Serial.println("Remapcro start.");
#endif

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

