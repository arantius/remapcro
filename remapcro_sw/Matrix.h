#define MATRIX_POLL_PERIOD 16


// Map from matrix number to (default) report value.  The obvious values
// except nulls for the "*" and "#" keys which have special meaning.
const uint8_t matrixMap[16] PROGMEM = {
    0x04, 0x05, 0x06, 0x07,
    0x5B, 0x5E, 0x61, 0x00,
    0x5A, 0x5D, 0x60, 0x62,
    0x59, 0x5C, 0x5F, 0x00};


void keyMatrixRead();

