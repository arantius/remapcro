/*
http://www.issi.com/WW/pdf/25LQ080.pdf

The flash chip is 8Mbit (1Mbyte), organized into sectors of 4 Kbyte and
blocks of 64 Kbyte.  Either a sector or block can be erased, while any page
of 256 byte can be programmed.

That makes for 256 sectors (of 4Kbyte), meaning plenty room to store one
macro per key ID with a reasonable max length.  The sector will store two
bytes of length followed by up to 4094 bytes of macro stream data.

Which sector is storing which key's macro will be stored in 256 bytes
of EEPROM, which can be scanned to discover unused sectors when storing
a new macro.  Remapping is implemented as a single press (and release)
macro.  A null byte in this map means no macro is stored on this key.
(And, by consequence, we'll never store one in flash sector zero.)

Macro streams are simply the "Usage ID" bytes of a keyboard key, as per
the USB HID spec.  The null byte is a special marker that the next byte
represents a change in the modifiers value instead.  Thus a press and
release of a key takes two bytes to store, while any change (press or
release) of a modifier key takes two, four for press and release.

The process to store a new macro will:
1) Select a free flash sector.
2) Erase it.  (Typical 120 max 300 milliseconds.)
3) Program each byte (or two) as each key is pressed (or released).
4) Upon macro termination, mark the flash sector used in EEPROM.

Random sectors will be selected, for primitive wear leveling.  A random
byte will be generated, then all used key/sector mappings will be scanned.
Whenver the current value is discovered to be used it will be incremented,
and the scan for a used sector restarts.  This is n^2 worst case, but since
keyboards have ~100 keys out of 256 slots, in practice a free sector will
be discovered quite rapidly.
*/


#define FLASH_CE_PIN 8

// Commands.
#define FLASH_COMMAND_RDID 0xAB
#define FLASH_COMMAND_JDEC_ID_READ 0x9F
#define FLASH_COMMAND_RDMDID 0x90
#define FLASH_COMMAND_WREN 0x06
#define FLASH_COMMAND_WRDI 0x04
#define FLASH_COMMAND_RDSR 0x05
#define FLASH_COMMAND_WRSR 0x01
#define FLASH_COMMAND_READ 0x03
#define FLASH_COMMAND_FAST_READ 0x0B
#define FLASH_COMMAND_FRDO 0x3B
#define FLASH_COMMAND_FRDIO 0xBB
#define FLASH_COMMAND_FRQO 0x6B
#define FLASH_COMMAND_FRQIO 0xEB
#define FLASH_COMMAND_MR 0xFF
#define FLASH_COMMAND_PAGE_PROG 0x02
#define FLASH_COMMAND_SECTOR_ER 0xD7
#define FLASH_COMMAND_BLOCK_ER 0xD8
#define FLASH_COMMAND_CHIP_ER 0xC7


// Status register bits.
#define FLASH_STATUS_WIP 0x01
#define FLASH_STATUS_WEL 0x02
#define FLASH_STATUS_BP0 0x04
#define FLASH_STATUS_BP1 0x08
#define FLASH_STATUS_BP2 0x10
#define FLASH_STATUS_BP3 0x20
#define FLASH_STATUS_QE  0x40
#define FLASH_STATUS_SRWD 0x80


void initFlash();
void flashEraseSector(uint8_t sector);
void flashRead(uint32_t addr, uint8_t size, uint8_t *dst);
void flashWrite(uint32_t addr, uint8_t size, uint8_t *src);

