#ifndef GOF2_ABYSSENGINE_H
#define GOF2_ABYSSENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/render/Engine.h"
#include "engine/render/Material.h"
#include "engine/render/ImageFont.h"
#include "engine/render/SpriteSystem.h"
#include "engine/render/Curve.h"
#include "engine/render/Image.h"
#include "engine/render/Image2D.h"
#include "engine/render/AELoadedTexture.h"

#include "engine/math/AEMath.h"


namespace AbyssEngine {
    class Engine;
    class Material;
    class PaintCanvas;
}

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

    // ImageFont / SpriteSystem / Curve / Image / Image2D / AELoadedTexture
    // are each defined in their own header (included above).
}

#endif
