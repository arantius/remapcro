// All the functions related to being a USB device, i.e. handling all
// the events coming from elsewhere to transmit data to the computer.
// Sometimes, this also means special functions (recording or
// transmitting a macro/remap).  The bulk of the project!

#ifdef DEBUG
#define USB_DEV_DBG
#endif

#define MACRO_READ_BLOCK 64


KeyReport *reportOut;
KeyReport *reportErr;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void initUsbDev() {
  Serial.println(F("Init USB device ..."));

  KeyboardPlus.begin();

  reportOut = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportOut, 0, 8);

  reportErr = (KeyReport*) malloc(sizeof(struct KeyReport));
  reportErr->modifiers = 0;
  reportErr->reserved = 0;
  memset(reportErr->keys, 1, 6);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

uint8_t isMacroRecording = 0;
uint16_t macroSize = 0;
uint32_t macroTargetAddr = 0;
uint8_t macroTargetKey = 0;
uint8_t macroTargetSector = 0;


uint8_t handleKeyDuringMacro(KeyEventType t, uint8_t d) {
  if (macroTargetKey == 0) {
    if (t == KeyEventType::keyUp) {
#ifdef USB_DEV_DBG
      Serial.print(F("Target macro key: "));
      Serial.println(d, HEX);
#endif
      ledBlinking = 0;
      lightLed(LedColor::green);
      macroSize = 0;
      macroTargetKey = d;
      macroTargetSector = selectUnusedFlashSector();
#ifdef USB_DEV_DBG
      Serial.print(F("Selected macro sector: "));
      Serial.println(macroTargetSector, HEX);
#endif
      macroTargetAddr = (macroTargetSector << 12) + 2;  // +2 for size
    }
    return 1;
  } else {
    if (t == KeyEventType::modifier) {
      writeOneMacroByte((uint8_t*) "\0");
    }
#ifdef USB_DEV_DBG
    Serial.print(d, HEX); Serial.print(F(" "));
#endif
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
#ifdef USB_DEV_DBG
    Serial.println(F("Starting macro record..."));
#endif
    isMacroRecording = 1;
    ledBlinking = 1;
    lightLed(LedColor::green);
    macroTargetKey = 0;
  } else {
#ifdef USB_DEV_DBG
    Serial.println(F("Finishing macro record..."));
#endif
    isMacroRecording = 0;
    ledBlinking = 0;
    lightLed(LedColor::off);

    if (macroSize > 0) {
      uint8_t sz[2];
      sz[0] = (macroSize >> 8) & 0xFF;
      sz[1] = macroSize & 0xFF;
#ifdef USB_DEV_DBG
      Serial.print(F("Write macro size: "));
      Serial.print(sz[0], HEX); Serial.print(" ");
      Serial.print(sz[1], HEX); Serial.println(" ");
#endif
      flashWrite( (macroTargetSector << 12), 2, sz);

#ifdef USB_DEV_DBG
      Serial.print(F("Write macro sector "));
      Serial.print(macroTargetSector, HEX);
      Serial.print(F(" for key "));
      Serial.println(macroTargetKey, HEX);
#endif
      EEPROM.write(EEPROM_MACRO_SECTORS_BASE + macroTargetKey, macroTargetSector);
    } else if (macroTargetKey) {
      EEPROM.write(EEPROM_MACRO_SECTORS_BASE + macroTargetKey, 0x00);
    }

    lightLed(LedColor::off);
  }
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void handleMatrixKey(uint8_t pressed, uint8_t key) {
#ifdef USB_DEV_DBG
  Serial.print(F("handleMatrixKey("));
  Serial.print(pressed);
  Serial.print(F(", "));
  Serial.print(key, HEX);
  Serial.println(F(");"));
#endif

  if (pressed) {
    // Only react to release of matrix keys.
    return;
  }

  if (key == MATRIX_MACRO) {
    toggleMacroRecording();
  } else {
    if (isMacroRecording && macroTargetKey == 0) {
#ifdef USB_DEV_DBG
      Serial.println(F("macro recording, with no target."));
#endif
      handleKeyDuringMacro(KeyEventType::keyUp, MATRIX_MACRO_OFFSET + key);
    } else if (!isMacroRecording) {
#ifdef USB_DEV_DBG
      Serial.println(F("no macro recording..."));
#endif
      uint8_t macroSector = EEPROM.read(
          EEPROM_MACRO_SECTORS_BASE + MATRIX_MACRO_OFFSET + key);
      if (macroSector != 0x00) {
        replayMacro(macroSector);
      }
    }
  }
}


void handleModifiers(uint8_t modifiers) {
  if (isMacroRecording) {
    handleKeyDuringMacro(KeyEventType::modifier, modifiers);
  }
  reportOut->modifiers = modifiers;
  sendReport();
}


void handleUsbKey(uint8_t pressed, uint8_t key) {
  if (key == 1) {
    sendErrReport();
    return;
  }

  if (isMacroRecording) {
    uint8_t res = handleKeyDuringMacro(
        pressed ? KeyEventType::keyDown : KeyEventType::keyUp, key);
    if (res) return;
  }

  uint8_t macroSector = 0;
  // Only look up macro contents if we are not currently recording a macro!
  if (!isMacroRecording) macroSector = EEPROM.read(EEPROM_MACRO_SECTORS_BASE + key);

  if (macroSector) {
    if (!pressed) replayMacro(macroSector);
    return;
  } else if (pressed) {
    for (uint8_t i = 0; i < 6; i++) {
      if (reportOut->keys[i] == 0x00) {
        reportOut->keys[i] = key;
        sendReport();
        return;
      }
    }
  } else if (!pressed) {
    for (uint8_t i = 0; i < 6; i++) {
      if (reportOut->keys[i] == key) {
        reportOut->keys[i] = 0x00;
        sendReport();
        return;
      }
    }
  }

  // Would have returned above in case of success.
  sendErrReport();
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void replayMacro(uint8_t sector) {
  lightLed(LedColor::blue);

#ifdef USB_DEV_DBG
  Serial.print(F("Sending macro, sector "));
  Serial.print(sector);
  Serial.print(F(", size "));
#endif

  uint32_t addr = (sector << 12);
  uint16_t size;
  uint8_t size8[2];
  flashRead(addr, 2, size8);
  size = (size8[0]<<8) + size8[1];
  addr += 2;

  if (size == 0xFFFF) {
    Serial.println(F("Error, flash sector empty."));
    lightLed(LedColor::off);
    return;
  } else if (size > 0x1000 - 2) {
    Serial.print(F("Error, impossibly large size: 0x"));
    Serial.println(size, HEX);
    return;
  } else {
#ifdef USB_DEV_DBG
    Serial.println(size);
#endif
  }

  memset(reportOut, 0, 8);

  uint8_t i = MACRO_READ_BLOCK;
  uint8_t modifierMode = 0;
  uint8_t buf[MACRO_READ_BLOCK] = {0};
  uint16_t tmNext = millis();
  while (size) {
    if (i == MACRO_READ_BLOCK) {
      i = 0;
      flashRead(addr, MACRO_READ_BLOCK, buf);
      addr += MACRO_READ_BLOCK;
    }

    uint8_t key = buf[i];
#ifdef USB_DEV_DBG
    Serial.print(key, HEX); Serial.print(" ");
#endif
    if (!modifierMode && key == 0x00) {
      modifierMode = 1;
    } else {
      if (modifierMode) {
        modifierMode = 0;
        reportOut->modifiers = key;
      } else {
        for (uint8_t i = 0; i < 6; i++) {
          if (reportOut->keys[i] == key) {
            reportOut->keys[i] = 0x00;
            break;
          } else if (reportOut->keys[i] == 0x00) {
            reportOut->keys[i] = key;
            break;
          }
        }
      }

      sendReport();
      while (millis() < tmNext) ;;
      tmNext = millis() + 16;
    }

    size--;
    i++;
  };

  memset(reportOut, 0, 8);
  sendReport();

#ifdef USB_DEV_DBG
  Serial.println("");
#endif

  lightLed(LedColor::off);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void dumpReport() {
  char tmp[8];
  sprintf(tmp, "%02x 00 ", reportOut->modifiers);
  Serial.print(tmp);
  for (uint8_t i = 0; i < 6; i++) {
    sprintf(tmp, "%02x ", reportOut->keys[i]);
    Serial.print(tmp);
  }
  Serial.println();
}


void sendErrReport() {
  Serial.println(F("Sending error report"));
  KeyboardPlus.sendReport(reportErr);
}


void sendReport() {
#ifdef USB_DEV_DBG
  Serial.print(F("Sending report to CPU: "));
  dumpReport();
#endif

  KeyboardPlus.sendReport(reportOut);
}
