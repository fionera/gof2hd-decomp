#ifndef GOF2_RESOURCETEXTURE_H
#define GOF2_RESOURCETEXTURE_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include <cstdint>
#include <cstring>

namespace AbyssEngine {
    class ResourceTexture {
    public:
        char *name;
        float value;

        // Defined in-class: the original emits these as weak (inline) C2 symbols in the
        // GameResource TU that uses them.
        ResourceTexture(const char *name, float value) {
            uint32_t len = String::GetStringLength(name);
            this->name = new char[len + 1U];
            memcpy(this->name, name, len + 1U);
            this->value = value;
        }

        ResourceTexture(const String &name, float value) {
            char *utf8 = name.GetAEChar();
            uint32_t len = name.length;
            this->name = new char[len + 1U];
            memcpy(this->name, utf8, len + 1U);
            this->value = value;
            delete[] utf8;
        }

        // Inline (the original exports only a WEAK D2, emitted at 0x77ebc.. by the PaintCanvas TU,
        // right after ReleaseAllResources).
        ~ResourceTexture() {
            delete[] this->name;
            this->name = nullptr;
        }
    };
}

#endif
