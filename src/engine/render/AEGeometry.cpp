#include "engine/render/AEGeometry.h"
#include "engine/render/LodMeshMerger.h"
#include "platform/libc.h"
// NOTE: gof2/Transform.h is intentionally NOT included here. It declares an
// AbyssEngine::Mesh that conflicts with the complete AbyssEngine::Mesh from
// gof2/Mesh.h (pulled in via gof2/LodMeshMerger.h). The only Transform method
// this translation unit needs is SetCurrentAnimationTime, so we forward-declare
// a minimal AbyssEngine::Transform exposing just that member to avoid the header
// clash. The call links to the real out-of-line definition in Transform.cpp.
namespace AbyssEngine {
class Transform {
public:
    void SetCurrentAnimationTime(long long value);
};
} // namespace AbyssEngine

using namespace AbyssEngine::AEMath;

extern "C" void _ae_geom_render(uint32_t canvas, uint32_t tf, int z);
extern "C" void _ae_TransformAddMeshId(uint32_t canvas, uint32_t tf, uint32_t meshId);
extern "C" void _ae_TransformAddMesh(uint32_t canvas, uint32_t tf, uint16_t mesh, int z);
extern "C" void _ae_MatrixSetRotation(void *out, uint32_t loc, float x, float y, float z, int order);
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);
extern "C" void _ae_setPosition3(void *self, float x, float y, float z);
extern "C" void _ae_TransformRemoveChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);
extern "C" void _ae_setmesh_a(uint32_t canvas, uint32_t tf, uint32_t mesh, int z);
extern "C" void _ae_setmesh_b(uint32_t canvas, uint32_t mesh, uint32_t *tfp);
extern "C" void _ae_MatrixSetTranslation(void *out, float x, float y, float z);
extern "C" void _ae_TransformSetLocal(uint32_t canvas, uint32_t tf, void *m);
extern "C" void _ae_MeshCreate(PaintCanvas *c, uint16_t mesh, uint32_t *out, bool flag);
extern "C" void _ae_getDirection(void *self, Vector *out);
extern "C" void _ae_getPosition(void *self, Vector *out);
extern "C" uint32_t Transform_GetTransform(uint32_t tf);

// VectorCross(out, b): out = out x b (engine math helper).
void VectorCross(Vector *out, const Vector *b);

// getPosition() -> position column of the local transform matrix.
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetPosition(loc);
}

AEGeometry::~AEGeometry()
{
    delete[] this->lodTransforms;       this->lodTransforms = nullptr;
    delete[] this->lodMeshes;           this->lodMeshes = nullptr;
    delete[] this->lodChildTransforms;  this->lodChildTransforms = nullptr;
    delete[] this->lodChildMeshes;      this->lodChildMeshes = nullptr;
    delete[] this->lodDistancesSq;      this->lodDistancesSq = nullptr;
}

bool AEGeometry::hasLod() { return this->lodTransforms != nullptr; }

uint16_t AEGeometry::getID() { return this->mesh; }

uint8_t AEGeometry::isVisible() { return (uint8_t)this->visibility; }

void AEGeometry::setVisible(bool v)
{
    // Both bytes of the visibility pair are set together.
    this->visibility = v ? 0x0101 : 0;
}

void AEGeometry::DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b)
{
    this->mergerIndex = (uint32_t)a;
    this->merger = b;
}

void AEGeometry::addChild(uint32_t child)
{
    AEGeomCanvas::TransformAddChild(this->canvas, this->transform, child);
    uint32_t old = this->childTransform;
    if (old != 0xffffffffu)
        this->parentTransform = old;
    this->childTransform = child;
}

void AEGeometry::render()
{
    if ((uint8_t)this->visibility == 0)
        return;
    _ae_geom_render((uint32_t)(uintptr_t)this->canvas, this->transform, 0);
}

void AEGeometry::translate(const Vector &v)
{
    translate(v.x, v.y, v.z);
}

void AEGeometry::setScaling(float s)
{
    setScaling(s, s, s);
}

Vector AEGeometry::getRotation()
{
    return this->rotation;
}

void AEGeometry::setScaling(const Vector &v)
{
    setScaling(v.x, v.y, v.z);
}

void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count)
{
    this->lodMeshes = new uint16_t[count];
    this->lodDistancesSq = new unsigned long long[count];
    this->lodCount = count;
    this->lodTransforms = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        this->lodMeshes[i] = meshes[i];
        this->lodDistancesSq[i] = (unsigned long long)(long long)dists[i];
        AEGeomCanvas::TransformCreate(this->canvas, &this->lodTransforms[i]);
        _ae_TransformAddMeshId((uint32_t)(uintptr_t)this->canvas, this->lodTransforms[i], meshIds[i]);
        unsigned long long v = this->lodDistancesSq[i];
        this->lodDistancesSq[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            AEGeomCanvas::TransformAddChild(this->canvas, this->lodTransforms[i], this->childTransform);
    }
}

void AEGeometry::setLodLastVisibleDistance(uint64_t d)
{
    this->lastVisibleDistSq = d * d;
}

Vector AEGeometry::getRightVector()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetRight(loc);
}

Vector AEGeometry::getUpVector()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetUp(loc);
}

Matrix &AEGeometry::getMatrix()
{
    return *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
}

// getDirection() -> forward axis of the local transform matrix.
Vector AEGeometry::getDirection()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetDir(loc);
}

Matrix &AEGeometry::getReferenceMatrix()
{
    return this->referenceMatrix;
}

void AEGeometry::setMatrix(const Matrix &m)
{
    AEGeomCanvas::TransformSetLocal(this->canvas, this->transform, (Matrix *)&m);
}

void AEGeometry::setRotationOrder(AbyssEngine::AEMath::RotationOrder order)
{
    this->rotationOrder = order;
}

void AEGeometry::setPosition(float x, float y, float z)
{
    _ae_setPosition3(this, x, y, z);
}

Vector AEGeometry::getScaling()
{
    Vector v;
    v.x = this->scalingX;
    v.y = this->scalingY;
    v.z = this->scalingZ;
    return v;
}

void AEGeometry::updateReferenceMatrix()
{
    Matrix *loc = (Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    this->referenceMatrix = *loc;
}

void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count)
{
    this->lodMeshes = new uint16_t[count];
    this->lodDistancesSq = new unsigned long long[count];
    this->lodCount = count;
    this->lodTransforms = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        this->lodMeshes[i] = meshes[i];
        this->lodDistancesSq[i] = (unsigned long long)(long long)dists[i];
        AEGeomCanvas::TransformCreate(this->canvas, &this->lodTransforms[i]);
        _ae_TransformAddMesh((uint32_t)(uintptr_t)this->canvas, this->lodTransforms[i], meshes[i], 0);
        unsigned long long v = this->lodDistancesSq[i];
        this->lodDistancesSq[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            AEGeomCanvas::TransformAddChild(this->canvas, this->lodTransforms[i], this->childTransform);
    }
}

Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = (uint32_t)(uintptr_t)this->canvas;
    uint32_t tf = this->altTransform;
    if (tf == 0xffffffffu)
        tf = this->transform;
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal(canvas, tf);
    return MatrixGetPosition(loc);
}

void AEGeometry::setRotation(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetRotation(buf, loc, x, y, z, this->rotationOrder);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling(buf, loc, this->scalingX, this->scalingY, this->scalingZ);
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
}

void AEGeometry::setRotation(const Vector &v)
{
    setRotation(v.x, v.y, v.z);
}

void AEGeometry::setPosition(const Vector &v)
{
    setPosition(v.x, v.y, v.z);
}

void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildMeshes = new uint16_t[count];
        this->lodChildTransforms = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            this->lodChildMeshes[i] = meshes[i];
            AEGeomCanvas::TransformCreate(this->canvas, &this->lodChildTransforms[i]);
            _ae_TransformAddMesh((uint32_t)(uintptr_t)this->canvas, this->lodChildTransforms[i], meshes[i], 0);
            AEGeomCanvas::TransformAddChild(this->canvas, this->lodTransforms[i], this->lodChildTransforms[i]);
            _ae_TransformRemoveChild(this->canvas, this->lodTransforms[i], this->childTransform);
            AEGeomCanvas::TransformAddChild(this->canvas, this->lodTransforms[i], this->childTransform);
            count = this->lodCount;
        }
    }
}

void AEGeometry::rotate(float x, float y, float z)
{
    char buf[60];
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetRotation(buf, loc, this->rotation.x, this->rotation.y, this->rotation.z,
                          this->rotationOrder);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling(buf, loc, this->scalingX, this->scalingY, this->scalingZ);
}

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    this->cameraDelta = Vector{0, 0, 0};
    this->referenceMatrix = Matrix();
    this->mesh = 0;
    this->canvas = canvas;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);

    this->rotation = Vector{0.0f, 0.0f, 0.0f};
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lodDistancesSq = nullptr;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->meshId = 0xffffffff;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->merger = nullptr;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
}

void AEGeometry::setMesh(uint16_t mesh)
{
    uint32_t canvas = (uint32_t)(uintptr_t)this->canvas;
    if (this->transform != 0)
        return _ae_setmesh_a(canvas, this->transform, mesh, 0);
    return _ae_setmesh_b(canvas, mesh, &this->transform);
}

void AEGeometry::translate(float x, float y, float z)
{
    char buf[60];
    char src[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    memcpy(src, (void *)loc, 0x3c);
    _ae_MatrixSetTranslation(buf, *(float *)(src + 0x0c) + z, *(float *)(src + 0x1c),
                             *(float *)(src + 0x2c) + y);
    _ae_TransformSetLocal((uint32_t)(uintptr_t)this->canvas, this->transform, buf);
    (void)x;
}

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
{
    this->cameraDelta = Vector{0, 0, 0};
    this->referenceMatrix = Matrix();
    this->canvas = canvas;
    this->mesh = mesh;
    this->transform = 0;
    this->baseTransform = 0;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);
    _ae_MeshCreate(canvas, mesh, &this->meshId, flag);
    _ae_TransformAddMeshId((uint32_t)(uintptr_t)canvas, this->baseTransform, this->meshId);

    this->rotation = Vector{0.0f, 0.0f, 0.0f};
    this->scalingX = 1.0f;
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->lodTransforms = nullptr;
    this->lodChildTransforms = nullptr;
    this->lodMeshes = nullptr;
    this->lodChildMeshes = nullptr;
    this->lodDistancesSq = nullptr;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)canvas, 0);
    this->referenceMatrix = *(Matrix *)loc;
    this->merger = nullptr;
}

void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetRotation(buf, loc, this->rotation.x, this->rotation.y, this->rotation.z,
                          this->rotationOrder);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    this->scalingX = x;
    this->scalingY = y;
    this->scalingZ = z;
}

void AEGeometry::moveForward(float dist)
{
    Vector pos;
    Vector n;
    _ae_getDirection(this, &pos);
    VectorNormalize(&n, &pos);
    _ae_getPosition(this, &pos);
    pos.x = pos.x + n.x * dist;
    pos.y = pos.y + n.y * dist;
    pos.z = pos.z + n.z * dist;
    this->setPosition(pos);
}

void AEGeometry::updateLod(const Vector &camPos, float screenScale)
{
    // Mirror the high byte of the visibility pair into the low byte.
    this->visibility = (this->visibility & 0xff00) | (this->visibility >> 8);

    char matrixCopy[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    memcpy(matrixCopy, (void *)loc, 0x3c);

    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    Vector pos = MatrixGetPosition(*(Matrix *)loc);
    this->cameraDelta = camPos - pos;

    float dx = this->cameraDelta.x, dy = this->cameraDelta.y, dz = this->cameraDelta.z;
    this->distSq = (unsigned long long)(dy * dy + dx * dx + dz * dz);

    unsigned long long lastVis = this->lastVisibleDistSq;
    bool visible;
    if (lastVis == 0) {
        visible = true;            // no clamp configured -> always considered visible
    } else {
        visible = this->distSq < lastVis;
        this->visibility = (this->visibility & 0xff00) | (visible ? 1 : 0);
    }

    if (!visible) {
        this->currentLod = -1;
        return;
    }

    Transform_GetTransform((uint32_t)(uintptr_t)this->canvas);

    // Detail factor: tighter LOD thresholds when the object is small on screen.
    float factor = (screenScale <= 0.0625f) ? 0.75f : 1.0f;
    float detail = (0.03125f < screenScale) ? factor : 0.5f;

    int level = this->lodCount;

    // Walk levels high->low; stop at the first whose scaled threshold is met.
    while (level >= 1) {
        int idx = level - 1;
        float thresh = (float)this->lodDistancesSq[idx];
        float d = (float)this->distSq;
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }
        // This level is the one to use.
        uint32_t lodTf = this->lodTransforms[idx];
        if (lodTf != this->transform) {
            AEGeomCanvas::TransformSetLocal(this->canvas, this->transform, (Matrix *)(uintptr_t)lodTf);
            this->transform = this->lodTransforms[idx];
            uint32_t t = Transform_GetTransform((uint32_t)(uintptr_t)this->canvas);
            ((AbyssEngine::Transform *)(uintptr_t)t)->SetCurrentAnimationTime(0);
            t = Transform_GetTransform((uint32_t)(uintptr_t)this->canvas);
            ((AbyssEngine::Transform *)(uintptr_t)t)->SetCurrentAnimationTime(0);
            this->currentLod = level;
            this->referenceMatrix = *(Matrix *)matrixCopy;
            if (this->merger != nullptr)
                this->merger->setLod(this->mergerIndex, (signed char)level);
        }
        return;
    }

    // No level matched: fall back to the base mesh.
    AEGeomCanvas::TransformSetLocal(this->canvas, this->transform, (Matrix *)(uintptr_t)this->baseTransform);
    this->currentLod = 0;
    this->transform = this->baseTransform;
    if (this->merger != nullptr)
        this->merger->setLod(this->mergerIndex, 0);
}

void AEGeometry::setDirection(const Vector &dir, const Vector &up)
{
    char local[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    memcpy(local, (void *)loc, 0x3c);

    // right = normalize(up x dir)
    Vector right = up;
    VectorCross(&right, &dir);
    Vector tmp;
    VectorNormalize(&tmp, &right);
    right = tmp;

    // recomputedUp = normalize(right x dir)
    Vector rUp;
    VectorCross(&tmp, &dir);     // tmp currently == normalized right
    rUp = tmp;
    VectorNormalize(&tmp, &rUp);
    rUp = tmp;

    // Assemble basis rows (right, up, forward) into the local matrix.
    Matrix &m = *(Matrix *)local;
    m.m[0] = right.x;  m.m[1] = right.y;  m.m[2] = right.z;
    m.m[3] = rUp.x;    m.m[4] = rUp.y;    m.m[5] = rUp.z;
    m.m[6] = dir.x;    m.m[7] = dir.y;    m.m[8] = dir.z;

    AEGeomCanvas::TransformSetLocal(this->canvas, this->transform, &m);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling((void *)local, loc, this->scalingX, this->scalingY, this->scalingZ);
}

void AEGeometry::rotate(const Vector &v)
{
    rotate(v.x, v.y, v.z);
}

void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildTransforms = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            AEGeomCanvas::TransformAddChild(this->canvas, this->lodTransforms[i], param);
            count = this->lodCount;
        }
    }
}
