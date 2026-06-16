#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "common.h"

namespace AbyssEngine {

// A resource node that owns two heap-allocated arrays which are released on
// destruction.
class ResourceTransform {
public:
    void* dataA;
    void* dataB;

    ~ResourceTransform();
};

} // namespace AbyssEngine
#endif
