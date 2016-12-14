#define MACRO_LED_PIN 2
#define MACRO_LED_BLINK_PERIOD 75


uint8_t macroLedBlinking = 0;

void handleMatrixKey(uint8_t pressed, uint8_t key);
void handleModifiers(uint8_t modifiers);
void handleUsbKey(uint8_t pressed, uint8_t key);
void initUsbDev();

void sendErrReport();
void dumpReport();

