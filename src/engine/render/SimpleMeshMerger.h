#ifndef GOF2_SIMPLEMESHMERGER_H
#define GOF2_SIMPLEMESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

namespace AbyssEngine {
    class Mesh;
}

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

class SimpleMeshMerger {
public:
    using Matrix = AbyssEngine::AEMath::Matrix;
    using Vector = AbyssEngine::AEMath::Vector;

    int matrixCount; // +0x0  number of per-mesh transform matrices
    short mergeFactor; // +0x4  merged-mesh creation flag
    unsigned char valid; // +0x6  set once construction completes
    Array<AbyssEngine::Mesh *> meshes; // +0x8  loaded source meshes
    PaintCanvas *canvas; // +0x14 owning paint canvas
    unsigned mergedMeshId; // +0x18 handle of the produced merged mesh
    unsigned transformId; // +0x1c handle of the transform holding the merged mesh

    // meshIds:    { count, u16* ids }     -- source mesh ids in `canvas`
    // transforms: { count, Matrix* rows } -- per-mesh placement matrices (0x3c-byte rows)
    SimpleMeshMerger(const Array<unsigned short> &meshIds, Array<Matrix> transforms,
                     PaintCanvas *canvas, unsigned short flags);
};

#endif
