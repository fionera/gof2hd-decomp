#ifndef GOF2_PLATFORM_LIBC_H
#define GOF2_PLATFORM_LIBC_H
#include <cstddef>
#include <cstdint>

// libc / ARM EABI runtime entry points referenced by recovered engine code. These are external
// platform symbols, declared here once so individual translation units don't redeclare them.
extern "C" {
void *malloc(size_t size);
void  free(void *ptr);
void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);

// ARM EABI helpers the decompiler emitted as explicit calls.
float __aeabi_l2f(int64_t value);
} // extern "C"

#endif // GOF2_PLATFORM_LIBC_H
