#pragma once

#include <stddef.h>


void flash_unlock(void);
void flash_lock(void);

void flash_erase(unsigned char sector);
void flash_write(char *dst, const char *src, size_t n);
