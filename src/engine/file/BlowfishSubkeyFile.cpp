

#include <cstdio>

// Blowfish substitution boxes: 4 boxes of 256 32-bit entries (4096 bytes, .bss).
// Emitted as the bare C-linkage symbol "S" to match the original binary.
extern "C" unsigned int S[4][256];
unsigned int S[4][256];

// Blowfish P-array: 18 32-bit subkeys (72 bytes, .bss).
// Emitted as the bare C-linkage symbol "P" to match the original binary.
extern "C" unsigned int P[18];
unsigned int P[18];

static std::FILE *gSubkeyFile = nullptr;

extern "C" int opensubkeyfile() {
    gSubkeyFile = std::fopen("Blowfish.dat", "rb");
    return gSubkeyFile != nullptr ? 0 : -1;
}

extern "C" void *decrypt(void *ctx) {
    return ctx;
}
