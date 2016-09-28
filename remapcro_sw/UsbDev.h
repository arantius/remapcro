void handleMatrixKey(uint8_t pressed, uint8_t key);
void handleModifiers(uint8_t modifiers);
void handleUsbKey(uint8_t pressed, uint8_t key);
void initUsbDev();

void sendErrReport();


enum class KeyEventType : byte { invalid, keyDown, keyUp, modifier };
struct KeyEvent  {
  KeyEventType type;
  // When type == modifier, data is the current modifier byte.
  // Otherwise it is the raw USB HID byte of the affected key.
  uint8_t data;
};

// A fixed buffer of up to MACRO_BUF_SIZE KeyEvents where new macros are actively recorded.
#define MACRO_BUF_SIZE 254
KeyEvent macroBuf[MACRO_BUF_SIZE] = {};
uint8_t macroBufLen = 0;

