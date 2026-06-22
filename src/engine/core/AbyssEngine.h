#ifndef GOF2_ABYSSENGINE_H
#define GOF2_ABYSSENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/render/Engine.h"
#include "engine/render/Material.h"

using ::AbyssEngine::Engine;
using ::AbyssEngine::Material;
using ::AbyssEngine::PaintCanvas;

unsigned int AELabelObject(unsigned int glIdentifier, unsigned int name, const char *label);

namespace AbyssEngine {
    using AEMath::Vector;
    using AEMath::Matrix;

    struct ESMatrix {
        float m[4][4];
    };
}

static inline int16_t &s16(void *self, uint32_t off) { return *(int16_t *) ((char *) self + off); }

#endif
