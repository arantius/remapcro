#define USB_CE_PIN 10

#define PRESSED 1
#define RELEASED 0

void initUsbHost();


class HIDSelector : public HIDComposite {
  public:
    HIDSelector(USB *p) : HIDComposite(p) {};
  protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);
};


class KbdRptParser : public KeyboardReportParser {
  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
};

