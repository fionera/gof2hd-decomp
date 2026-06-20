#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include <cstdint>

namespace AbyssEngine {

// A resource node that owns two heap-allocated arrays which are released on
// destruction. The two owned slots live at the tail of the object (+0x40,
// +0x48); the leading 0x40 bytes hold base/transform state owned elsewhere.
class ResourceTransform {
public:
    uint8_t header[0x40];   // +0x00: leading transform/base state
    uint8_t* dataA;         // +0x40: first owned heap array
    uint32_t pad44;         // +0x44: padding between the two owned slots
    uint8_t* dataB;         // +0x48: second owned heap array

    ~ResourceTransform();
};

} // namespace AbyssEngine
#endif
