#ifndef GOF2_PLATFORM_LIBC_H
#define GOF2_PLATFORM_LIBC_H
#include <cstddef>
#include <cstdint>

extern "C" {
void *malloc(size_t size);

void free(void *ptr);

void *memcpy(void *dst, const void *src, size_t n);

void *memset(void *dst, int c, size_t n);

float __aeabi_l2f(int64_t value);
}

#endif
