#ifndef GOF2_MATERIALSHADING_H
#define GOF2_MATERIALSHADING_H

#include "engine/math/Vector.h"

namespace AbyssEngine {
    // Shading parameter block of a ResourceMaterial (+0x14): copied by PaintCanvas into the
    // runtime Material (flags1/flags2/vec). It has its own (inline) constructor: in the
    // original the ResourceMaterial constructors are weak C2 symbols that the compiler did not
    // treat as nounwind (their calls in BuildResourceList are invokes with landing pads), which
    // is what a subobject-constructor call in their body produces.
    struct MaterialShading {
        int flags1;
        int flags2;
        Vector vec;

        MaterialShading() : flags1(0), flags2(0) {
            vec.x = -10.0f;
            vec.y = 0.0f;
            vec.z = 0.0f;
        }
    };
}

#endif
