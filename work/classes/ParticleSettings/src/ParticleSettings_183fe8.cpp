#include "class.h"

extern "C" void *String_ctor(void *s);  // AbyssEngine::String::String() -> this

// ParticleSettings::ParticleSettings(): default-construct the embedded String at the
// head of each of the 48 ParticleSet entries (stride 0xa0, total 0x1e00 bytes).
ParticleSettings::ParticleSettings() {
    int i = 0;
    do {
        String_ctor((char *)this + i);
        i += 0xa0;
    } while (i != 0x1e00);
}
