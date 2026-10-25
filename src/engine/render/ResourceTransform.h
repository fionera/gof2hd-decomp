#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "engine/math/Matrix.h"
#include <cstdint>
#include <new>

namespace AbyssEngine {
    // Transform resource descriptor: the local matrix followed by the child mesh
    // and child transform id arrays consumed by PaintCanvas::TransformCreate.
    class ResourceTransform {
    public:
        AEMath::Matrix worldMatrix;
        uint16_t childMeshCount;
        uint16_t *childMeshIds;
        uint16_t childTfCount;
        uint16_t *childTfIds;

        ~ResourceTransform() {
            if (this->childMeshIds != nullptr) {
                ::operator delete[](this->childMeshIds); // lint: operator_call -- a delete[] expression drops the null test the original keeps
            }
            this->childMeshIds = nullptr;

            if (this->childTfIds != nullptr) {
                ::operator delete[](this->childTfIds); // lint: operator_call -- see above
            }
            this->childTfIds = nullptr;
        }
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(__builtin_offsetof(ResourceTransform, childMeshCount) == 0x3c,
                  "ResourceTransform::childMeshCount");
    static_assert(__builtin_offsetof(ResourceTransform, childMeshIds) == 0x40,
                  "ResourceTransform::childMeshIds");
    static_assert(__builtin_offsetof(ResourceTransform, childTfCount) == 0x44,
                  "ResourceTransform::childTfCount");
    static_assert(__builtin_offsetof(ResourceTransform, childTfIds) == 0x48,
                  "ResourceTransform::childTfIds");
    static_assert(sizeof(ResourceTransform) == 0x4c, "ResourceTransform size");
#endif
}
#endif
