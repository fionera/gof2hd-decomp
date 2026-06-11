#ifndef GOF2_MESHMERGER_H
#define GOF2_MESHMERGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.


namespace AbyssEngine {

struct Mesh;        // full definition in gof2/Mesh.h (AbyssEngine::Mesh)
struct PaintCanvas;

namespace AEMath {

// AEMath::Matrix is a 15-float (0x3c byte) object with an engine ctor/operator=.

} // namespace AEMath

} // namespace AbyssEngine

struct Mesh;   // global Mesh view (full definition in gof2/Mesh.h)

// Field accessors via byte offset (recovered code reads fields by raw offset).

struct MeshMerger {
    int      field_0x0;    // +0x0   rows (mesh count)
    uint16_t flags;    // +0x4   flags
    uint8_t  initialized;    // +0x6   initialized flag
    uint8_t  pad7;
    void    *sourceMeshes;    // +0x8   source mesh pointer table
    void    *canvas;    // +0xc   PaintCanvas*
    uint32_t mergedMeshId;   // +0x10  merged mesh id
    uint32_t transformId;   // +0x14  transform id
    void    *transformedMeshes;   // +0x18  transformed mesh slots
    void    *matrices;   // +0x1c  per-row matrices
    int      mergedMesh;   // +0x20  merged mesh pointer
    void    *lods;   // +0x24  per-row LOD bytes
    void    *enabledFlags;   // +0x28  per-row enabled bytes
    void    *visibleFlags;   // +0x2c  per-row visible bytes
    int      field_0x30;   // +0x30  cols (LOD count)
    uint8_t  dirty;   // +0x34  dirty flag

    MeshMerger(const Array<uint16_t> &meshIds, Array<AbyssEngine::AEMath::Matrix> transforms,
               AbyssEngine::PaintCanvas *canvas, uint16_t flags);
    MeshMerger(int rows, int cols, AbyssEngine::PaintCanvas *canvas, uint16_t flags);
    ~MeshMerger();
    void setMatrix(int index, const AbyssEngine::AEMath::Matrix &m);
    void setLod(int index, signed char lod);
    void setMesh(int index, signed char lod, uint16_t meshId);
    void setEnabled(int index, bool enabled);
    void render();
    void update();
    int  init();
    void *transformMesh(AbyssEngine::Mesh *mesh, const AbyssEngine::AEMath::Matrix &m);
};
#endif
