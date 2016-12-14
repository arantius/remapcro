#define MACRO_LED_PIN 2
#define MACRO_LED_BLINK_PERIOD 75

uint8_t macroLedBlinking = 0;

void handleMatrixKey(uint8_t pressed, uint8_t key);
void handleModifiers(uint8_t modifiers);
void handleUsbKey(uint8_t pressed, uint8_t key);
void initUsbDev();

void sendErrReport();
void dumpReport();


enum class KeyEventType : byte { invalid, keyDown, keyUp, modifier };
struct KeyEvent  {
  KeyEventType type;
  // When type == modifier, data is the current modifier byte.
  // Otherwise it is the raw USB HID byte of the affected key.
  uint8_t data;
};

