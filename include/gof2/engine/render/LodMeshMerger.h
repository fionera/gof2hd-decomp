#ifndef GOF2_LODMESHMERGER_H
#define GOF2_LODMESHMERGER_H
#include "gof2/common.h"
#include "gof2/math.h"
#include "gof2/engine/render/Mesh.h"

// Galaxy on Fire 2 -- LodMeshMerger.
//
// Merges several LOD variants of a set of source meshes into a single mesh,
// picking a LOD per row to stay within a triangle budget and re-uploading the
// merged geometry whenever the selection changes.

class PaintCanvas;

namespace AbyssEngine {

// Combines per-row source meshes (each available at several LOD levels) into one
// merged mesh. Geometry is transformed per row and the active LOD is chosen so the
// total index count stays under budget.
class LodMeshMerger {
public:
    using Matrix = AEMath::Matrix;
    using Vector = AEMath::Vector;

    int             rows;               // source-mesh rows
    uint16_t        flags;              // merged-mesh creation flags
    uint8_t         initialized;        // non-zero once init() has built the merge
    Array<Mesh*>    sourceMeshes;       // source meshes (rows * cols)
    PaintCanvas*    canvas;             // owning paint canvas
    uint32_t        mergedMeshId;       // id of the merged mesh
    uint32_t        transformId;        // shared transform id
    void*           mergedMesh;         // merged Mesh pointer
    void**          transformedMeshes;  // transformed-mesh slot table (rows * cols)
    Matrix*         transforms;         // per-row transform matrices
    int8_t*         lodLevels;          // active LOD per row
    uint8_t*        enabled;            // per-row enabled flag
    uint8_t*        visible;            // per-row visibility flag
    int             cols;               // LOD levels per row
    uint8_t         dirty;              // merged geometry needs rebuilding

    LodMeshMerger(int rows, int cols, PaintCanvas* canvas, uint16_t flags);
    ~LodMeshMerger();

    void  setEnabled(int index, bool enabled);
    void  setLod(int index, signed char lod);
    void  setMatrix(int index, const Matrix& m);
    void  setMesh(int index, signed char lod, uint16_t meshId);
    void  update();
    int   init();
    void* transformMesh(Mesh* src, const Matrix& m);
};

} // namespace AbyssEngine

#endif
