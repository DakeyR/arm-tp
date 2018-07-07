#include "board.h"
#include "flash.h"


void flash_unlock(void)
{
    GET_REG(FLASH_KEYR) = FLASH_KEY1;
    GET_REG(FLASH_KEYR) = FLASH_KEY2;
}

void flash_lock(void)
{
    REG_SET_BIT(FLASH_CR, 31); // LOCK
}

void flash_erase(unsigned char sector)
{
    // assert(0 <= sector);
    // assert(sector < FLASH_SECTORS);

    while (REG_GET_BIT(FLASH_SR, 16)) // BSY
        continue;

    // See SNB: Ref Man 3.9.8 (page 106)
    if (sector >= 12)
        sector |= (1 << 4);

    REG_PUT_BITS(FLASH_CR, 3, 7, sector); // SNB
    REG_SET_BIT(FLASH_CR, 1); // SER: Sector Erase

    REG_SET_BIT(FLASH_CR, 16); // STRT

    while (REG_GET_BIT(FLASH_SR, 16))
        continue;
}

void flash_write(char *dst, const char *src, size_t n)
{
    // assert(&_flash_start <= dst);
    // assert(dst + n < &_flash_end);

    while (REG_GET_BIT(FLASH_SR, 16)) // BSY
        continue;

    while (n--)
    {
        REG_SET_BIT(FLASH_CR, 0); // PG

        *(dst++) = *(src++);

        while (REG_GET_BIT(FLASH_SR, 16)) // BSY
            continue;
    }
}
