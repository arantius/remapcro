// All the functions related to interacting with the Flash storage chip.

void initFlash() {
  Serial.println(F("Init Flash ..."));
  pinMode(FLASH_CE_PIN, OUTPUT);
  digitalWrite(FLASH_CE_PIN, HIGH);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

//#define FLASH_COMMAND_DBG

void flashEraseChip() {
#ifdef FLASH_COMMAND_DBG
  Serial.print(F("flashEraseChip()");
#endif

  flashWriteEnable();

  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_CHIP_ER);
  digitalWrite(FLASH_CE_PIN, HIGH);

  // TODO: Only block at the next operation, if necessary?
  flashWaitForWriteCompletion();
}


void flashEraseSector(uint8_t sector) {
#ifdef FLASH_COMMAND_DBG
  Serial.print(F("flashEraseSector("));
  Serial.print(sector, HEX);
  Serial.println(F(");"));
#endif

  flashWriteEnable();

  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_BLOCK_ER);
  flashSendSectorAddr(sector);
  digitalWrite(FLASH_CE_PIN, HIGH);

  // TODO: Only block at the next operation, if necessary?
  flashWaitForWriteCompletion();
}


void flashRead(uint32_t addr, uint8_t size, uint8_t *dst) {
#ifdef FLASH_COMMAND_DBG
  Serial.print(F("flashRead("));
  Serial.print(addr, HEX);
  Serial.print(F(", "));
  Serial.print(size, HEX);
  Serial.println(F(");"));
#endif

  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_READ);
  flashSendAddr(addr);
  while (size) {
    *dst = SPI.transfer(0x00);
    dst++;
    size--;
  }
  digitalWrite(FLASH_CE_PIN, HIGH);
}


void flashWrite(uint32_t addr, uint8_t size, uint8_t *src) {
#ifdef FLASH_COMMAND_DBG
  Serial.print(F("flashWrite("));
  Serial.print(addr, HEX);
  Serial.print(F(", "));
  Serial.print(size, HEX);
  Serial.println(F(");"));
#endif

  flashWriteEnable();

  while (size) {
    digitalWrite(FLASH_CE_PIN, LOW);
    SPI.transfer(FLASH_COMMAND_PAGE_PROG);
    flashSendAddr(addr);
    while (size) {
      SPI.transfer(*src);
      addr++;
      src++;
      size--;

      if ((addr & 0x000000FF) == 0) {
        break;  // Continue to the next page with a new program operation.
      }
    }
    digitalWrite(FLASH_CE_PIN, HIGH);
  }

  // TODO: Only block at the next operation, if necessary?
  flashWaitForWriteCompletion();
}


uint8_t selectUnusedFlashSector() {
  // Load into a bitmask the full set of used sectors.
  uint8_t usedSectors[32] = {0};
  for (uint8_t i = 1; i > 0; i++) {
    if (EEPROM.read(EEPROM_MACRO_SECTORS_BASE + i)) {
      bitMaskSet(usedSectors, i);
    }
  }
  // Pick a random sector.
  randomSeed(millis());
  uint8_t sector = (uint8_t) random(255);
  // Increment until we find an un-set bit (un-used sector).
  while (bitMaskTest(usedSectors, sector)) sector++;

  flashEraseSector(sector);

  return sector;
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void flashSendAddr(uint32_t addr) {
  SPI.transfer((addr & 0x00FF0000) >> 16);
  SPI.transfer((addr & 0x0000FF00) >> 8);
  SPI.transfer((addr & 0x000000FF));
}


void flashSendSectorAddr(uint8_t sector) {
  // To turn sector number (of 0-255) into byte address (of 1024*1024),
  // Shift left twelve bits.  Out of a 24 bit address, that means the
  // bytes go: 0x0H 0xL0 0x00.
  SPI.transfer(sector >> 4);
  SPI.transfer((sector & 0x0F) <<4);
  SPI.transfer(0x00);
}


void flashWaitForWriteCompletion() {
  uint8_t status;
  do {
    digitalWrite(FLASH_CE_PIN, LOW);
    SPI.transfer(FLASH_COMMAND_RDSR);
    status = SPI.transfer(0x00);
    digitalWrite(FLASH_CE_PIN, HIGH);
  } while (status & FLASH_STATUS_WIP);
}


void flashWriteEnable() {
  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_WREN);
  digitalWrite(FLASH_CE_PIN, HIGH);
}

