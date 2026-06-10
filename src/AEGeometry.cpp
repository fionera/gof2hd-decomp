#include "gof2/AEGeometry.h"

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
extern "C" void _ae_setPosition_v(void *self, Vector *v);

// ---- getPosition_a4592.cpp ----
// AEGeometry::getPosition() -> TransformGetLocal(canvas,tf); return MatrixGetPosition(loc)
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    return MatrixGetPosition(loc);
}

// ---- _AEGeometry_a44f8.cpp ----
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

// ---- hasLod_a4f94.cpp ----
bool AEGeometry::hasLod() { return this->lodTf() != 0; }

// ---- isVisible_a4a4a.cpp ----
uint8_t AEGeometry::isVisible() { return (uint8_t)this->field_0x48; }

// ---- setVisible_a4a40.cpp ----
void AEGeometry::setVisible(bool v)
{
    // low byte (0x48) and next byte (0x49) both set; field_0x48 is the u16 pair.
    this->field_0x48 = v ? 0x0101 : 0;
}

// ---- DEBUG_setMeshMergerIndex_a4f9e.cpp ----
void AEGeometry::DEBUG_setMeshMergerIndex(int a, void *b)
{
    this->field_0x0 = (uint32_t)a;
    this->field_0x4 = b;
}

// ---- addChild_a453a.cpp ----
void AEGeometry::addChild(uint32_t child)
{
    AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c, this->field_0xc, child);
    uint32_t old = this->field_0x14;
    if (old != 0xffffffffu)
        this->field_0x10 = old;
    this->field_0x14 = child;
}

// ---- render_a4a50.cpp ----
void AEGeometry::render()
{
    if ((uint8_t)this->field_0x48 == 0)
        return;
    return _ae_geom_render((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc, 0);
}

// ---- translate_a4920.cpp ----
void AEGeometry::translate(const Vector &v)
{
    return translate(v.x, v.y, v.z);
}

// ---- setScaling_a47f8.cpp ----
void AEGeometry::setScaling(float s)
{
    return setScaling(s, s, s);
}

// ---- getRotation_a45fa.cpp ----
Vector AEGeometry::getRotation()
{
    Vector v;
    v.x = this->field_0x30.a;
    v.y = this->field_0x30.b;
    v.z = this->field_0x30.c;
    return v;
}

// ---- setScaling_a4874.cpp ----
void AEGeometry::setScaling(const Vector &v)
{
    return setScaling(v.x, v.y, v.z);
}

// ---- setLodMeshesWithMeshIds_a4b88.cpp ----
void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count)
{
    this->lodMesh() = new uint16_t[count];
    this->lodDist() = new long long[count];
    this->field_0x50 = count;
    this->lodTf() = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        ((uint16_t *)this->lodMesh())[i] = meshes[i];
        long long *d = (long long *)this->lodDist();
        d[i] = (long long)dists[i];
        AEGeomCanvas::TransformCreate((PaintCanvas *)this->field_0x2c,
                                      &((uint32_t *)this->lodTf())[i]);
        _ae_TransformAddMeshId((uint32_t)(uintptr_t)this->field_0x2c, ((uint32_t *)this->lodTf())[i], meshIds[i]);
        long long v = ((long long *)this->lodDist())[i];
        ((long long *)this->lodDist())[i] = v * v;
        if (this->field_0x14 != 0xffffffffu)
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c,
                                            ((uint32_t *)this->lodTf())[i], this->field_0x14);
    }
}

// ---- setLodLastVisibleDistance_a4a86.cpp ----
void AEGeometry::setLodLastVisibleDistance(uint64_t d)
{
    this->field_0x70 = d * d;
}

// ---- getRightVector_a45e0.cpp ----
Vector AEGeometry::getRightVector()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    return MatrixGetRight(loc);
}

// ---- getUpVector_a45c6.cpp ----
Vector AEGeometry::getUpVector()
{
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    return MatrixGetUp(loc);
}

// ---- getMatrix_a4616.cpp ----
Matrix &AEGeometry::getMatrix()
{
    return *(Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
}

// ---- getScaling_a4608.cpp ----
Vector AEGeometry::getScaling()
{
    Vector v;
    v.x = this->field_0x30.d;   // scaling.x aliases 0x3c
    v.y = this->field_0x40;
    v.z = this->field_0x44;
    return v;
}

// ---- updateReferenceMatrix_a4a6a.cpp ----
void AEGeometry::updateReferenceMatrix()
{
    Matrix *loc = (Matrix *)AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    this->field_0x84 = *loc;
}

// ---- setLodMeshes_a4c84.cpp ----
void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count)
{
    this->lodMesh() = new uint16_t[count];
    this->lodDist() = new long long[count];
    this->field_0x50 = count;
    this->lodTf() = new uint32_t[count];
    for (int i = 0; i < count; i++) {
        ((uint16_t *)this->lodMesh())[i] = meshes[i];
        long long *d = (long long *)this->lodDist();
        d[i] = (long long)dists[i];
        AEGeomCanvas::TransformCreate((PaintCanvas *)this->field_0x2c,
                                      &((uint32_t *)this->lodTf())[i]);
        _ae_TransformAddMesh((uint32_t)(uintptr_t)this->field_0x2c, ((uint32_t *)this->lodTf())[i],
                             (uint16_t)meshes[i], 0);
        long long v = ((long long *)this->lodDist())[i];
        ((long long *)this->lodDist())[i] = v * v;
        if (this->field_0x14 != 0xffffffffu)
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c,
                                            ((uint32_t *)this->lodTf())[i], this->field_0x14);
    }
}

// ---- getParentPosition_a4572.cpp ----
Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = (uint32_t)(uintptr_t)this->field_0x2c;
    uint32_t tf = this->field_0x24;
    if (tf + 1 == 0)
        tf = this->field_0xc;
    Matrix &loc = *(Matrix *)AEGeomCanvas::TransformGetLocal(canvas, tf);
    return MatrixGetPosition(loc);
}

// ---- setRotation_a4784.cpp ----
void AEGeometry::setRotation(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetRotation(buf, loc, x, y, z, this->field_0x4c);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetScaling(buf, loc, this->field_0x30.d, this->field_0x40, this->field_0x44);
    this->field_0x30.a = x;
    this->field_0x30.b = y;
    this->field_0x30.c = z;
}

// ---- setRotation_a4778.cpp ----
void AEGeometry::setRotation(const Vector &v)
{
    return setRotation(v.x, v.y, v.z);
}

// ---- setPosition_a4632.cpp ----
void AEGeometry::setPosition(const Vector &v)
{
    return _ae_setPosition3(this, v.x, v.y, v.z);
}

// ---- setLodChildMeshes_a4a98.cpp ----
void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = this->field_0x50;
    if (count > 0) {
        this->lodChildMesh() = new uint16_t[count];
        this->lodChildTf() = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            ((uint16_t *)this->lodChildMesh())[i] = meshes[i];
            AEGeomCanvas::TransformCreate((PaintCanvas *)this->field_0x2c,
                                          &((uint32_t *)this->lodChildTf())[i]);
            _ae_TransformAddMesh((uint32_t)(uintptr_t)this->field_0x2c, ((uint32_t *)this->lodChildTf())[i],
                                 (uint16_t)meshes[i], 0);
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c,
                                            ((uint32_t *)this->lodTf())[i],
                                            ((uint32_t *)this->lodChildTf())[i]);
            _ae_TransformRemoveChild((PaintCanvas *)this->field_0x2c,
                                     ((uint32_t *)this->lodTf())[i], this->field_0x14);
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c,
                                            ((uint32_t *)this->lodTf())[i], this->field_0x14);
            count = this->field_0x50;
        }
    }
}

// ---- rotate_a4880.cpp ----
void AEGeometry::rotate(float x, float y, float z)
{
    char buf[60];
    this->field_0x30.a = this->field_0x30.a + x;
    this->field_0x30.b = this->field_0x30.b + y;
    this->field_0x30.c = this->field_0x30.c + z;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetRotation(buf, loc, this->field_0x30.a, this->field_0x30.b, this->field_0x30.c,
                          this->field_0x4c);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetScaling(buf, loc, this->field_0x30.d, this->field_0x40, this->field_0x44);
}

// ---- AEGeometry_a4468.cpp ----
// AEGeometry::AEGeometry(PaintCanvas*)
__attribute__((visibility("hidden"))) static const V4 kDir = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    this->field_0x78 = (V4){0, 0, 0, 0};
    new (&this->field_0x84) Matrix();
    this->field_0x8 = 0;
    this->field_0x2c = canvas;
    AEGeomCanvas::TransformCreate(canvas, &this->field_0x18);

    this->field_0x30 = kDir;
    this->field_0x54 = (V4){0.0f, 0.0f, 0.0f, 0.0f};

    this->field_0x40 = 1.0f;
    this->field_0x44 = 1.0f;
    this->field_0x48 = 0x101;
    this->field_0x4c = 0;
    this->lodDist() = 0;
    this->field_0x1c = 0xffffffff;
    this->field_0x20 = 0xffffffff;
    this->field_0x24 = 0xffffffff;
    this->field_0x28 = 0;
    this->field_0x4 = 0;
    this->field_0xc = this->field_0x18;
    this->field_0x10 = 0xffffffff;
    this->field_0x14 = 0xffffffff;
}

// ---- setMesh_a4558.cpp ----
void AEGeometry::setMesh(uint16_t mesh)
{
    uint32_t canvas = (uint32_t)(uintptr_t)this->field_0x2c;
    if (this->field_0xc != 0)
        return _ae_setmesh_a(canvas, this->field_0xc, mesh, 0);
    return _ae_setmesh_b(canvas, mesh, &this->field_0xc);
}

// ---- translate_a492c.cpp ----
void AEGeometry::translate(float x, float y, float z)
{
    char buf[60];
    char src[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    __aeabi_memcpy(src, (void *)loc, 0x3c);
    _ae_MatrixSetTranslation(buf, *(float *)(src + 0x0c) + z, *(float *)(src + 0x1c),
                             *(float *)(src + 0x2c) + y);
    _ae_TransformSetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc, buf);
    (void)x;
}

// ---- AEGeometry_a4398.cpp ----
// AEGeometry::AEGeometry(unsigned short mesh, PaintCanvas* canvas, bool flag)
__attribute__((visibility("hidden"))) static const V4 kDir2 = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
{
    this->field_0x78 = (V4){0, 0, 0, 0};
    new (&this->field_0x84) Matrix();
    this->field_0x2c = canvas;
    this->field_0x8 = mesh;
    this->field_0xc = 0;
    this->field_0x18 = 0;
    AEGeomCanvas::TransformCreate(canvas, &this->field_0x18);
    _ae_MeshCreate(canvas, mesh, &this->field_0x1c, flag);
    _ae_TransformAddMeshId((uint32_t)(uintptr_t)canvas, this->field_0x18, this->field_0x1c);

    this->field_0x30 = kDir2;
    this->field_0x54 = (V4){0, 0, 0, 0};
    this->field_0x40 = 1.0f;
    this->field_0x44 = 1.0f;
    this->field_0x48 = 0x101;
    this->field_0x4c = 0;
    this->lodDist() = 0;
    this->field_0x20 = 0xffffffff;
    this->field_0x24 = 0xffffffff;
    this->field_0x28 = 0;
    this->field_0xc = this->field_0x18;
    this->field_0x10 = 0xffffffff;
    this->field_0x14 = 0xffffffff;
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)canvas, 0);
    this->field_0x84 = *(Matrix *)loc;
    this->field_0x4 = 0;
}

// ---- setScaling_a4800.cpp ----
void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetRotation(buf, loc, this->field_0x30.a, this->field_0x30.b, this->field_0x30.c,
                          this->field_0x4c);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    this->field_0x30.d = x;   // scaling.x at 0x3c
    this->field_0x40 = y;
    this->field_0x44 = z;
}

// ---- moveForward_a49b8.cpp ----
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
    _ae_setPosition_v(this, &pos);
}

// ---- updateLod_a4d80.cpp ----
// AEGeometry::updateLod(const Vector& camPos, float screenScale)
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n);     // 0x6f1e4
extern "C" Vector AEMath_MatrixGetPosition_ret(const void *m);                 // 0x6f16c
extern "C" unsigned long long __aeabi_f2ulz_(float f);                          // 0x73078
extern "C" float __aeabi_ul2f_(unsigned long long v);                          // 0x73084
extern "C" uint32_t Transform_GetTransform(uint32_t tf);                       // 0x72088
extern "C" void Transform_SetCurrentAnimationTime(uint32_t t, uint32_t a, uint32_t b); // 0x73090
extern "C" void LodMeshMerger_setLod(void *lmm, uint32_t idx, int lod);        // 0x7309c
extern "C" void _ae_matrix_assign(void *dst, const void *src);                 // 0x6f148
extern "C" void _ae_vector_assign(void *dst, const void *src);                 // 0x6eb3c

void AEGeometry::updateLod(const Vector &camPos, float screenScale)
{
    // copy low byte of field_0x48 from its high byte (0x48 = 0x49)
    this->field_0x48 = (this->field_0x48 & 0xff00) | (this->field_0x48 >> 8);

    char matrixCopy[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    __aeabi_memcpy_b(matrixCopy, (void *)loc, 0x3c);

    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    Vector pos = AEMath_MatrixGetPosition_ret((void *)loc);
    Vector delta = AbyssEngine::AEMath::operator-(camPos, pos);
    _ae_vector_assign(&this->field_0x78, &delta);

    float dx = this->field_0x78.a, dy = this->field_0x78.b, dz = this->field_0x78.c;
    unsigned long long distSq = __aeabi_f2ulz_(dy * dy + dx * dx + dz * dz);
    this->field_0x68 = distSq;

    unsigned long long lastVis = this->field_0x70;
    bool visible;
    if (lastVis == 0) {
        visible = true;            // no clamp configured -> always considered visible
    } else {
        visible = distSq < lastVis;
        this->field_0x48 = (this->field_0x48 & 0xff00) | (visible ? 1 : 0);
    }

    if (!visible) {
        this->field_0x28 = -1;
        return;
    }

    Transform_GetTransform((uint32_t)(uintptr_t)this->field_0x2c);

    // Detail factor: tighter LOD thresholds when the object is small on screen.
    float factor = (screenScale <= 0.0625f) ? 0.75f : 1.0f;
    float detail = (0.03125f < screenScale) ? factor : 0.5f;

    int count = this->field_0x50;
    int level = count;
    char *distances = (char *)this->lodDist();   // per-level squared distance (u64 each)

    // Walk levels high->low; stop at the first whose scaled threshold is met.
    while (level >= 1) {
        int idx = level - 1;
        float thresh = __aeabi_ul2f_(*(unsigned long long *)(distances + idx * 8));
        float d = __aeabi_ul2f_(this->field_0x68);
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }
        // This level is the one to use.
        void **lodMats = (void **)this->lodTf();
        void *lodMat = lodMats[idx];
        if (lodMat != (void *)(uintptr_t)this->field_0xc) {
            AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->field_0x2c,
                                            this->field_0xc, (Matrix *)lodMat);
            this->field_0xc = (uint32_t)(uintptr_t)((void **)this->lodTf())[idx];
            uint32_t t = Transform_GetTransform((uint32_t)(uintptr_t)this->field_0x2c);
            Transform_SetCurrentAnimationTime(t, 0, 0);
            t = Transform_GetTransform((uint32_t)(uintptr_t)this->field_0x2c);
            Transform_SetCurrentAnimationTime(t, 0, 0);
            this->field_0x28 = level;
            _ae_matrix_assign(&this->field_0x84, matrixCopy);
            void *lmm = this->field_0x4;
            if (lmm != 0)
                LodMeshMerger_setLod(lmm, this->field_0x0, (signed char)level);
        }
        return;
    }

    // No level matched: fall back to the base mesh.
    AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->field_0x2c,
                                    this->field_0xc, (Matrix *)(uintptr_t)this->field_0x18);
    this->field_0x28 = 0;
    this->field_0xc = this->field_0x18;
    void *lmm = this->field_0x4;
    if (lmm != 0)
        LodMeshMerger_setLod(lmm, this->field_0x0, 0);
}

// ---- setDirection_a4688.cpp ----
// AEGeometry::setDirection(const Vector& dir, const Vector& up)
void VectorCross(Vector *out, const Vector *b);                 // 0x6ec8c (out = out x b)

void AEGeometry::setDirection(const Vector &dir, const Vector &up)
{
    char local[60];
    uint32_t loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
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

    AEGeomCanvas::TransformSetLocal((PaintCanvas *)this->field_0x2c,
                                    this->field_0xc, &m);
    loc = AEGeomCanvas::TransformGetLocal((uint32_t)(uintptr_t)this->field_0x2c, this->field_0xc);
    _ae_MatrixSetScaling((void *)local, loc, this->field_0x30.d, this->field_0x40, this->field_0x44);
}

// ---- rotate_a4914.cpp ----
void AEGeometry::rotate(const Vector &v)
{
    return rotate(v.x, v.y, v.z);
}

// ---- setLodChildTransform_a4b3c.cpp ----
void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = this->field_0x50;
    if (count > 0) {
        this->lodChildTf() = new uint32_t[count];
        for (int i = 0; i < count; i++) {
            AEGeomCanvas::TransformAddChild((PaintCanvas *)this->field_0x2c,
                                            ((uint32_t *)this->lodTf())[i], param);
            count = this->field_0x50;
        }
    }
}
