

#include <cstdio>

extern "C" std::FILE *gSubkeyFile;
std::FILE *gSubkeyFile = nullptr;

extern "C" int opensubkeyfile() {
    gSubkeyFile = std::fopen("Blowfish.dat", "rb");
    return gSubkeyFile != nullptr ? 0 : -1;
}

extern "C" void *decrypt(void *ctx) {
    return ctx;
}
