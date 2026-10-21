

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
    unsigned int temp;
    for (int i = 0; i < 16; ++i) {
        Xl = Xl ^ P[i];
        Xr = F(Xl) ^ Xr;
        temp = Xl;
        Xl = Xr;
        Xr = temp;
    }
    temp = Xl;
    Xl = Xr;
    Xr = temp;
    Xr = Xr ^ P[16];
    Xl = Xl ^ P[17];
    *xl = Xl;
    *xr = Xr;
}

extern "C" void Blowfish_decipher(unsigned int *xl, unsigned int *xr) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    unsigned int Xl = *xl;
    unsigned int Xr = *xr;
    unsigned int temp;
    for (unsigned int i = 17; i >= 2; --i) {
        Xl = Xl ^ P[i];
        Xr = F(Xl) ^ Xr;
        temp = Xl;
        Xl = Xr;
        Xr = temp;
    }
    temp = Xl;
    Xl = Xr;
    Xr = temp;
    Xr = Xr ^ P[1];
    Xl = Xl ^ P[0];
    *xl = Xl;
    *xr = Xr;
}

// Reference Blowfish key schedule (P-array and S-boxes read from Blowfish.dat, then mixed with the key).
extern "C" short InitializeBlowfish(unsigned char key[], short keybytes) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    unsigned int data;
    unsigned int datal;
    unsigned int datar;

    int error = opensubkeyfile();
    if (error != 0) {
        std::printf("Unable to open subkey initialization file : %d\n", error);
        return (short) error;
    }

    for (unsigned short i = 0; i < 18; ++i) {
        unsigned short numread = (unsigned short) std::fread(&data, 4, 1, SubkeyFile);
        if (numread != 1)
            return -1;
        P[i] = data;
    }
    for (unsigned short i = 0; i < 4; ++i) {
        for (unsigned short j = 0; j < 256; ++j) {
            unsigned short numread = (unsigned short) std::fread(&data, 4, 1, SubkeyFile);
            if (numread != 1)
                return -1;
            S[i][j] = data;
        }
    }
    std::fclose(SubkeyFile);

    short j = 0;
    for (short i = 0; i < 18; ++i) {
        data = 0;
        for (unsigned short k = 0; k < 4; ++k) {
            data = (data << 8) | key[j];
            j = j + 1;
            if (j >= keybytes)
                j = 0;
        }
        P[i] = P[i] ^ data;
    }

    datal = 0;
    datar = 0;
    for (short i = 0; i < 18; i += 2) {
        Blowfish_encipher(&datal, &datar);
        P[i] = datal;
        P[i + 1] = datar;
    }
    for (short i = 0; i < 4; ++i) {
        for (short k = 0; k < 256; k += 2) {
            Blowfish_encipher(&datal, &datar);
            S[i][k] = datal;
            S[i][k + 1] = datar;
        }
    }
    return (short) error;
}
