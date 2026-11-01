// Blowfish reference implementation.  The original Android build compiled this as C:
// the S/P arrays are tentative definitions, so they are COMMON and gold places them in
// the .bss COMMON block at the tail (before the other COMMON globals), matching the
// original.  Compiling as C++ would make S/P ordinary .bss objects and put them at the
// Blowfish TU's early link position instead.

#include <stdio.h>

unsigned int S[4][256];
unsigned int P[18];

extern FILE *SubkeyFile;

unsigned int F(unsigned int value);

// Opens the Blowfish subkey file.
int opensubkeyfile(void) {
    SubkeyFile = fopen("Blowfish.dat", "rb");
    return SubkeyFile != NULL ? 0 : -1;
}

// Blowfish round function over the S-boxes loaded by InitializeBlowfish.
unsigned int F(unsigned int value) {
    const unsigned int a = value >> 24;
    const unsigned int b = (value >> 16) & 0xff;
    const unsigned int c = (value >> 8) & 0xff;
    const unsigned int d = value & 0xff;

    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
}

// Enciphers one 64-bit block.
void Blowfish_encipher(unsigned int *xl, unsigned int *xr) {
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

// Deciphers one 64-bit block.
void Blowfish_decipher(unsigned int *xl, unsigned int *xr) {
    unsigned int Xl = *xl;
    unsigned int Xr = *xr;
    unsigned int temp;
    short i;
    for (i = 17; i > 1; --i) {
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
short InitializeBlowfish(unsigned char key[], short keybytes) {
    unsigned int data;
    unsigned int datal;
    unsigned int datar;

    int error = opensubkeyfile();
    if (error != 0) {
        printf("Unable to open subkey initialization file : %d\n", error);
        return (short) error;
    }

    for (unsigned short i = 0; i < 18; ++i) {
        unsigned short numread = (unsigned short) fread(&data, 4, 1, SubkeyFile);
        if (numread != 1)
            return -1;
        P[i] = data;
    }
    for (unsigned short i = 0; i < 4; ++i) {
        for (unsigned short j = 0; j < 256; ++j) {
            unsigned short numread = (unsigned short) fread(&data, 4, 1, SubkeyFile);
            if (numread != 1)
                return -1;
            S[i][j] = data;
        }
    }
    fclose(SubkeyFile);

    short k;
    short j = 0;
    for (short i = 0; i < 18; ++i) {
        data = 0;
        for (k = 0; k < 4; ++k) {
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
