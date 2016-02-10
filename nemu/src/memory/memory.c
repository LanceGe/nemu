#include "common.h"
#include "memory/memory.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
#ifdef USE_CACHE
    return cached_read(addr, len);
#else
    return dram_read(addr, len);
#endif
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
#ifdef USE_CACHE
    cached_write(addr, len, data);
#else
    dram_write(addr, len, data);
#endif
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
    return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
    assert(len == 1 || len == 2 || len == 4);
#endif
    return lnaddr_read(addr, len);
}

void swaddr_read_bytes(void *dest, swaddr_t addr, size_t len) {
    while (len >= 4) {
        *(uint32_t *)dest = swaddr_read(addr, 4);
        dest += 4;
        addr += 4;
        len -= 4;
    }
    while (len > 1) {
        *(uint8_t *)dest = swaddr_read(addr, 1);
        dest += 1;
        addr += 1;
        len -= 1;
    }
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
    assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_write(addr, len, data);
}
