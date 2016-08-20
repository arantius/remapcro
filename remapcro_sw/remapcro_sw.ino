#include <hidboot.h>
#include <hidcomposite.h>

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

// https://forum.arduino.cc/index.php?topic=38107.msg282342#msg282342
void PrintHex8(uint8_t *data, uint8_t length) {
   char tmp[length*3+1];
   memset(tmp, 0x20, length * 3);
   byte first;
   byte second;
   for (int i=0; i<length; i++) {
     first = (data[i] >> 4) & 0x0f;
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
  Serial.print("ep: "); Serial.print(ep, HEX);
  Serial.print("\tis_rpt_id: "); Serial.print(is_rpt_id, HEX);
  Serial.print("\tlen: "); Serial.println(len, HEX);

  // Default report parser handles LED twiddling.
  keyboardReportParser.Parse(hid, is_rpt_id, len, buf);

  if (len)  {
    PrintHex8(buf, len);
  }
}

HIDSelector hidSelector(&Usb);

// \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\ // \\

void setup() {
  Serial.begin(74880);
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }
  
  // Set this to higher values to enable more debug information
  // minimum 0x00, maximum 0xff, default 0x80
  UsbDEBUGlvl = 0xff;

  delay( 200 );
}


void loop() {
  Usb.Task();
}

