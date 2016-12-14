// All the functions related to being a USB device, i.e. handling all
// the events coming from elsewhere to transmit data to the computer.
// Sometimes, this also means special functions (recording or
// transmitting a macro/remap).  The bulk of the project!

//#define FLASH_COMMAND_DBG

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
#ifdef FLASH_COMMAND_DBG
      Serial.print(F("Target macro key: "));
      Serial.println(d, HEX);
#endif
      macroLedBlinking = 0;
      digitalWrite(MACRO_LED_PIN, LOW);
      macroSize = 0;
      macroTargetKey = d;
      macroTargetSector = selectUnusedFlashSector();
#ifdef FLASH_COMMAND_DBG
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
#ifdef FLASH_COMMAND_DBG
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
#ifdef FLASH_COMMAND_DBG
    Serial.println(F("Starting macro record..."));
#endif
    isMacroRecording = 1;
    macroLedBlinking = 1;
    macroTargetKey = 0;
  } else {
#ifdef FLASH_COMMAND_DBG
    Serial.println(F("Finishing macro record..."));
#endif
    isMacroRecording = 0;
    macroLedBlinking = 0;

    if (macroSize > 0) {
      uint8_t sz[2];
      sz[0] = (macroSize >> 8) & 0xFF;
      sz[1] = macroSize & 0xFF;
#ifdef FLASH_COMMAND_DBG
      Serial.print(F("Write macro size: "));
      Serial.print(sz[0], HEX); Serial.print(" ");
      Serial.print(sz[1], HEX); Serial.println(" ");
#endif
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
#ifdef FLASH_COMMAND_DBG
    Serial.print(F("handleMatrixKey("));
    Serial.print(pressed);
    Serial.print(F(", "));
    Serial.print(key, HEX);
    Serial.println(F(");"));
#endif
    handleUsbKey(pressed, pgm_read_byte_near(matrixMap + key));
  }
}


void handleModifiers(uint8_t modifiers) {
  if (isMacroRecording) {
    handleMacroKey(KeyEventType::modifier, modifiers);
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
    uint8_t res = handleMacroKey(
        pressed ? KeyEventType::keyDown : KeyEventType::keyUp, key);
    if (res) return;
  }

  uint8_t macroSector = 0;
  // Only look up macro contents if we are not currently recording a macro!
  if (!isMacroRecording) macroSector = EEPROM.read(EEPROM_MACRO_SECTORS_BASE + key);

  if (macroSector) {
    if (!pressed) replayMacro(macroSector);
  } else {
    for (uint8_t i = 0; i < 6; i++) {
      if (reportOut->keys[i] == key) {
        if (!pressed) {
          if (i < 5) {
            memmove(&reportOut->keys[i], &reportOut->keys[i+1], 6 - i);
          }
          reportOut->keys[5] = 0x00;
          sendReport();
        }
        return;
      } else if (reportOut->keys[i] == 0x00) {
        if (pressed) {
          reportOut->keys[i] = key;
          sendReport();
        }
        return;
      }
    }
    // Would have returned above in case of success.
    sendErrReport();
  }
}

void replayMacro(uint8_t sector) {
  digitalWrite(MACRO_LED_PIN, LOW);

#ifdef FLASH_COMMAND_DBG
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
    return;
  } else if (size > 0x1000 - 2) {
    Serial.print(F("Error, impossibly large size: 0x"));
    Serial.println(size, HEX);
    return;
  } else {
#ifdef FLASH_COMMAND_DBG
    Serial.println(size);
#endif
  }

  memset(reportOut, 0, 8);

  uint8_t i = 16, modifierMode = 0, buf[16] = {0};
  uint16_t tmNext = millis();
  while (size) {
    if (i == 16) {
      i = 0;
      flashRead(addr, 16, buf);
      addr += 16;
    }

    uint8_t key = buf[i];
#ifdef FLASH_COMMAND_DBG
    Serial.print(key, HEX); Serial.print(" ");
#endif
    if (key == 0x00) {
      modifierMode = 1;
    } else {
      if (modifierMode) {
        modifierMode = 0;
        reportOut->modifiers = key;
      } else {
        for (uint8_t j = 0; j < 6; j++) {
          if (reportOut->keys[j] == key) {
            // Key is in report, remove it.
            if (j < 5) {
              memmove(&reportOut->keys[j], &reportOut->keys[j+1], 6 - j);
            }
            reportOut->keys[5] = 0x00;
            break;
          } else if (reportOut->keys[j] == 0x00) {
            // Key is not in report, add it.
            reportOut->keys[j] = key;
            break;
          }
        }
      }

      sendReport();
    }

    if (key != 0x00) {
      while (millis() < tmNext) ;;
      tmNext = millis() + 20;
    }

    size--;
    i++;
  };

  memset(reportOut, 0, 8);
  sendReport();

#ifdef FLASH_COMMAND_DBG
  Serial.println("");
#endif

  digitalWrite(MACRO_LED_PIN, HIGH);
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
#ifdef FLASH_COMMAND_DBG
  Serial.print(F("Sending report to CPU: "));
  dumpReport();
#endif

  KeyboardPlus.sendReport(reportOut);
}
