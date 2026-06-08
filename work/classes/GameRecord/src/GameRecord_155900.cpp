#include "class.h"

// GameRecord::GameRecord()
GameRecord::GameRecord()
{
    char *t = (char *)this;

    // Two embedded AbyssEngine::String members.
    AEString_ctor(t + 0x188);
    AEString_ctor(t + 0x194);

    // Heap buffer at +0x00.
    void *buf = operator_new__(0x87);
    *(uint32_t *)(t + 0x11c) = 0;
    *(void **)t = buf;

    // Clear the leading block.
    __aeabi_memclr8(t + 0x8, 0x8c);

    // Seven 16-byte vector slots cleared to zero (a {0,0,0,0} init).
    *(uint32_t *)(t + 0xb8) = 0; *(uint32_t *)(t + 0xbc) = 0;
    *(uint32_t *)(t + 0xc0) = 0; *(uint32_t *)(t + 0xc4) = 0;
    *(uint32_t *)(t + 0xa8) = 0; *(uint32_t *)(t + 0xac) = 0;
    *(uint32_t *)(t + 0xb0) = 0; *(uint32_t *)(t + 0xb4) = 0;
    *(uint32_t *)(t + 0x98) = 0; *(uint32_t *)(t + 0x9c) = 0;
    *(uint32_t *)(t + 0xa0) = 0; *(uint32_t *)(t + 0xa4) = 0;
    *(uint32_t *)(t + 0x10b) = 0; *(uint32_t *)(t + 0x10f) = 0;
    *(uint32_t *)(t + 0x113) = 0; *(uint32_t *)(t + 0x117) = 0;
    *(uint32_t *)(t + 0x100) = 0; *(uint32_t *)(t + 0x104) = 0;
    *(uint32_t *)(t + 0x108) = 0; *(uint32_t *)(t + 0x10c) = 0;
    *(uint32_t *)(t + 0xf0) = 0; *(uint32_t *)(t + 0xf4) = 0;
    *(uint32_t *)(t + 0xf8) = 0; *(uint32_t *)(t + 0xfc) = 0;
    *(uint32_t *)(t + 0xe0) = 0; *(uint32_t *)(t + 0xe4) = 0;
    *(uint32_t *)(t + 0xe8) = 0; *(uint32_t *)(t + 0xec) = 0;

    *(uint32_t *)(t + 0x1a0) = 0xffffffff;
    *(uint32_t *)(t + 0xc8) = 0;
    *(uint32_t *)(t + 0xcc) = 0;
    *(uint32_t *)(t + 0xd0) = 0;
    *(uint8_t *)(t + 0xd4) = 0;
    *(uint32_t *)(t + 0xd8) = 0;
    *(uint8_t *)(t + 0xdc) = 0;
    *(uint32_t *)(t + 0x1b8) = 0;
    *(uint8_t *)(t + 0x1c0) = 0;

    __aeabi_memclr8(t + 0x130, 0x58);

    for (int iVar5 = 0x69; iVar5 != 0x6d; iVar5 = iVar5 + 1)
        *(uint32_t *)(t + iVar5 * 4) = 0;

    *(uint32_t *)(t + 0x1b4) = 0;
}
