#include "class.h"

typedef uint64_t uint64x2_unaligned __attribute__((vector_size(16), aligned(8)));

extern "C" void ApplicationManager_OnTouchEndSimple(ApplicationManager *self)
{
    uint64x2_unaligned zero = (uint64x2_unaligned){0, 0};
    *(uint32_t *)((char *)self + 0x08) = 0;
    *(uint32_t *)((char *)self + 0x0c) = 0;
    *(uint32_t *)((char *)self + 0x80) = 0;
    *(uint32_t *)((char *)self + 0x84) = 0;
    *(uint64x2_unaligned *)((char *)self + 0x98) = zero;
}
