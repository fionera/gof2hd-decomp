#ifndef GOF2_SIMPLEMESHMERGER_H
#define GOF2_SIMPLEMESHMERGER_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/render/AMeshMerger.h"

namespace AbyssEngine {
    class Mesh;
}

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

class SimpleMeshMerger : public AMeshMerger {
public:
    SimpleMeshMerger(const Array<unsigned short> &meshIds, Array<Matrix> transforms,
                     PaintCanvas *canvas, unsigned short flags);
};

#endif
