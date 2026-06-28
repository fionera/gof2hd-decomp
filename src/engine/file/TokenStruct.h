#ifndef GOF2_TOKENSTRUCT_H
#define GOF2_TOKENSTRUCT_H
#include "engine/core/Array.h"
#include "../core/AEString.h"

namespace AbyssEngine {
    class ConfigReader;

    typedef void (*ConfigTokenReadFunction)(ConfigReader *, void *);

    // lint: void_ptr callback ctx param; baked into RegisterTokenReadFunction ABI signature

    struct TokenStruct {
        String name;
        ConfigTokenReadFunction read;
        void *context; // lint: void_ptr opaque callback ctx; matches RegisterTokenReadFunction ABI
    };
}

#endif
