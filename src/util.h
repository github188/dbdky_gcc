#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

namespace dbdky
{
static uint16_t makeword(uint8_t low, uint8_t high)
{
    uint16_t ret = 0;
    ret |= high;
    ret <<= 8;
    ret |= low;

    return ret;
}

static uint32_t makelong(uint16_t low, uint16_t high)
{
    uint32_t ret = 0;
    ret |= high;
    ret <<= 16;
    ret |= low;
    
    return ret;
}
}
#endif    //__UTIL_H_
