KeyReport *reportOut;
KeyReport *reportErr;

// Map from matrix number to report value.
const uint8_t matrixMap[16] PROGMEM = {
    0x04, 0x05, 0x06, 0x07,
    0x5B, 0x5E, 0x61, 0xA0,
    0x5A, 0x5D, 0x60, 0x62,
    0x59, 0x5C, 0x5F, 0x55};


void handleMatrixKey(uint8_t pressed, uint8_t key) {
  //Serial.print(F("handleMatrixKey("));
  //Serial.print(pressed);
  //Serial.print(F(", "));
  //Serial.print(key, HEX);
  //Serial.println(F(");"));

  handleUsbKey(pressed, pgm_read_byte_near(matrixMap + key));
}


void handleModifiers(uint8_t modifiers) {
  reportOut->modifiers = modifiers;
  sendReport();
}


void handleUsbKey(uint8_t pressed, uint8_t key) {
  if (key == 1) {
    sendErrReport();
    return;
  }

  if (pressed) {
    for (uint8_t i = 0; i < 6; i++) {
      if (reportOut->keys[i] == key) {
        // Already pressed.
        return;
      } else if (reportOut->keys[i] == 0x00) {
        reportOut->keys[i] = key;
        sendReport();
        return;
      }
    }
    // Would have returned above in case of success.
    sendErrReport();
  } else {
    for (uint8_t i = 0; i < 6; i++) {
      if (reportOut->keys[i] == key) {
        if (i < 5) {
          memmove(&reportOut->keys[i], &reportOut->keys[i+1], 6 - i);
        }
        reportOut->keys[5] = 0x00;
        sendReport();
        return;
      } else if (reportOut->keys[i] == 0x00) {
        // Wasn't pressed, ignore.
        return;
      }
    }
  }
}


void initUsbDev() {
#ifdef DEV
  Serial.println("Init USB device ...");
#endif
  reportOut = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportOut, 0, 8);

  memset(reportErr, 0, 2);
  memset(reportErr->keys, 1, 6);
}


void sendErrReport() {
#ifdef DEV
  Serial.println("Sending error report");
#endif
  KeyboardPlus.sendReport(reportErr);
}


void sendReport() {
#ifdef DEV
  Serial.print("Sending report: ");
  char tmp[8];
  sprintf(tmp, "%02x 00 ", reportOut->modifiers);
  Serial.print(tmp);
  for (uint8_t i = 0; i < 6; i++) {
    sprintf(tmp, "%02x ", reportOut->keys[i]);
    Serial.print(tmp);
  }
  Serial.println();
#endif
  KeyboardPlus.sendReport(reportOut);
}

