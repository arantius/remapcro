#define MACRO_LED_PIN 2
#define MACRO_LED_BLINK_PERIOD 75


enum class KeyEventType : byte { invalid, keyDown, keyUp, modifier };


uint8_t macroLedBlinking = 0;

uint8_t handleMacroKey(KeyEventType t, uint8_t d);
void handleMatrixKey(uint8_t pressed, uint8_t key);
void handleModifiers(uint8_t modifiers);
void handleUsbKey(uint8_t pressed, uint8_t key);
void initUsbDev();

void sendErrReport();
void dumpReport();

