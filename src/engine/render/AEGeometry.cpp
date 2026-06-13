#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/render/LodMeshMerger.h"
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
extern "C" void *__aeabi_memcpy(void *d, const void *s, uint32_t n);
extern "C" void _ae_MatrixSetTranslation(void *out, float x, float y, float z);
extern "C" void _ae_TransformSetLocal(uint32_t canvas, uint32_t tf, void *m);
extern "C" void _ae_MeshCreate(PaintCanvas *c, uint16_t mesh, uint32_t *out, bool flag);
extern "C" void _ae_getDirection(void *self, Vector *out);
extern "C" void _ae_getPosition(void *self, Vector *out);

// AEGeometry::getPosition() -> TransformGetLocal(canvas,tf); return MatrixGetPosition(loc)
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetPosition(loc);
}

// AEGeometry::~AEGeometry() -> delete[] five LOD arrays in order 0x54,0x5c,0x58,0x60,0x64
AEGeometry::~AEGeometry()
{
    if (this->lodTf())        operator delete[](this->lodTf());
    this->lodTf() = 0;
    if (this->lodMesh())      operator delete[](this->lodMesh());
    this->lodMesh() = 0;
    if (this->lodChildTf())   operator delete[](this->lodChildTf());
    this->lodChildTf() = 0;
    if (this->lodChildMesh()) operator delete[](this->lodChildMesh());
    this->lodChildMesh() = 0;
    if (this->lodDist())      operator delete[](this->lodDist());
    this->lodDist() = 0;
}

bool AEGeometry::hasLod() { return this->lodTf() != 0; }

uint8_t AEGeometry::isVisible() { return (uint8_t)this->visibility; }

void AEGeometry::setVisible(bool v)
{
    // low byte (0x48) and next byte (0x49) both set; field_0x48 is the u16 pair.
    this->visibility = v ? 0x0101 : 0;
}

void AEGeometry::DEBUG_setMeshMergerIndex(int a, void *b)
{
    this->mergerIndex = (uint32_t)a;
    this->merger = b;
}

void AEGeometry::addChild(uint32_t child)
{
    AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas, this->transform, child);
    uint32_t old = this->childTransform;
    if (old != 0xffffffffu)
        this->parentTransform = old;
    this->childTransform = child;
}

void AEGeometry::render()
{
    if ((uint8_t)this->visibility == 0)
        return;
    return _ae_geom_render((uint32_t)(uintptr_t)this->canvas, this->transform, 0);
}

void AEGeometry::translate(const Vector &v)
{
    return translate(v.x, v.y, v.z);
}

void AEGeometry::setScaling(float s)
{
    return setScaling(s, s, s);
}

Vector AEGeometry::getRotation()
{
    Vector v;
    v.x = this->rotation.a;
    v.y = this->rotation.b;
    v.z = this->rotation.c;
    return v;
}

void AEGeometry::setScaling(const Vector &v)
{
    return setScaling(v.x, v.y, v.z);
}

void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count)
{
    this->lodMesh() = new uint16_t[count];
    this->lodDist() = new long long[count];
    this->lodCount = count;
    this->lodTf() = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        ((uint16_t *)this->lodMesh())[i] = meshes[i];
        long long *d = (long long *)this->lodDist();
        d[i] = (long long)dists[i];
        AEGeomCanvas::TransformCreate((PaintCanvas *)this->canvas,
                                      &((uint32_t *)this->lodTf())[i]);
        _ae_TransformAddMeshId((uint32_t)(uintptr_t)this->canvas, ((uint32_t *)this->lodTf())[i], meshIds[i]);
        long long v = ((long long *)this->lodDist())[i];
        ((long long *)this->lodDist())[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas,
                                            ((uint32_t *)this->lodTf())[i], this->childTransform);
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

// AEGeometry::getDirection() -> forward axis of the local transform matrix
Vector AEGeometry::getDirection()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    return MatrixGetDir(loc);
}

// AEGeometry::getReferenceMatrix() -> &field_0x84 (cached reference matrix)
Matrix &AEGeometry::getReferenceMatrix()
{
    return this->referenceMatrix;
}

// AEGeometry::setMatrix(const Matrix&) -> install m as the local transform
void AEGeometry::setMatrix(const Matrix &m)
{
    AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->canvas, this->transform, (Matrix *)&m);
}

// AEGeometry::setRotationOrder(RotationOrder) -> store field_0x4c
void AEGeometry::setRotationOrder(int order)
{
    this->rotationOrder = order;
}

// AEGeometry::setPosition(float,float,float)
void AEGeometry::setPosition(float x, float y, float z)
{
    return _ae_setPosition3(this, x, y, z);
}

Vector AEGeometry::getScaling()
{
    Vector v;
    v.x = this->rotation.d;   // scaling.x aliases 0x3c
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
    this->lodMesh() = new uint16_t[count];
    this->lodDist() = new long long[count];
    this->lodCount = count;
    this->lodTf() = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        ((uint16_t *)this->lodMesh())[i] = meshes[i];
        long long *d = (long long *)this->lodDist();
        d[i] = (long long)dists[i];
        AEGeomCanvas::TransformCreate((PaintCanvas *)this->canvas,
                                      &((uint32_t *)this->lodTf())[i]);
        _ae_TransformAddMesh((uint32_t)(uintptr_t)this->canvas, ((uint32_t *)this->lodTf())[i],
                             (uint16_t)meshes[i], 0);
        long long v = ((long long *)this->lodDist())[i];
        ((long long *)this->lodDist())[i] = v * v;
        if (this->childTransform != 0xffffffffu)
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas,
                                            ((uint32_t *)this->lodTf())[i], this->childTransform);
    }
}

Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = (uint32_t)(uintptr_t)this->canvas;
    uint32_t tf = this->altTransform;
    if (tf + 1 == 0)
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
    _ae_MatrixSetScaling(buf, loc, this->rotation.d, this->scalingY, this->scalingZ);
    this->rotation.a = x;
    this->rotation.b = y;
    this->rotation.c = z;
}

void AEGeometry::setRotation(const Vector &v)
{
    return setRotation(v.x, v.y, v.z);
}

void AEGeometry::setPosition(const Vector &v)
{
    return setPosition(v.x, v.y, v.z);
}

void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildMesh() = new uint16_t[count];
        this->lodChildTf() = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            ((uint16_t *)this->lodChildMesh())[i] = meshes[i];
            AEGeomCanvas::TransformCreate((PaintCanvas *)this->canvas,
                                          &((uint32_t *)this->lodChildTf())[i]);
            _ae_TransformAddMesh((uint32_t)(uintptr_t)this->canvas, ((uint32_t *)this->lodChildTf())[i],
                                 (uint16_t)meshes[i], 0);
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas,
                                            ((uint32_t *)this->lodTf())[i],
                                            ((uint32_t *)this->lodChildTf())[i]);
            _ae_TransformRemoveChild((PaintCanvas *)this->canvas,
                                     ((uint32_t *)this->lodTf())[i], this->childTransform);
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas,
                                            ((uint32_t *)this->lodTf())[i], this->childTransform);
            count = this->lodCount;
        }
    }
}

void AEGeometry::rotate(float x, float y, float z)
{
    char buf[60];
    this->rotation.a = this->rotation.a + x;
    this->rotation.b = this->rotation.b + y;
    this->rotation.c = this->rotation.c + z;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetRotation(buf, loc, this->rotation.a, this->rotation.b, this->rotation.c,
                          this->rotationOrder);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling(buf, loc, this->rotation.d, this->scalingY, this->scalingZ);
}

// AEGeometry::AEGeometry(PaintCanvas*)
__attribute__((visibility("hidden"))) static const V4 kDir = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    this->cameraDelta = (V4){0, 0, 0, 0};
    new (&this->referenceMatrix) Matrix();
    this->mesh = 0;
    this->canvas = canvas;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);

    this->rotation = kDir;
    this->lodPtrs = (V4){0.0f, 0.0f, 0.0f, 0.0f};

    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->lodDist() = 0;
    this->meshId = 0xffffffff;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->merger = 0;
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
    __aeabi_memcpy(src, (void *)loc, 0x3c);
    _ae_MatrixSetTranslation(buf, *(float *)(src + 0x0c) + z, *(float *)(src + 0x1c),
                             *(float *)(src + 0x2c) + y);
    _ae_TransformSetLocal((uint32_t)(uintptr_t)this->canvas, this->transform, buf);
    (void)x;
}

// AEGeometry::AEGeometry(unsigned short mesh, PaintCanvas* canvas, bool flag)
__attribute__((visibility("hidden"))) static const V4 kDir2 = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
{
    this->cameraDelta = (V4){0, 0, 0, 0};
    new (&this->referenceMatrix) Matrix();
    this->canvas = canvas;
    this->mesh = mesh;
    this->transform = 0;
    this->baseTransform = 0;
    AEGeomCanvas::TransformCreate(canvas, &this->baseTransform);
    _ae_MeshCreate(canvas, mesh, &this->meshId, flag);
    _ae_TransformAddMeshId((uint32_t)(uintptr_t)canvas, this->baseTransform, this->meshId);

    this->rotation = kDir2;
    this->lodPtrs = (V4){0, 0, 0, 0};
    this->scalingY = 1.0f;
    this->scalingZ = 1.0f;
    this->visibility = 0x101;
    this->rotationOrder = 0;
    this->lodDist() = 0;
    this->meshHandle = 0xffffffff;
    this->altTransform = 0xffffffff;
    this->currentLod = 0;
    this->transform = this->baseTransform;
    this->parentTransform = 0xffffffff;
    this->childTransform = 0xffffffff;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)canvas, 0);
    this->referenceMatrix = *(Matrix *)loc;
    this->merger = 0;
}

void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetRotation(buf, loc, this->rotation.a, this->rotation.b, this->rotation.c,
                          this->rotationOrder);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    this->rotation.d = x;   // scaling.x at 0x3c
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

// AEGeometry::updateLod(const Vector& camPos, float screenScale)
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n);
extern "C" unsigned long long __aeabi_f2ulz_(float f);
extern "C" float __aeabi_ul2f_(unsigned long long v);
extern "C" uint32_t Transform_GetTransform(uint32_t tf);

namespace AbyssEngine { namespace AEMath {
Vector MatrixGetPosition(const Matrix &matrix);
} }
static inline Vector AEMath_MatrixGetPosition_ret(const void *m) {
    return AbyssEngine::AEMath::MatrixGetPosition(*(const AbyssEngine::AEMath::Matrix *)m);
}
static inline void _ae_matrix_assign(void *dst, const void *src) {
    *(AbyssEngine::AEMath::Matrix *)dst = *(const AbyssEngine::AEMath::Matrix *)src;
}
static inline void _ae_vector_assign(void *dst, const void *src) {
    *(AbyssEngine::AEMath::Vector *)dst = *(const AbyssEngine::AEMath::Vector *)src;
}

void AEGeometry::updateLod(const Vector &camPos, float screenScale)
{
    // copy low byte of field_0x48 from its high byte (0x48 = 0x49)
    this->visibility = (this->visibility & 0xff00) | (this->visibility >> 8);

    char matrixCopy[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    __aeabi_memcpy_b(matrixCopy, (void *)loc, 0x3c);

    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    Vector pos = AEMath_MatrixGetPosition_ret((void *)loc);
    Vector delta = AbyssEngine::AEMath::operator-(camPos, pos);
    _ae_vector_assign(&this->cameraDelta, &delta);

    float dx = this->cameraDelta.a, dy = this->cameraDelta.b, dz = this->cameraDelta.c;
    unsigned long long distSq = __aeabi_f2ulz_(dy * dy + dx * dx + dz * dz);
    this->distSq = distSq;

    unsigned long long lastVis = this->lastVisibleDistSq;
    bool visible;
    if (lastVis == 0) {
        visible = true;            // no clamp configured -> always considered visible
    } else {
        visible = distSq < lastVis;
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

    int count = this->lodCount;
    int level = count;
    char *distances = (char *)this->lodDist();   // per-level squared distance (u64 each)

    // Walk levels high->low; stop at the first whose scaled threshold is met.
    while (level >= 1) {
        int idx = level - 1;
        float thresh = __aeabi_ul2f_(*(unsigned long long *)(distances + idx * 8));
        float d = __aeabi_ul2f_(this->distSq);
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }
        // This level is the one to use.
        void **lodMats = (void **)this->lodTf();
        void *lodMat = lodMats[idx];
        if (lodMat != (void *)(uintptr_t)this->transform) {
            AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->canvas,
                                            this->transform, (Matrix *)lodMat);
            this->transform = (uint32_t)(uintptr_t)((void **)this->lodTf())[idx];
            uint32_t t = Transform_GetTransform((uint32_t)(uintptr_t)this->canvas);
            ((AbyssEngine::Transform *)(uintptr_t)t)->SetCurrentAnimationTime(0);
            t = Transform_GetTransform((uint32_t)(uintptr_t)this->canvas);
            ((AbyssEngine::Transform *)(uintptr_t)t)->SetCurrentAnimationTime(0);
            this->currentLod = level;
            _ae_matrix_assign(&this->referenceMatrix, matrixCopy);
            void *lmm = this->merger;
            if (lmm != 0)
                ((LodMeshMerger *)(lmm))->setLod(this->mergerIndex, (signed char)level);
        }
        return;
    }

    // No level matched: fall back to the base mesh.
    AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->canvas,
                                    this->transform, (Matrix *)(uintptr_t)this->baseTransform);
    this->currentLod = 0;
    this->transform = this->baseTransform;
    void *lmm = this->merger;
    if (lmm != 0)
        ((LodMeshMerger *)(lmm))->setLod(this->mergerIndex, 0);
}

// AEGeometry::setDirection(const Vector& dir, const Vector& up)
void VectorCross(Vector *out, const Vector *b);                 // 0x6ec8c (out = out x b)

void AEGeometry::setDirection(const Vector &dir, const Vector &up)
{
    char local[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    __aeabi_memcpy_b(local, (void *)loc, 0x3c);

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

    AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->canvas,
                                    this->transform, &m);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->canvas, this->transform);
    _ae_MatrixSetScaling((void *)local, loc, this->rotation.d, this->scalingY, this->scalingZ);
}

void AEGeometry::rotate(const Vector &v)
{
    return rotate(v.x, v.y, v.z);
}

void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = this->lodCount;
    if (count > 0) {
        this->lodChildTf() = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->canvas,
                                            ((uint32_t *)this->lodTf())[i], param);
            count = this->lodCount;
        }
    }
}

// Single-argument convenience overload: orient the forward axis along 'dir'
// while keeping the geometry's current up axis as the reference up vector.
// (The two-vector form does the actual basis re-orthonormalisation.)
void AEGeometry::setDirection(const Vector &dir)
{
    Vector up = this->getUpVector();
    setDirection(dir, up);
}

// Re-commit the geometry's current world position. The owning object (e.g. a
// PlayerWormHole) updates its own position fields and then notifies the
// geometry so its transform stays in sync; we read the position back out of the
// local transform and re-apply it through the normal setPosition path.
void AEGeometry::positionChanged()
{
    Vector pos = this->getPosition();
    setPosition(pos.x, pos.y, pos.z);
}
