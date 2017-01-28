// All the functions related to being a USB host, i.e. reading input
// from the attached USB keyboard and turning it into events that the
// rest of the system can digest (calling USB device functions).

HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
HIDSelector hidSelector(&Usb);
KbdRptParser Prs;


void initUsbHost() {
  Serial.println(F("Init USB host ..."));

  if (Usb.Init() == -1) {
    Serial.println(F("USB host init fail!"));
  }

  HidKeyboard.SetReportParser(0, &Prs);
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto) {
  if (proto != 0) {
    return true;
  }
  return false;
}


void HIDSelector::ParseHIDData(
    USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf) {
}

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ //

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
  handleModifiers(after);
}


void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  handleUsbKey(1, key);
}


void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key) {
  handleUsbKey(0, key);
}

