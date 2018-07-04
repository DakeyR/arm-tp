#include "board.h"
#include "flash.h"


#define KEY1 0x45670123
#define KEY2 0xCDEF89AB


void flash_unlock(void)
{
    GET_REG(FLASH_KEYR) = KEY1;
    GET_REG(FLASH_KEYR) = KEY2;
}

void flash_lock(void)
{
    REG_SET_BIT(FLASH_CR, 31); // LOCK
}

void flash_erase(unsigned char sector)
{
    // 0 <= sector < 24

    while (REG_GET_BIT(FLASH_SR, 16)) // BSY
        continue;

    // See SNB: Ref Man 3.9.8 (page 106)
    if (sector >= 12)
        sector += 4;

    REG_PUT_BITS(FLASH_CR, 3, 7, sector); // SNB
    REG_SET_BIT(FLASH_CR, 1); // SER: Sector Erase

    REG_SET_BIT(FLASH_CR, 16); // STRT

    while (REG_GET_BIT(FLASH_SR, 16))
        continue;
}
