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

HIDSelector hidSelector(&Usb);

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

bool report_contains(KeyReport* report, byte key) {
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keys[i] == key) return true;
    if (report->keys[i] == 0) return false;
  }
  return false;
}

void debugReport() {
  static uint8_t n = 0;

  if (report_prev->modifiers != report->modifiers) {
    Serial.print(F(" Mod"));
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

  if (n > 9) {
    n = 0;
    Serial.println("");
  }
}

