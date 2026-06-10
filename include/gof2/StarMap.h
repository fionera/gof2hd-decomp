#ifndef GOF2_STARMAP_H
#define GOF2_STARMAP_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Mission;



template <class T>
static inline T &field(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

static inline void *&ptr_field(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}









void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void *operator new[](__SIZE_TYPE__ size);
void operator delete[](void *ptr) noexcept;

struct StarMap {
    Vector field_0x78;                  // +0x78
};
#endif
