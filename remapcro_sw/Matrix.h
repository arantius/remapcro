#define MATRIX_POLL_PERIOD 16
#define MATRIX_MACRO 0

// When storing a macro on a key from the "real" keyboard, we use the HID
// ID of its key to store the macro sector (in EEPROM).  When storing the
// macro sector for a matrix key, we want to use an ID that won't conflict
// with those "real" keys.  There's just one byte, and most of the
// possible values are taken.  The space for F13 through F24 starts at
// 0x68 and we have twelve (well, eleven ...) possible matrix keys.
// Seems like a perfect space to reuse.
#define MATRIX_MACRO_OFFSET 0x68


#define PIN_LED_R 6
#define PIN_LED_G 5
#define PIN_LED_B 3

enum class LedColor : byte { off, red, green, blue, yellow };

uint8_t ledBlinking = 0;
LedColor ledColor = LedColor::off;


void initMatrix();
void keyMatrixRead();
void lightLed(LedColor c);

