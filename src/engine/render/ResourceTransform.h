#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include <cstdint>
#include <new>

namespace AbyssEngine {
    class ResourceTransform {
    public:
        uint8_t header[0x40];
        uint8_t *dataA;
        uint32_t pad44;
        uint8_t *dataB;

        // Inline (the original exports only a WEAK D2, emitted at 0x77ebc.. by the PaintCanvas TU,
        // right after ReleaseAllResources).
        ~ResourceTransform() {
            if (this->dataA != nullptr) {
                ::operator delete[](this->dataA); // lint: operator_call -- explicit call keeps the null test (a delete[] expression elides it; orig keeps the cbz)
            }
            this->dataA = nullptr;

            if (this->dataB != nullptr) {
                ::operator delete[](this->dataB); // lint: operator_call -- see above
            }
            this->dataB = nullptr;
        }
    };
}
#endif
