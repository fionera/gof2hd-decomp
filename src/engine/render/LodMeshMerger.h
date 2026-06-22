#ifndef GOF2_LODMESHMERGER_H
#define GOF2_LODMESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/render/Mesh.h"

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;
using ::AbyssEngine::Mesh;

class LodMeshMerger {
public:
    using Matrix = AbyssEngine::AEMath::Matrix;
    using Vector = AbyssEngine::AEMath::Vector;

    int rows; // source-mesh rows
    uint16_t flags; // merged-mesh creation flags
    uint8_t initialized; // non-zero once init() has built the merge
    Array<Mesh *> sourceMeshes; // source meshes (rows * cols)
    PaintCanvas *canvas; // owning paint canvas
    uint32_t mergedMeshId; // id of the merged mesh
    uint32_t transformId; // shared transform id
    void *mergedMesh; // merged Mesh pointer
    void **transformedMeshes; // transformed-mesh slot table (rows * cols)
    Matrix *transforms; // per-row transform matrices
    int8_t *lodLevels; // active LOD per row
    uint8_t *enabled; // per-row enabled flag
    uint8_t *visible; // per-row visibility flag
    int cols; // LOD levels per row
    uint8_t dirty; // merged geometry needs rebuilding

    LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags);

    ~LodMeshMerger();

    void setEnabled(int index, bool enabled);

    void setLod(int index, signed char lod);

    void setMatrix(int index, const Matrix &m);

    void setMesh(int index, signed char lod, uint16_t meshId);

    void update();

    int init();

    void *transformMesh(Mesh *src, const Matrix &m);
};

#endif
