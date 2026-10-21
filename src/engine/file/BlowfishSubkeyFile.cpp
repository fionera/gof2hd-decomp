

#include <cstdio>

extern "C" unsigned int S[4][256]; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
unsigned int S[4][256];

extern "C" unsigned int P[18]; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
unsigned int P[18];

extern "C" std::FILE *SubkeyFile; // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
std::FILE *SubkeyFile = nullptr;

extern "C" unsigned int F(unsigned int value); // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)

extern "C" short opensubkeyfile() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
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

#define BLOWFISH_N 16
#define BLOWFISH_NO_ERR 0
#define BLOWFISH_DATA_ERROR -1

extern "C" short InitializeBlowfish(unsigned char key[], short keybytes) { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    short i, j, k;
    short error;
    unsigned short numread;
    unsigned int data, datal, datar;

    // First, open the file containing the array initialization data.
    error = opensubkeyfile();
    if (error == BLOWFISH_NO_ERR) {
        for (i = 0; i < BLOWFISH_N + 2; ++i) {
            numread = std::fread(&data, 4, 1, SubkeyFile);
            if (numread != 1)
                return BLOWFISH_DATA_ERROR;
            P[i] = data;
        }
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 256; ++j) {
                numread = std::fread(&data, 4, 1, SubkeyFile);
                if (numread != 1)
                    return BLOWFISH_DATA_ERROR;
                S[i][j] = data;
            }
        }
        std::fclose(SubkeyFile);

        j = 0;
        for (i = 0; i < BLOWFISH_N + 2; ++i) {
            data = 0x00000000;
            for (k = 0; k < 4; ++k) {
                data = (data << 8) | key[j];
                j = j + 1;
                if (j >= keybytes)
                    j = 0;
            }
            P[i] = P[i] ^ data;
        }

        datal = 0x00000000;
        datar = 0x00000000;
        for (i = 0; i < BLOWFISH_N + 2; i += 2) {
            Blowfish_encipher(&datal, &datar);
            P[i] = datal;
            P[i + 1] = datar;
        }
        for (i = 0; i < 4; ++i) {
            for (j = 0; j < 256; j += 2) {
                Blowfish_encipher(&datal, &datar);
                S[i][j] = datal;
                S[i][j + 1] = datar;
            }
        }
    } else {
        std::printf("Unable to open subkey initialization file : %d\n", error);
    }
    return error;
}
