

#include <cstdio>

static std::FILE *gSubkeyFile = nullptr;

int opensubkeyfile() {
    gSubkeyFile = std::fopen("Blowfish.dat", "rb");
    return gSubkeyFile != nullptr ? 0 : -1;
}

void *decrypt(void *ctx) {
    return ctx;
}
