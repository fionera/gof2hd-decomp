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
    uint16_t field_0x4;    // +0x4   flags
    uint8_t  field_0x6;    // +0x6   initialized flag
    uint8_t  pad7;
    void    *field_0x8;    // +0x8   source mesh pointer table
    void    *field_0xc;    // +0xc   PaintCanvas*
    uint32_t field_0x10;   // +0x10  merged mesh id
    uint32_t field_0x14;   // +0x14  transform id
    void    *field_0x18;   // +0x18  transformed mesh slots
    void    *field_0x1c;   // +0x1c  per-row matrices
    int      field_0x20;   // +0x20  merged mesh pointer
    void    *field_0x24;   // +0x24  per-row LOD bytes
    void    *field_0x28;   // +0x28  per-row enabled bytes
    void    *field_0x2c;   // +0x2c  per-row visible bytes
    int      field_0x30;   // +0x30  cols (LOD count)
    uint8_t  field_0x34;   // +0x34  dirty flag

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
