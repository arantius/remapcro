// All the functions related to being a USB host, i.e. reading input
// from the attached USB keyboard and turning it into events that the
// rest of the system can digest (calling USB device functions).

byte buf[8] = {0};
KeyboardReportParser keyboardReportParser;
KeyReport *reportIn;
KeyReport *reportInPrev;

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void initUsbHost() {
  Serial.println(F("Init USB host ..."));

  if (Usb.Init() == -1) {
    Serial.println(F("USB host init fail!"));
  }

  KeyboardPlus.begin();

  reportIn = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportIn, 0, 8);
  reportInPrev = (KeyReport*) malloc(sizeof(struct KeyReport));
  memset(reportInPrev, 0, 8);
}


bool reportContains(KeyReport* report, byte key) {
  for (uint8_t i = 0; i < 6; i++) {
    if (report->keys[i] == key) return true;
    if (report->keys[i] == 0) return false;
  }
  return false;
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

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

  memcpy(reportIn, buf, len);

  if (reportInPrev->modifiers != reportIn->modifiers) {
    handleModifiers(reportIn->modifiers);
  }

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t key = reportIn->keys[i];
    if (key == 0) break;
    if (key == 1) {
      sendErrReport();
      return;  // Error reports "1".
    }
    if (!reportContains(reportInPrev, key)) {
      handleUsbKey(1, key);
    }
  }

  for (uint8_t i = 0; i < 6; i++) {
    uint8_t key = reportInPrev->keys[i];
    if (key == 0) break;
    if (!reportContains(reportIn, key)) {
      handleUsbKey(0, key);
    }
  }

  memcpy(reportInPrev, reportIn, 8);
}


HIDSelector hidSelector(&Usb);

