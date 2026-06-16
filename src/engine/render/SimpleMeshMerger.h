#ifndef GOF2_SIMPLEMESHMERGER_H
#define GOF2_SIMPLEMESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {

class Mesh;
class PaintCanvas;

// SimpleMeshMerger — combines several source meshes (each placed by a per-mesh
// matrix) into one merged mesh. The merged mesh is created inside `canvas`'s mesh
// table; a Transform is then built to hold it. Construction does all the work; the
// resulting object just records the handles produced.
class SimpleMeshMerger {
public:
    int          matrixCount;   // number of per-mesh transform matrices
    short        mergeFactor;
    unsigned char valid;        // set once construction completes
    Array<Mesh*> meshes;        // loaded source meshes
    PaintCanvas* canvas;
    unsigned     mergedMeshId;  // handle of the produced merged mesh
    unsigned     transformId;   // handle of the transform holding the merged mesh

    // meshIds:   { count, u16* ids }       — source mesh ids in `canvas`
    // transforms:{ count, Matrix* rows }   — per-mesh placement matrices (0x3c-byte rows)
    SimpleMeshMerger(unsigned* meshIds, unsigned* transforms, PaintCanvas* canvas, float factor);
};

} // namespace AbyssEngine

#endif
