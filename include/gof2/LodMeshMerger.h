#ifndef GOF2_LODMESHMERGER_H
#define GOF2_LODMESHMERGER_H
#include "gof2/common.h"
#include "gof2/math.h"
#include "gof2/Mesh.h"
// Galaxy on Fire 2 -- LodMeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
//
// Field layout (this):
//   0x00 int   rows
//   0x04 u16   flags
//   0x06 u8    initFlag
//   0x08 Array<Mesh*> {len@0x08, data@0x0c}
//   0x14 PaintCanvas* canvas
//   0x18 u32   meshId
//   0x1c u32   transformId
//   0x20 void* outMesh
//   0x24 void** slots (transformed-mesh ptr table)
//   0x28 Matrix* matrices (0x3c each)
//   0x2c int8*  lods
//   0x30 u8*    enabled
//   0x34 u8*    visible
//   0x38 int    cols
//   0x3c u8     dirty

// Mesh is AbyssEngine::Mesh (complete type defined in gof2/Mesh.h). The global
// ::Mesh in fwd.h is only an incomplete forward declaration; an unqualified
// `Mesh` binds to that incomplete type, so we alias the engine type explicitly.
// We cannot use `using AbyssEngine::Mesh;` because it would collide with ::Mesh.
// PaintCanvas is only used as an opaque pointer here (forward-declared in fwd.h).
using Matrix = AbyssEngine::AEMath::Matrix;
using Vector = AbyssEngine::AEMath::Vector;
using AEMesh = AbyssEngine::Mesh;

class LodMeshMerger {
public:
    int32_t            field_0x0;    // +0x00 rows
    uint16_t           field_0x4;    // +0x04 flags
    uint8_t            field_0x6;    // +0x06 initFlag
    Array<AEMesh*>     field_0x8;    // +0x08 source meshes (rows*cols)
    PaintCanvas*       field_0x14;   // +0x14 canvas
    uint32_t           field_0x18;   // +0x18 meshId
    uint32_t           field_0x1c;   // +0x1c transformId
    void*              field_0x20;   // +0x20 outMesh
    void**             field_0x24;   // +0x24 transformed-mesh slots
    Matrix*            field_0x28;   // +0x28 per-row matrices (0x3c each)
    int8_t*            field_0x2c;   // +0x2c lods
    uint8_t*           field_0x30;   // +0x30 enabled
    uint8_t*           field_0x34;   // +0x34 visible
    int32_t            field_0x38;   // +0x38 cols
    uint8_t            field_0x3c;   // +0x3c dirty

    LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags);
    ~LodMeshMerger();
    void setEnabled(int index, bool enabled);
    void setLod(int index, signed char lod);
    void setMatrix(int index, const Matrix &m);
    void setMesh(int index, signed char lod, uint16_t meshId);
    void update();
    int  init();
    void *transformMesh(AEMesh *src, const Matrix &m);
};
#endif
