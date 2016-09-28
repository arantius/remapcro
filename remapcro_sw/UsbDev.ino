// All the functions related to being a USB device, i.e. handling all
// the events coming from elsewhere to transmit data to the computer.
// Sometimes, this also means special functions (recording or
// transmitting a macro/remap).

KeyReport *reportOut;
KeyReport *reportErr;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

uint8_t isMacroRecording = 0;
uint8_t macroTargetKey = 0;


void debugPrintMacro() {
  Serial.print(F("TODO: Store macro now:\nOn: "));
  Serial.println(macroTargetKey, HEX);
  Serial.print(F("Events: "));
  for (uint8_t i = 0; i < macroBufLen; i++) {
    Serial.print(
        macroBuf[i].type == KeyEventType::modifier
            ? "M" : macroBuf[i].type == KeyEventType::keyUp ? "-" : "+");
    Serial.print(macroBuf[i].data, HEX);
    Serial.print(" ");
  }
  Serial.println("");
}

void handleMacroKey(KeyEventType t, uint8_t d) {
  if (macroTargetKey == 0) {
    if (t == KeyEventType::keyUp) {
      macroTargetKey = d;
    }
  } else {
    if (macroBufLen <= MACRO_BUF_SIZE) {
      macroBuf[macroBufLen].type = t;
      macroBuf[macroBufLen].data = d;
      macroBufLen++;
    }
  }
}


void storeMacro() {
  if (macroBufLen == 0) {
    Serial.println(F("Abort macro."));
  } else {
    debugPrintMacro();

    // TODO: Write to EEPROM/SPI here.

    macroBufLen = 0;
  }
}


void toggleMacroRecording() {
  if (!isMacroRecording) {
    Serial.println(F("Starting macro record..."));
    isMacroRecording = 1;
    // TODO: Light "macro is recording" LED.
  } else {
    isMacroRecording = 0;
    storeMacro();
  }
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

#define MATRIX_STAR 15
#define MATRIX_HASH 7

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
    handleMacroKey(pressed ? KeyEventType::keyDown : KeyEventType::keyUp, key);
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

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void initUsbDev() {
  Serial.println("Init USB device ...");
  reportOut = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportOut, 0, 8);

  memset(reportErr, 0, 2);
  memset(reportErr->keys, 1, 6);
}


void sendErrReport() {
  Serial.println("Sending error report");
  KeyboardPlus.sendReport(reportErr);
}


void sendReport() {
  /*
  Serial.print("Sending report to CPU: ");
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
