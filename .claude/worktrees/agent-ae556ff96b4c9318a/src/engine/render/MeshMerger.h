#ifndef GOF2_MESHMERGER_H
#define GOF2_MESHMERGER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

// Galaxy on Fire 2 -- MeshMerger.
//
// Merges several source meshes into a single combined mesh: each source mesh's
// vertices, normals, uvs, colours and triangles are transformed by a per-row
// matrix and packed into one shared vertex/index buffer, which is re-uploaded
// whenever the visible/enabled selection changes.

// Engine canvas type; its real type lives in namespace AbyssEngine
// (see gof2/game/core/PaintCanvasClass.h). Used only through a pointer here.
// Engine types live in namespace AbyssEngine; MeshMerger itself is a global
// (its mangled symbol is _ZN10MeshMerger..., no AbyssEngine prefix).
namespace AbyssEngine { class PaintCanvas; class Mesh; }
using ::AbyssEngine::PaintCanvas;

class MeshMerger {
public:
    using Mesh   = AbyssEngine::Mesh;
    using Matrix = AbyssEngine::AEMath::Matrix;
    using Vector = AbyssEngine::AEMath::Vector;

    int          rows;               // source-mesh rows (mesh count)
    uint16_t     flags;              // merged-mesh creation flags
    uint8_t      initialized;        // non-zero once the merge has been built
    void*        sourceMeshes;       // source mesh pointer table (rows * cols)
    PaintCanvas* canvas;             // owning paint canvas
    uint32_t     mergedMeshId;       // id of the merged mesh
    uint32_t     transformId;        // shared transform id
    void**       transformedMeshes;  // transformed-mesh slot table (rows * cols)
    char*        matrices;           // per-row transform matrices (each 0x3c bytes)
    void*        mergedMesh;         // merged Mesh pointer
    int8_t*      lods;               // active LOD per row
    uint8_t*     enabledFlags;       // per-row enabled flag
    uint8_t*     visibleFlags;       // per-row visibility flag
    int          cols;               // LOD levels per row
    uint8_t      dirty;              // merged geometry needs rebuilding

    MeshMerger(const Array<uint16_t>& meshIds, Array<Matrix> transforms,
               PaintCanvas* canvas, uint16_t flags);
    MeshMerger(int rows, int cols, PaintCanvas* canvas, uint16_t flags);
    ~MeshMerger();

    void  setMatrix(int index, const Matrix& m);
    void  setLod(int index, signed char lod);
    void  setMesh(int index, signed char lod, uint16_t meshId);
    void  setEnabled(int index, bool enabled);
    void  render();
    void  update();
    int   init();
    void* transformMesh(Mesh* mesh, const Matrix& m);
};

#endif
