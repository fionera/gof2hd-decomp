// Blowfish subkey-file glue recovered from the Android binary
// (ghidra_addr 0x7b2a0, 0x7b8ca).
//
// The game ships its Blowfish key schedule in an external "Blowfish.dat" asset.
// opensubkeyfile() opens that asset for reading and stashes the FILE* in a
// module-global so InitializeBlowfish() (recovered elsewhere) can read the
// schedule out of it. decrypt() is an empty stub in the Android build (the
// decryption proper happens in the Blowfish round functions); it simply hands
// back its first argument.
#include <cstdio>

// The opened subkey file. opensubkeyfile() fills this in; the Blowfish
// initialiser drains it. Module-global with C linkage to match the binary.
extern "C" std::FILE *gSubkeyFile;
std::FILE *gSubkeyFile = nullptr;

// Open "Blowfish.dat" for binary reading and remember the handle.
// Returns 0 on success, -1 if the asset could not be opened.
extern "C" int opensubkeyfile()
{
    gSubkeyFile = std::fopen("Blowfish.dat", "rb");
    return gSubkeyFile != nullptr ? 0 : -1;
}

// No-op decrypt hook: returns its first argument unchanged.
extern "C" void *decrypt(void *ctx)
{
    return ctx;
}
