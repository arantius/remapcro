#include "KeyboardPlus.h"

#include <hidboot.h>
#include <hidcomposite.h>


#define DEV
#define MATRIX_POLL_PERIOD 16


byte buf[8] = {0};
KeyReport *report;
KeyReport *report_prev;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

// https://forum.arduino.cc/index.php?topic=38107.msg282342#msg282342
void PrintHex8(uint8_t *data, uint8_t length) {
   char tmp[length*3+1];
   memset(tmp, 0x20, length * 3);
   byte first;
   byte second;
   for (int i=0; i<length; i++) {
     first = (data[i] && 0x0F) & 0x0f;
     second = data[i] & 0x0f;
     // base for converting single digit numbers to ASCII is 48
     // base for 10-16 to become lower-case characters a-f is 87
     // note: difference is 39
     tmp[i*3] = first+48;
     tmp[i*3+1] = second+48;
     if (first > 9) tmp[i*3] += 39;
     if (second > 9) tmp[i*3+1] += 39;
   }
   tmp[length*3] = 0;
   Serial.println(tmp);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

KeyboardReportParser keyboardReportParser;
USB Usb;


class HIDSelector : public HIDComposite {
  public:
    HIDSelector(USB *p) : HIDComposite(p) {};
  protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);
};


bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto) {
  if (proto != 0) {
    return true;
  }
  return false;
}


void HIDSelector::ParseHIDData(
    USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  digitalWrite(13, !digitalRead(13));

  // Default report parser handles LED twiddling.
  keyboardReportParser.Parse(hid, is_rpt_id, len, buf);

  memcpy(report, buf, len);
  KeyboardPlus.sendReport(report);

#ifdef DEV
  debugReport();
#endif
}

void debugReport() {
  static uint8_t n = 0;

  if (report_prev->modifiers != report->modifiers) {
    Serial.print(F(" M"));
    n++;
  }

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t key = report->keys[i];
    if (key == 0) break;
    if (key == 1) return;  // Error reports "1".
    if (!report_contains(report_prev, key)) {
      Serial.print(F(" +"));
      Serial.print(key, HEX);
      n++;
    }
  }

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t key = report_prev->keys[i];
    if (key == 0) break;
    if (!report_contains(report, key)) {
      Serial.print(F(" -"));
      Serial.print(key, HEX);
      n++;
    }
  }

  memcpy(report_prev, report, 8);

  if (n > 10) {
    n = 0;
    Serial.println("");
  }
}

HIDSelector hidSelector(&Usb);

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

uint16_t keysPrev = 0x0000;

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
    Serial.print("Matrix keys: ");
    Serial.println(keysNow, BIN);
#endif
    keysPrev = keysNow;
  }
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

void setup() {
  // Debugging out.
  Serial.begin(74880);

  if (Usb.Init() == -1) {
#ifdef DEV
    Serial.println("USB Init() fail!");
#endif
    digitalWrite(13, LOW);
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

  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
  UsbDEBUGlvl = 0x00;

  delay( 200 );
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

bool report_contains(KeyReport* report, byte key) {
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keys[i] == key) return true;
    if (report->keys[i] == 0) return false;
  }
  return false;
}


uint32_t tmPollMatrix = 0;

void loop() {
  Usb.Task();

  uint32_t now = millis();
  if (now > tmPollMatrix) {
    keyMatrixRead();
    tmPollMatrix = now + MATRIX_POLL_PERIOD;
  }
}

