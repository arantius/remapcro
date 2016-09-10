uint16_t keysPrev = 0xFFFF;


/*
This function works with a 4x4 keyboard matrix, four column pins (driven
through diodes) and four row pins.  It fills a 16 bit unsigned int, and due
to the arrangement and reading pattern, the keys end up in bit positions
(MSB=15, LSB=0):

+----+----+----+----+
| 12 |  8 |  4 |  0 |
+----+----+----+----+
| 13 |  9 |  5 |  1 |
+----+----+----+----+
| 14 | 10 |  6 |  2 |
+----+----+----+----+
| 15 | 11 |  7 |  3 |
+----+----+----+----+

IOW from bottom left counting up then right.
*/

void keyMatrixRead() {
  uint16_t keysNow = 0x0000;

  uint8_t col;
  uint8_t i = 0;
  uint8_t cols[4] = {};
  uint8_t mask = 0x08;
  while (1) {
    Usb.gpioWr(~mask);
    col = Usb.gpioRd() >> 4;
    cols[i++] = col;
    keysNow <<= 4;
    keysNow |= col;

    if (mask == 1) break;
    mask >>= 1;
  }

  // Check for ghosts.
  for (i = 0; i < 3; i++) {
    // Nothing pressed in this column?  Ignore.
    if (cols[i] == 0b1111) continue;

    for (uint8_t j = i + 1; j < 4; j++) {
      if (cols[i] == cols[j]) {
        // Two columns are the same.  Only one key in each is okay.
        if (   cols[i] != 0b1110
            && cols[i] != 0b1101
            && cols[i] != 0b1011
            && cols[i] != 0b0111
        ) {
          // Otherwise we've got a conflict between multiple keys sharing
          // a row and column, something's a ghost, but we can't tell which.
          // Ignore this case.

          // TODO: Set/track error status, clear when resolved.

          return;
        }
      }
    }
  }

  if (keysNow != keysPrev) {
#ifdef DEV
    debugMatrix(keysNow);
#endif
    keysPrev = keysNow;
  }
}


void debugMatrix(uint16_t keysNow) {
  static uint8_t n = 0;
  
  uint16_t tmpNow = keysNow;
  uint16_t tmpPrev = keysPrev;
  for (uint8_t i = 0; i < 16; i++) {
    if ( (tmpNow & 1) != (tmpPrev & 1)) {
      Serial.print(tmpNow & 1 ? " +M" : " -M");
      Serial.print(i, HEX);
      n++;
    }
    tmpNow >>= 1;
    tmpPrev >>= 1;
  }

  if (n > 9) {
    n = 0;
    Serial.println("");
  }
}

