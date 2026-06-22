#ifndef GOF2_ABYSSENGINE_H
#define GOF2_ABYSSENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"   // AbyssEngine::AEMath::Vector / Matrix, AbyssEngine::Quaternion

namespace AbyssEngine {
    class Engine;
}

using ::AbyssEngine::Engine;

namespace AbyssEngine {
    class Material;
}

using ::AbyssEngine::Material;

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

namespace AbyssEngine {
    struct Image;
}

using ::AbyssEngine::Image;

namespace AbyssEngine {
    struct Image2D;
}

using ::AbyssEngine::Image2D;

namespace AbyssEngine {
    struct ImageFont;
}

using ::AbyssEngine::ImageFont;

namespace AbyssEngine {
    struct Curve;
}

using ::AbyssEngine::Curve;

namespace AbyssEngine {
    struct SpriteSystem;
}

using ::AbyssEngine::SpriteSystem;

namespace AbyssEngine {
    struct AELoadedTexture;
}

using ::AbyssEngine::AELoadedTexture;

namespace AbyssEngine {
    using AEMath::Vector;
    using AEMath::Matrix;

    struct ESMatrix {
        float m[4][4];
    };

    class Camera;
} // namespace AbyssEngine

static inline int16_t &s16(void *self, uint32_t off) { return *(int16_t *) ((char *) self + off); }

#endif // GOF2_ABYSSENGINE_H
