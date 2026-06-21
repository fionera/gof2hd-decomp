#ifndef GOF2_SIMPLEMESHMERGER_H
#define GOF2_SIMPLEMESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

namespace AbyssEngine { class Mesh; }
namespace AbyssEngine { class PaintCanvas; }
using ::AbyssEngine::PaintCanvas;

// SimpleMeshMerger -- combines several source meshes (each placed by a per-mesh
// matrix) into one merged mesh. The merged mesh is created inside `canvas`'s mesh
// table; a Transform is then built to hold it. Construction does all the work; the
// resulting object just records the handles produced.
//
// Layout mirrors the reconstructed field map (32 bytes): the embedded Array<Mesh*>
// sits at +0x8, canvas at +0x14, the merged-mesh / transform handles at +0x18 / +0x1c.
class SimpleMeshMerger {
public:
    using Matrix = AbyssEngine::AEMath::Matrix;
    using Vector = AbyssEngine::AEMath::Vector;

    int          matrixCount;   // +0x0  number of per-mesh transform matrices
    short        mergeFactor;   // +0x4  merged-mesh creation flag
    unsigned char valid;        // +0x6  set once construction completes
    Array<AbyssEngine::Mesh*> meshes; // +0x8  loaded source meshes
    PaintCanvas* canvas;        // +0x14 owning paint canvas
    unsigned     mergedMeshId;  // +0x18 handle of the produced merged mesh
    unsigned     transformId;   // +0x1c handle of the transform holding the merged mesh

    // meshIds:    { count, u16* ids }     -- source mesh ids in `canvas`
    // transforms: { count, Matrix* rows } -- per-mesh placement matrices (0x3c-byte rows)
    SimpleMeshMerger(const Array<unsigned short>& meshIds, Array<Matrix> transforms,
                     PaintCanvas* canvas, unsigned short flags);
};

#endif
