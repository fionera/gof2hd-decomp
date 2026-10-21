#ifndef GOF2_RESOURCEMESH_H
#define GOF2_RESOURCEMESH_H
#include <cstdint>
#include <cstring>
#include "engine/core/AEString.h"

namespace AbyssEngine {
    class ResourceMesh {
    public:
        char *name;
        uint16_t wResourceId;
        uint8_t flag;

        // Defined in-class: weak (inline) C2 symbol in the original, emitted by GameResource.
        ResourceMesh(const char *name, unsigned short id, bool flag)
            : wResourceId(id), flag(static_cast<uint8_t>(flag)) {
            uint32_t len = static_cast<uint32_t>(String::GetStringLength(name));
            this->name = new char[len + 1];
            memcpy(this->name, name, len + 1);
        }

        // Inline (the original exports only a WEAK D2, emitted at 0x77ebc.. by the PaintCanvas TU,
        // right after ReleaseAllResources).
        ~ResourceMesh() {
            delete[] this->name;
            this->name = nullptr;
        }
    };
}

#endif
