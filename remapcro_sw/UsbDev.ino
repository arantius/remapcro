// All the functions related to being a USB device, i.e. handling all
// the events coming from elsewhere to transmit data to the computer.
// Sometimes, this also means special functions (recording or
// transmitting a macro/remap).  The bulk of the project!

KeyReport *reportOut;
KeyReport *reportErr;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void initUsbDev() {
  Serial.println(F("Init USB device ..."));

  pinMode(MACRO_LED_PIN, OUTPUT);
  digitalWrite(MACRO_LED_PIN, HIGH);

  reportOut = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportOut, 0, 8);

  memset(reportErr, 0, 2);
  memset(reportErr->keys, 1, 6);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

uint8_t isMacroRecording = 0;
uint16_t macroSize = 0;
uint32_t macroTargetAddr = 0;
uint8_t macroTargetKey = 0;
uint8_t macroTargetSector = 0;


uint8_t handleMacroKey(KeyEventType t, uint8_t d) {
  if (macroTargetKey == 0) {
    if (t == KeyEventType::keyUp) {
      macroLedBlinking = 0;
      digitalWrite(MACRO_LED_PIN, LOW);
      macroSize = 0;
      macroTargetKey = d;
      macroTargetSector = selectUnusedFlashSector();
      Serial.print(F("Selected macro sector: "));
      Serial.println(macroTargetSector, HEX);
      macroTargetAddr = (macroTargetSector << 12) + 2;  // +2 for size
    }
    return 1;
  } else {
    if (t == KeyEventType::modifier) {
      writeOneMacroByte((uint8_t*) "\0");
    }
    writeOneMacroByte(&d);

    return 0;
  }
}

void writeOneMacroByte(uint8_t *val) {
  flashWrite(macroTargetAddr, 1, val);
  macroTargetAddr++;
  macroSize++;
}


void toggleMacroRecording() {
  if (!isMacroRecording) {
    Serial.println(F("Starting macro record..."));
    isMacroRecording = 1;
    macroLedBlinking = 1;
  } else {
    Serial.println(F("Finishing macro record..."));
    isMacroRecording = 0;

    if (macroSize > 0) {
      uint8_t sz[2];
      sz[0] = (macroSize >> 8) & 0xFF;
      sz[1] = macroSize & 0xFF;
      Serial.print(F("Write macro size: "));
      Serial.print(sz[0], HEX); Serial.print(" ");
      Serial.print(sz[1], HEX); Serial.println(" ");
      flashWrite( (macroTargetSector << 12), 2, sz);
      EEPROM.write(EEPROM_MACRO_SECTORS_BASE + macroTargetKey, macroTargetSector);
    }

    digitalWrite(MACRO_LED_PIN, HIGH);
  }
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void handleMatrixKey(uint8_t pressed, uint8_t key) {
  if (key == MATRIX_STAR) {
    if (!pressed) {
      toggleMacroRecording();
    }
  } else if (key == MATRIX_HASH) {
    if (!pressed) {
      Serial.println(F("TODO: Remap mode activate here."));
    }
  } else {
    /*
    Serial.print(F("handleMatrixKey("));
    Serial.print(pressed);
    Serial.print(F(", "));
    Serial.print(key, HEX);
    Serial.println(F(");"));
    */

    handleUsbKey(pressed, pgm_read_byte_near(matrixMap + key));
  }
}


void handleModifiers(uint8_t modifiers) {
  if (isMacroRecording) {
    handleMacroKey(KeyEventType::modifier, modifiers);
  } else {
    reportOut->modifiers = modifiers;
    sendReport();
  }
}


void handleUsbKey(uint8_t pressed, uint8_t key) {
  if (key == 1) {
    sendErrReport();
    return;
  }

  if (isMacroRecording) {
    uint8_t res = handleMacroKey(
        pressed ? KeyEventType::keyDown : KeyEventType::keyUp, key);
    if (res) return;
  }

  // TODO: Check for macro mapping on `key` here.

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

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void sendErrReport() {
  Serial.println(F("Sending error report"));
  KeyboardPlus.sendReport(reportErr);
}


void sendReport() {
  /*
  Serial.print(F("Sending report to CPU: "));
  char tmp[8];
  sprintf(tmp, "%02x 00 ", reportOut->modifiers);
  Serial.print(tmp);
  for (uint8_t i = 0; i < 6; i++) {
    sprintf(tmp, "%02x ", reportOut->keys[i]);
    Serial.print(tmp);
  }
  Serial.println();
  */

  KeyboardPlus.sendReport(reportOut);
}
