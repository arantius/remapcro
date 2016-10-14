// All the functions related to interacting with the Flash storage chip.

#include "Flash.h"


void initFlash() {
  Serial.println(F("Init Flash ..."));
  pinMode(FLASH_CE_PIN, OUTPUT);
  digitalWrite(FLASH_CE_PIN, HIGH);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

//#define SPI_COMMAND_DBG

void flashEraseSector(uint8_t sector) {
#ifdef SPI_COMMAND_DBG
  Serial.print(F("flashEraseSector("));
  Serial.print(sector, HEX);
  Serial.println(F(");"));
#endif

  spiWriteEnable();

  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_BLOCK_ER);
  spiSendSectorAddr(sector);
  digitalWrite(FLASH_CE_PIN, HIGH);

  // TODO: Only block at the next operation, if necessary?
  spiWaitForWriteCompletion();
}


void flashRead(uint32_t addr, uint8_t size, uint8_t *dst) {
#ifdef SPI_COMMAND_DBG
  Serial.print(F("spiFlashRead("));
  Serial.print(addr, HEX);
  Serial.print(F(", "));
  Serial.print(size, HEX);
  Serial.println(F(");"));
#endif

  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_READ);
  spiSendAddr(addr);
  while (size) {
    *dst = SPI.transfer(0x00);
    dst++;
    size--;
  }
  digitalWrite(FLASH_CE_PIN, HIGH);
}


void flashWrite(uint32_t addr, uint8_t size, uint8_t *src) {
#ifdef SPI_COMMAND_DBG
  Serial.print(F("spiFlashWrite("));
  Serial.print(addr, HEX);
  Serial.print(F(", "));
  Serial.print(size, HEX);
  Serial.println(F(");"));
#endif

  spiWriteEnable();

  while (size) {
    digitalWrite(FLASH_CE_PIN, LOW);
    SPI.transfer(FLASH_COMMAND_PAGE_PROG);
    spiSendAddr(addr);
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
  spiWaitForWriteCompletion();
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void spiSendAddr(uint32_t addr) {
  SPI.transfer((addr & 0x00FF0000) >> 16);
  SPI.transfer((addr & 0x0000FF00) >> 8);
  SPI.transfer((addr & 0x000000FF));
}


void spiSendSectorAddr(uint8_t sector) {
  // To turn sector number (of 0-255) into byte address (of 1024*1024),
  // Shift left twelve bits.  Out of a 24 bit address, that means the
  // bytes go: 0x0H 0xL0 0x00.
  SPI.transfer(sector >> 4);
  SPI.transfer((sector & 0x0F) <<4);
  SPI.transfer(0x00);
}


void spiWaitForWriteCompletion() {
  uint8_t status;
  do {
    digitalWrite(FLASH_CE_PIN, LOW);
    SPI.transfer(FLASH_COMMAND_RDSR);
    status = SPI.transfer(0x00);
    digitalWrite(FLASH_CE_PIN, HIGH);
  } while (status & FLASH_STATUS_WIP);
}


void spiWriteEnable() {
  digitalWrite(FLASH_CE_PIN, LOW);
  SPI.transfer(FLASH_COMMAND_WREN);
  digitalWrite(FLASH_CE_PIN, HIGH);
}
