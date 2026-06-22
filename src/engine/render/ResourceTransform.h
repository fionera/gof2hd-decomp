#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include <cstdint>

namespace AbyssEngine {
    class ResourceTransform {
    public:
        uint8_t header[0x40]; // +0x00: leading transform/base state
        uint8_t *dataA; // +0x40: first owned heap array
        uint32_t pad44; // +0x44: padding between the two owned slots
        uint8_t *dataB; // +0x48: second owned heap array

        ~ResourceTransform();
    };
} // namespace AbyssEngine
#endif
