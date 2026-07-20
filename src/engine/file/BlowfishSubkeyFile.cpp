

#include <cstdio>

extern "C" unsigned int S[4][256]; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
unsigned int S[4][256];

extern "C" unsigned int P[18]; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
unsigned int P[18];

extern "C" std::FILE *SubkeyFile; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
std::FILE *SubkeyFile = nullptr;

extern "C" unsigned int F(unsigned int value); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" int opensubkeyfile() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    SubkeyFile = std::fopen("Blowfish.dat", "rb");
    return SubkeyFile != nullptr ? 0 : -1;
}

extern "C" void *decrypt(void *ctx) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI) // lint: void_ptr extern "C" ABI signature (opaque ctx in/out)
    // lint: void_ptr extern "C" ABI signature (opaque ctx in/out)
    return ctx;
}

extern "C" void Blowfish_encipher(unsigned int *xl, unsigned int *xr) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    unsigned int Xl = *xl;
    unsigned int Xr = *xr;
    for (int i = 0; i != 16; ++i) {
        unsigned int t = P[i] ^ Xl;
        Xl = F(t) ^ Xr;
        Xr = t;
    }
    *xl = P[17] ^ Xr;
    *xr = Xl ^ P[16];
}

extern "C" void Blowfish_decipher(unsigned int *xl, unsigned int *xr) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    unsigned int Xl = *xl;
    unsigned int Xr = *xr;
    for (unsigned int i = 17; i > 1; --i) {
        unsigned int t = P[i] ^ Xl;
        Xl = F(t) ^ Xr;
        Xr = t;
    }
    *xl = P[0] ^ Xr;
    *xr = Xl ^ P[1];
}

extern "C" void InitializeBlowfish(unsigned char *key, int keylen) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    if (opensubkeyfile() != 0) {
        std::printf("InitializeBlowfish: cannot open subkey file\n");
        return;
    }

    for (unsigned int i = 0; i < 18; ++i) {
        if (std::fread(&P[i], 4, 1, SubkeyFile) != 1)
            return;
    }
    for (unsigned int box = 0; box < 4; ++box) {
        for (unsigned int j = 0; j < 256; ++j) {
            if (std::fread(&S[box][j], 4, 1, SubkeyFile) != 1)
                return;
        }
    }
    std::fclose(SubkeyFile);

    short k = 0;
    for (int i = 0; i != 18; ++i) {
        unsigned int data = 0;
        for (unsigned short b = 0; b < 4; ++b) {
            int idx = k;
            ++k;
            if (keylen <= k)
                k = 0;
            data = (data << 8) | key[idx];
        }
        P[i] ^= data;
    }

    unsigned int dl = 0;
    unsigned int dr = 0;
    for (int i = 0; i < 18; i += 2) {
        Blowfish_encipher(&dl, &dr);
        P[i] = dl;
        P[i + 1] = dr;
    }
    for (int box = 0; box != 4; ++box) {
        for (int j = 0; j < 256; j += 2) {
            Blowfish_encipher(&dl, &dr);
            S[box][j] = dl;
            S[box][j + 1] = dr;
        }
    }
}
