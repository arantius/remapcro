// All the functions related to the keyboard matrix part of the device,
// including the RGB LED built into the (top left) macro button.

uint16_t keysPrev = 0xFFFF;


void initMatrix() {
  pinMode(PIN_LED_R, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  pinMode(PIN_LED_G, OUTPUT);
  digitalWrite(PIN_LED_G, LOW);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_B, LOW);
}

/*
This function works with the "key" board as described by sibling hardware
files.  The actual keyboard is two columns of six keys, the matrix is
four "columns" of three keys.  When viewed normally the keys are laid
out as in the left diagram:

+----+----+    +----+----+
| C0 | C2 |    |    |    |
| R0 | R0 |    |  0 |  6 |
+----+----+    +----+----+
+----+----+    +----+----+
| C0 | C2 |    |    |    |
| R1 | R1 |    |  1 |  7 |
+----+----+    +----+----+
| C0 | C2 |    |    |    |
| R2 | R2 |    |  2 |  8 |
+----+----+    +----+----+
| C1 | C3 |    |    |    |
| R0 | R0 |    |  3 |  9 |
+----+----+    +----+----+
| C1 | C3 |    |    |    |
| R1 | R2 |    |  4 | 10 |
+----+----+    +----+----+
| C1 | C3 |    |    |    |
| R2 | R2 |    |  5 | 11 |
+----+----+    +----+----+

Verbally: the columns are stacked in a top left down, top right down
order, and the rows cross the columns in their number order.

This function reads those rows and columns, with an individual diode
protecting each key, and fills a 16 bit unsigned int with their data.
The top four bits always remain zero, as we have only twelve keys.

Each column is hooked to a GPOUT line.  The matching GPIN lines
on the rows have pullups, so exactly one column is driven low,
then the three GPIN lines are read to discover the status of the
three keys in that "row" can be read (pressed keys read low,
unpressed keys' lines are pulled high).

The bits of the output map:

    111111
    5432109876543210
Col ----333222111000
Row ----210210210210

Or, each physical key maps to the bit number in the right diagram,
above.  Low to high bits store low to high columns, low to high
rows within each column.
*/
void keyMatrixRead() {
  uint16_t keysNow = 0xFFFF;

  uint8_t mask = 1 << 3;
  while (1) {
    Usb.gpioWr(~mask);
    keysNow <<= 3;
    keysNow |= (Usb.gpioRd() & 0b00000111);

    if (mask == 1) break;
    mask >>= 1;
  }
  if (keysNow == keysPrev) return;

  // Handle newly pressed keys.
  uint16_t tmpNow = keysNow;
  uint16_t tmpPrev = keysPrev;
  for (uint8_t i = 0; i < 16; i++) {
    if ( (tmpNow & 1) != (tmpPrev & 1) ) {
      handleMatrixKey(!(tmpNow & 1), i);
    }
    tmpNow >>= 1;
    tmpPrev >>= 1;
  }

  keysPrev = keysNow;
}


void lightLed(LedColor c) {
  if (c != LedColor::off) {
    ledColor = c;
  }

  switch (c) {
  case LedColor::off:
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);
    break;
  case LedColor::red:
    digitalWrite(PIN_LED_R, HIGH);
    digitalWrite(PIN_LED_G, LOW);
    digitalWrite(PIN_LED_B, LOW);
    break;
  case LedColor::green:
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, LOW);
    break;
  case LedColor::blue:
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, LOW);
    break;
  case LedColor::yellow:
    digitalWrite(PIN_LED_R, LOW);
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, HIGH);
    break;
  }
}

