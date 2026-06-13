#ifndef GOF2_AEGEOMETRY_H
#define GOF2_AEGEOMETRY_H
#include "gof2/common.h"
#include <new>
// Galaxy on Fire 2 -- AbyssEngine AEGeometry (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "AEGeometry::isVisible()" (no AbyssEngine:: on the class).
// Field offsets recovered per-method from the target disassembly.
//
// Field offsets (recovered):
//   +0x00 u32  mergerIndex          +0x04 LodMeshMerger* merger
//   +0x08 u16  mesh (field_0x8)     +0x0c u32 transform handle
//   +0x10 u32  parentTransform      +0x14 u32 childTransform
//   +0x18 u32  baseTransform        +0x1c u32 meshId
//   +0x20 u32  (mesh handle)        +0x24 u32 altTransform
//   +0x28 i32  currentLod           +0x2c PaintCanvas* canvas
//   +0x30 V4   rotation (xyz) / +0x3c..0x44 scaling xyz  (field_0x30, aliased)
//   +0x48 u16  visibility (field_0x48)  +0x4c i32 rotationOrder  +0x50 i32 lodCount
//   +0x54 V4   lod-array pointers (tf/childTf/mesh/childMesh)  (field_0x54)
//   +0x68 u64  distSq (field_0x68)  +0x70 u64 lastVisibleDistSq (field_0x70)
//   +0x78 V4   cameraDelta          +0x84 Matrix referenceMatrix (field_0x84)

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct PaintCanvas;   // ::PaintCanvas (defined in Radio.h); used by pointer here

// Four-float NEON-ish vector used for rotation/scaling and the LOD pointer block.
struct V4 { float a, b, c, d; };

namespace AbyssEngine {
namespace AEMath {
// AEMath free functions (resolved blx targets in the target).
void VectorNormalize(void *out, const Vector *v);        // 0x0006ec80
Vector operator-(const Vector &a, const Vector &b);      // 0x0006ec38
Vector MatrixGetUp(const Matrix &m);                     // 0x0006f4d8
Vector MatrixGetRight(const Matrix &m);                  // 0x0006f4e4
Vector MatrixGetPosition(const Matrix &m);               // 0x0006f16c
Vector MatrixGetDir(const Matrix &m);                    // 0x00080720
} // namespace AEMath
} // namespace AbyssEngine

// PaintCanvas transform/mesh static helpers (first arg is the canvas handle/pointer).
struct AEGeomCanvas {
    static uint32_t TransformGetLocal(uint32_t canvas, uint32_t tf);     // 0x000720c4
    static void TransformCreate(PaintCanvas *canvas, uint32_t *out);     // 0x000720ac
    static void TransformSetLocal(PaintCanvas *canvas, uint32_t tf, Matrix *m); // 0x000721c0
    static void TransformAddChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);    // 0x000720d0
};

class AEGeometry {
public:
    uint32_t mergerIndex;                 // +0x0  mergerIndex
    void *merger;                    // +0x4  LodMeshMerger*
    uint16_t mesh;                 // +0x8  mesh
    uint16_t pad_0xa;
    uint32_t transform;                 // +0xc  transform handle
    uint32_t parentTransform;                // +0x10
    uint32_t childTransform;                // +0x14
    uint32_t baseTransform;                // +0x18
    uint32_t meshId;                // +0x1c
    uint32_t meshHandle;                // +0x20
    uint32_t altTransform;                // +0x24
    int32_t currentLod;                 // +0x28 currentLod
    void *canvas;                   // +0x2c canvas
    V4 rotation;                      // +0x30 rotation.xyz + (.d == scaling.x at 0x3c)
    float scalingY;                   // +0x40 scaling.y
    float scalingZ;                   // +0x44 scaling.z
    uint16_t visibility;                // +0x48 visibility
    uint16_t pad_0x4a;
    int32_t rotationOrder;                 // +0x4c rotation order
    int32_t lodCount;                 // +0x50 lodCount
    V4 lodPtrs;                      // +0x54 lod ptr block (tf/childTf/mesh/childMesh)
    uint32_t pad_0x64;                  // +0x64 lod distance array ptr
    uint32_t pad_0x67;
    unsigned long long distSq;      // +0x68 distSq
    uint64_t lastVisibleDistSq;                // +0x70 lastVisibleDistSq
    V4 cameraDelta;                      // +0x78 cameraDelta
    uint32_t pad_0x88;
    Matrix referenceMatrix;                  // +0x84 referenceMatrix

    // scaling.x aliases rotation.d
    float &scaleX() { return rotation.d; }
    // lod pointer accessors (the V4 block at 0x54 holds 4 pointers)
    void *&lodTf()        { return *(void **)&lodPtrs.a; }   // +0x54
    void *&lodChildTf()   { return *(void **)&lodPtrs.b; }   // +0x58
    void *&lodMesh()      { return *(void **)&lodPtrs.c; }   // +0x5c
    void *&lodChildMesh() { return *(void **)&lodPtrs.d; }   // +0x60
    void *&lodDist()      { return *(void **)&pad_0x64; }       // +0x64

    AEGeometry(PaintCanvas *canvas);
    AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag);
    ~AEGeometry();

    Vector getPosition();
    Vector getRotation();
    Vector getScaling();
    Vector getRightVector();
    Vector getUpVector();
    Vector getParentPosition();
    Vector getDirection();
    Matrix &getMatrix();
    Matrix &getReferenceMatrix();
    void setMatrix(const Matrix &m);
    void setRotationOrder(int order);

    bool hasLod();
    uint8_t isVisible();
    void setVisible(bool v);
    void render();
    void updateReferenceMatrix();

    void addChild(uint32_t child);
    void setMesh(uint16_t mesh);

    void translate(float x, float y, float z);
    void translate(const Vector &v);
    void setScaling(float x, float y, float z);
    void setScaling(float s);
    void setScaling(const Vector &v);
    void setRotation(float x, float y, float z);
    void setRotation(const Vector &v);
    void setPosition(const Vector &v);
    void setPosition(float x, float y, float z);
    void rotate(float x, float y, float z);
    void rotate(const Vector &v);
    void moveForward(float dist);
    void setDirection(const Vector &dir, const Vector &up);
    // Aim the geometry's forward axis along 'dir', deriving the up vector from
    // the geometry's current local up (single-argument convenience overload).
    void setDirection(const Vector &dir);
    // Re-commit the geometry's current transform position (called after the
    // owning object mutates its position fields, to keep the transform in sync).
    void positionChanged();

    void setLodMeshes(uint16_t *meshes, int *dists, int count);
    void setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count);
    void setLodChildMeshes(uint16_t *meshes);
    void setLodChildTransform(uint32_t param);
    void setLodLastVisibleDistance(uint64_t d);
    void updateLod(const Vector &camPos, float screenScale);
    void DEBUG_setMeshMergerIndex(int a, void *b);
};
#endif
