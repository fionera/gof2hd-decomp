#include "class.h"

extern "C" void String_dtor(void *p);  // AbyssEngine::String::~String

// ParticleSettings::~ParticleSettings(): destruct the embedded String of each
// ParticleSet entry in reverse (from offset 0x1d60 down to 0, stride 0xa0).
ParticleSettings::~ParticleSettings() {
    int i = 0x1d60;
    do {
        String_dtor((char *)this + i);
        i -= 0xa0;
    } while (i != -0xa0);
}
