#include "class.h"

extern "C" void String_default_ctor(void *s);   // 0x6efbc
__attribute__((visibility("hidden"))) extern int g_mgameVtable; // @0x187b3c ([0]=vtable base)
__attribute__((visibility("hidden"))) extern int g_mgameInitVal; // @0x187c00 (DAT_00187c00)

// MGame::MGame() — install vtable, default-construct the title String, zero state.
extern "C" MGame *MGame_ctor(MGame *self) {
    F<int>(self, 0x0) = g_mgameVtable + 8;
    String_default_ctor((char *)self + 0x64);

    int z = 0;
    int initVal = g_mgameInitVal;

    // NEON-cleared 16-byte groups at 0xa4, 0x13c, 0x18c.
    static const unsigned grpA[] = {0xa4u, 0x13cu, 0x18cu};
    for (unsigned i = 0; i < 3; i++) {
        unsigned off = grpA[i];
        F<int>(self, off + 0)  = 0;
        F<int>(self, off + 4)  = 0;
        F<int>(self, off + 8)  = 0;
        F<int>(self, off + 12) = 0;
    }

    F<int>(self, 0x48) = z; F<int>(self, 0x4c) = z;
    F<int>(self, 0xe4) = z; F<int>(self, 0xe8) = z;
    F<int>(self, 0xec) = z;
    F<int>(self, 0x1bc) = z; F<int>(self, 0x1c0) = z;
    F<int>(self, 0x1c4) = z;
    F<int>(self, 0xb4) = z; F<int>(self, 0xb8) = z;
    F<int>(self, 0x14c) = z; F<int>(self, 0x150) = z;
    F<int>(self, 0x19c) = z; F<int>(self, 0x1a0) = z;

    F<int>(self, 0xc) = 0x64;       // field_0C = 100
    F<int>(self, 0x10) = -1;        // field_10 = -1
    F<int>(self, 0x14) = z;

    // NEON-cleared 16-byte groups at 0x30, 0x20, 0x80, 0x70.
    static const unsigned grpB[] = {0x30u, 0x20u, 0x80u, 0x70u};
    for (unsigned i = 0; i < 4; i++) {
        unsigned off = grpB[i];
        F<int>(self, off + 0)  = 0;
        F<int>(self, off + 4)  = 0;
        F<int>(self, off + 8)  = 0;
        F<int>(self, off + 12) = 0;
    }

    F<uint8_t>(self, 0x1a6) = 0;
    F<uint8_t>(self, 0x54) = 0;
    F<uint16_t>(self, 0xd5) = 0;
    F<int>(self, 0x114) = z;
    F<int>(self, 0xf4) = z;
    F<uint8_t>(self, 0x1e4) = 0;
    F<int>(self, 0x1d4) = z;
    F<int>(self, 0x40) = z;
    F<int>(self, 0x58) = z; F<int>(self, 0x5c) = z;
    F<uint16_t>(self, 0x60) = 0;
    F<int>(self, 0x90) = z; F<int>(self, 0x94) = z;
    F<uint8_t>(self, 0xc8) = 0;
    F<int>(self, 0xc4) = z;
    F<uint16_t>(self, 0xce) = 0;
    F<int>(self, 0xca) = z;
    F<int>(self, 0xd8) = z; F<int>(self, 0xdc) = z;
    F<int>(self, 0x1d8) = initVal;
    F<int>(self, 0x1e0) = z;
    return self;
}
