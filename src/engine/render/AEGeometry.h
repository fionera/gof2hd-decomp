#ifndef GOF2_AEGEOMETRY_H
#define GOF2_AEGEOMETRY_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "engine/math/AEMath.h"
// Galaxy on Fire 2 -- AbyssEngine AEGeometry.
// A renderable transform node: owns a base transform plus optional LOD levels,
// each with its own transform/mesh handles and a squared-distance threshold.

namespace AbyssEngine { class PaintCanvas; }   // real type in engine/render/PaintCanvas.h; pointer only
using ::AbyssEngine::PaintCanvas;
class LodMeshMerger;  // engine/render/LodMeshMerger.h; pointer only (global scope)

namespace AbyssEngine {

namespace AEMath {
void   VectorNormalize(Vector *out, const Vector *v);
Vector operator-(const Vector &a, const Vector &b);
Vector MatrixGetUp(const Matrix &m);
Vector MatrixGetRight(const Matrix &m);
Vector MatrixGetPosition(const Matrix &m);
Vector MatrixGetDir(const Matrix &m);
} // namespace AEMath

} // namespace AbyssEngine

// PaintCanvas transform/mesh static helpers (first arg is the canvas handle/pointer).
struct AEGeomCanvas {
    static uint32_t TransformGetLocal(uint32_t canvas, uint32_t tf);
    static void     TransformCreate(PaintCanvas *canvas, uint32_t *out);
    static void     TransformSetLocal(PaintCanvas *canvas, uint32_t tf, Matrix *m);
    static void     TransformAddChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);
};

class AEGeometry {
public:
    uint32_t                 mergerIndex;
    LodMeshMerger           *merger;
    uint16_t                 mesh;
    uint32_t                 transform;
    uint32_t                 parentTransform;
    uint32_t                 childTransform;
    uint32_t                 baseTransform;
    uint32_t                 meshId;
    uint32_t                 meshHandle;
    uint32_t                 altTransform;
    int32_t                  currentLod;
    PaintCanvas             *canvas;

    // rotation.xyz are the Euler angles; scalingX aliases the rotation block's
    // fourth slot in the original layout, so it is kept as a sibling field.
    Vector                   rotation;
    float                    scalingX;
    float                    scalingY;
    float                    scalingZ;

    uint16_t                 visibility;
    int32_t                  rotationOrder;
    int32_t                  lodCount;

    // Per-LOD parallel arrays (length == lodCount), allocated lazily.
    uint32_t                *lodTransforms;
    uint32_t                *lodChildTransforms;
    uint16_t                *lodMeshes;
    uint16_t                *lodChildMeshes;
    unsigned long long      *lodDistancesSq;

    unsigned long long       distSq;
    unsigned long long       lastVisibleDistSq;
    Vector                   cameraDelta;
    Matrix                   referenceMatrix;

    AEGeometry(PaintCanvas *canvas);
    AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag);
    ~AEGeometry();

    Vector  getPosition();
    Vector  getRotation();
    Vector  getScaling();
    Vector  getRightVector();
    Vector  getUpVector();
    Vector  getParentPosition();
    Vector  getDirection();
    Matrix &getMatrix();
    Matrix &getReferenceMatrix();
    void    setMatrix(const Matrix &m);
    void    setRotationOrder(AbyssEngine::AEMath::RotationOrder order);

    bool    hasLod();
    uint8_t isVisible();
    void    setVisible(bool v);
    void    render();
    void    updateReferenceMatrix();

    void    addChild(uint32_t child);
    void    setMesh(uint16_t mesh);

    void    translate(float x, float y, float z);
    void    translate(const Vector &v);
    void    setScaling(float x, float y, float z);
    void    setScaling(float s);
    void    setScaling(const Vector &v);
    void    setRotation(float x, float y, float z);
    void    setRotation(const Vector &v);
    void    setPosition(const Vector &v);
    void    setPosition(float x, float y, float z);
    void    rotate(float x, float y, float z);
    void    rotate(const Vector &v);
    void    moveForward(float dist);
    void    setDirection(const Vector &dir, const Vector &up);
    // Aim the geometry's forward axis along 'dir', deriving the up vector from
    // the geometry's current local up (single-argument convenience overload).
    void    setDirection(const Vector &dir);
    // Re-commit the geometry's current transform position (called after the
    // owning object mutates its position fields, to keep the transform in sync).
    void    positionChanged();

    void    setLodMeshes(uint16_t *meshes, int *dists, int count);
    void    setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count);
    void    setLodChildMeshes(uint16_t *meshes);
    void    setLodChildTransform(uint32_t param);
    void    setLodLastVisibleDistance(uint64_t d);
    void    updateLod(const Vector &camPos, float screenScale);
    void    DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b);
};
#endif
