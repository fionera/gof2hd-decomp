#include "AEGeometry.h"


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
extern "C" void _ae_TransformAddMeshId(PaintCanvas *c, uint32_t tf, uint32_t meshId);
extern "C" void _ae_getDirection(void *self, Vector *out);
extern "C" void _ae_getPosition(void *self, Vector *out);
extern "C" void _ae_setPosition_v(void *self, Vector *v);

// ---- getPosition_a4592.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getPosition() -> TransformGetLocal(canvas,tf); return MatrixGetPosition(loc)
struct AEGeometry { Vector getPosition(); };
Vector AEGeometry::getPosition()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetPosition(loc);
}

// ---- _AEGeometry_a44f8.cpp ----
// AEGeometry::~AEGeometry() -> delete[] five LOD arrays in order 0x54,0x5c,0x58,0x60,0x64
struct AEGeometry {
    ~AEGeometry();
};
AEGeometry::~AEGeometry()
{
    if (pp(this, 0x54)) operator delete[](pp(this, 0x54));
    pp(this, 0x54) = 0;
    if (pp(this, 0x5c)) operator delete[](pp(this, 0x5c));
    pp(this, 0x5c) = 0;
    if (pp(this, 0x58)) operator delete[](pp(this, 0x58));
    pp(this, 0x58) = 0;
    if (pp(this, 0x60)) operator delete[](pp(this, 0x60));
    pp(this, 0x60) = 0;
    if (pp(this, 0x64)) operator delete[](pp(this, 0x64));
    pp(this, 0x64) = 0;
}

// ---- hasLod_a4f94.cpp ----
// AEGeometry::hasLod()  ->  ldr; cmp; it ne; movne #1; bx lr
struct AEGeometry { bool hasLod(); };
bool AEGeometry::hasLod() { return i32(this, 0x54) != 0; }

// ---- isVisible_a4a4a.cpp ----
// AEGeometry::isVisible()  ->  ldrb.w r0,[r0,#0x48] ; bx lr
struct AEGeometry { uint8_t isVisible(); };
uint8_t AEGeometry::isVisible() { return u8(this, 0x48); }

// ---- setVisible_a4a40.cpp ----
// AEGeometry::setVisible(bool)  ->  strb.w r1,[r0,#0x49] ; strb.w r1,[r0,#0x48] ; bx lr
struct AEGeometry { void setVisible(bool v); };
void AEGeometry::setVisible(bool v)
{
    u8(this, 0x48) = v;
    u8(this, 0x49) = v;
}

// ---- DEBUG_setMeshMergerIndex_a4f9e.cpp ----
// AEGeometry::DEBUG_setMeshMergerIndex(int, LodMeshMerger*) -> strd r1,r2,[r0,#0x0] ; bx lr
struct LodMeshMerger;
struct AEGeometry { void DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b); };
void AEGeometry::DEBUG_setMeshMergerIndex(int a, LodMeshMerger *b)
{
    i32(this, 0) = a;
    pp(this, 4) = (void *)b;
}

// ---- addChild_a453a.cpp ----
using namespace AbyssEngine;

// AEGeometry::addChild(unsigned int child)
struct AEGeometry { void addChild(uint32_t child); };
void AEGeometry::addChild(uint32_t child)
{
    PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c), u32(this, 0xc), child);
    uint32_t old = u32(this, 0x14);
    if (old != 0xffffffffu)
        u32(this, 0x10) = old;
    u32(this, 0x14) = child;
}

// ---- render_a4a50.cpp ----
// AEGeometry::render() -> if (visible) tail-call PaintCanvas render(canvas, tf, 0)

struct AEGeometry { void render(); };
void AEGeometry::render()
{
    if (u8(this, 0x48) == 0)
        return;
    return _ae_geom_render(u32(this, 0x2c), u32(this, 0xc), 0);
}

// ---- translate_a4920.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::translate(Vector const&) -> tail-call translate(v.x, v.y, v.z)
struct AEGeometry {
    void translate(float, float, float);
    void translate(const Vector &v);
};
void AEGeometry::translate(const Vector &v)
{
    return translate(v.x, v.y, v.z);
}

// ---- setScaling_a47f8.cpp ----
// AEGeometry::setScaling(float) -> tail-call setScaling(s, s, s)
struct AEGeometry {
    void setScaling(float, float, float);
    void setScaling(float s);
};
void AEGeometry::setScaling(float s)
{
    return setScaling(s, s, s);
}

// ---- getRotation_a45fa.cpp ----
// AEGeometry::getRotation()  ->  returns Vector(this+0x30..0x38) by value (sret in r0, this in r1)
struct AEGeometry { AbyssEngine::AEMath::Vector getRotation(); };
AbyssEngine::AEMath::Vector AEGeometry::getRotation()
{
    AbyssEngine::AEMath::Vector v;
    v.x = f32(this, 0x30);
    v.y = f32(this, 0x34);
    v.z = f32(this, 0x38);
    return v;
}

// ---- setScaling_a4874.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::setScaling(Vector const&) -> tail-call setScaling(v.x, v.y, v.z)
struct AEGeometry {
    void setScaling(float, float, float);
    void setScaling(const Vector &v);
};
void AEGeometry::setScaling(const Vector &v)
{
    return setScaling(v.x, v.y, v.z);
}

// ---- setLodMeshesWithMeshIds_a4b88.cpp ----
using namespace AbyssEngine;

// AEGeometry::setLodMeshesWithMeshIds(unsigned short* meshes, unsigned int* meshIds, int* dists, int count)

struct AEGeometry {
    void setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count);
};
void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count)
{
    uint16_t *meshArr = new uint16_t[count];
    pp(this, 0x5c) = meshArr;
    long long *distArr = new long long[count];
    pp(this, 0x64) = distArr;
    i32(this, 0x50) = count;
    uint32_t *tfArr = new uint32_t[count];
    pp(this, 0x54) = tfArr;
    for (int i = 0; i < count; i++) {
        ((uint16_t *)u32(this, 0x5c))[i] = meshes[i];
        long long *d = (long long *)u32(this, 0x64);
        d[i] = (long long)dists[i];
        PaintCanvas::TransformCreate((PaintCanvas *)u32(this, 0x2c),
                                     &((uint32_t *)u32(this, 0x54))[i]);
        _ae_TransformAddMeshId(u32(this, 0x2c), ((uint32_t *)u32(this, 0x54))[i], meshIds[i]);
        long long v = ((long long *)u32(this, 0x64))[i];
        ((long long *)u32(this, 0x64))[i] = v * v;
        if (u32(this, 0x14) != 0xffffffffu)
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
    }
}

// ---- setLodLastVisibleDistance_a4a86.cpp ----
// AEGeometry::setLodLastVisibleDistance(unsigned long long)
//   *(u64*)(this+0x70) = d * d   (umull + 2x mla for the high word)
struct AEGeometry { void setLodLastVisibleDistance(uint64_t d); };
void AEGeometry::setLodLastVisibleDistance(uint64_t d)
{
    *(uint64_t *)((char *)this + 0x70) = d * d;
}

// ---- getRightVector_a45e0.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getRightVector() -> TransformGetLocal(canvas,tf); return MatrixGetRight(loc)
struct AEGeometry { Vector getRightVector(); };
Vector AEGeometry::getRightVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetRight(loc);
}

// ---- getUpVector_a45c6.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getUpVector() -> TransformGetLocal(canvas,tf); return MatrixGetUp(loc)
struct AEGeometry { Vector getUpVector(); };
Vector AEGeometry::getUpVector()
{
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    return MatrixGetUp(loc);
}

// ---- getMatrix_a4616.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getMatrix() -> return *(Matrix*)TransformGetLocal(canvas, tf)  (returns a reference)
struct AEGeometry { Matrix &getMatrix(); };
Matrix &AEGeometry::getMatrix()
{
    return *(Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
}

// ---- getScaling_a4608.cpp ----
// AEGeometry::getScaling()  ->  Vector(this+0x3c..0x44) by value
struct AEGeometry { AbyssEngine::AEMath::Vector getScaling(); };
AbyssEngine::AEMath::Vector AEGeometry::getScaling()
{
    AbyssEngine::AEMath::Vector v;
    v.x = f32(this, 0x3c);
    v.y = f32(this, 0x40);
    v.z = f32(this, 0x44);
    return v;
}

// ---- updateReferenceMatrix_a4a6a.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::updateReferenceMatrix() -> (this+0x84) = TransformGetLocal(canvas, tf)
struct AEGeometry { void updateReferenceMatrix(); };
void AEGeometry::updateReferenceMatrix()
{
    Matrix *loc = (Matrix *)PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    Matrix *ref = (Matrix *)((char *)this + 0x84);
    *ref = *loc;
}

// ---- setLodMeshes_a4c84.cpp ----
using namespace AbyssEngine;

// AEGeometry::setLodMeshes(unsigned short* meshes, int* dists, int count)

struct AEGeometry { void setLodMeshes(uint16_t *meshes, int *dists, int count); };
void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count)
{
    uint16_t *meshArr = new uint16_t[count];
    pp(this, 0x5c) = meshArr;
    long long *distArr = new long long[count];
    pp(this, 0x64) = distArr;
    i32(this, 0x50) = count;
    uint32_t *tfArr = new uint32_t[count];
    pp(this, 0x54) = tfArr;
    for (int i = 0; i < count; i++) {
        ((uint16_t *)u32(this, 0x5c))[i] = meshes[i];
        long long *d = (long long *)u32(this, 0x64);
        d[i] = (long long)dists[i];
        PaintCanvas::TransformCreate((PaintCanvas *)u32(this, 0x2c),
                                     &((uint32_t *)u32(this, 0x54))[i]);
        _ae_TransformAddMesh(u32(this, 0x2c), ((uint32_t *)u32(this, 0x54))[i],
                             (uint16_t)meshes[i], 0);
        long long v = ((long long *)u32(this, 0x64))[i];
        ((long long *)u32(this, 0x64))[i] = v * v;
        if (u32(this, 0x14) != 0xffffffffu)
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
    }
}

// ---- getParentPosition_a4572.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::getParentPosition() -> tf = this->0x24; if (tf==-1) tf = this->0xc; then matrix pos
struct AEGeometry { Vector getParentPosition(); };
Vector AEGeometry::getParentPosition()
{
    uint32_t canvas = u32(this, 0x2c);
    uint32_t tf = u32(this, 0x24);
    if (tf + 1 == 0)
        tf = u32(this, 0xc);
    Matrix &loc = *(Matrix *)PaintCanvas::TransformGetLocal(canvas, tf);
    return MatrixGetPosition(loc);
}

// ---- setRotation_a4784.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setRotation(float, float, float)

struct AEGeometry { void setRotation(float x, float y, float z); };
void AEGeometry::setRotation(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetRotation(buf, loc, x, y, z, i32(this, 0x4c));
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling(buf, loc, f32(this, 0x3c), f32(this, 0x40), f32(this, 0x44));
    f32(this, 0x30) = x;
    f32(this, 0x34) = y;
    f32(this, 0x38) = z;
}

// ---- setRotation_a4778.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::setRotation(Vector const&) -> tail-call setRotation(v.x, v.y, v.z)
struct AEGeometry {
    void setRotation(float, float, float);
    void setRotation(const Vector &v);
};
void AEGeometry::setRotation(const Vector &v)
{
    return setRotation(v.x, v.y, v.z);
}

// ---- setPosition_a4632.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::setPosition(Vector const&) -> tail-call ext(this, v.x, v.y, v.z)

struct AEGeometry { void setPosition(const Vector &v); };
void AEGeometry::setPosition(const Vector &v)
{
    return _ae_setPosition3(this, v.x, v.y, v.z);
}

// ---- setLodChildMeshes_a4a98.cpp ----
using namespace AbyssEngine;

// AEGeometry::setLodChildMeshes(unsigned short*)

struct AEGeometry { void setLodChildMeshes(uint16_t *meshes); };
void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = i32(this, 0x50);
    if (count > 0) {
        uint16_t *m = new uint16_t[count];
        pp(this, 0x60) = m;
        uint32_t *tfs = new uint32_t[count];
        pp(this, 0x58) = tfs;
        for (int i = 0; i < count; i++) {
            ((uint16_t *)u32(this, 0x60))[i] = meshes[i];
            PaintCanvas::TransformCreate((PaintCanvas *)u32(this, 0x2c),
                                         &((uint32_t *)u32(this, 0x58))[i]);
            _ae_TransformAddMesh(u32(this, 0x2c), ((uint32_t *)u32(this, 0x58))[i],
                                 (uint16_t)meshes[i], 0);
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i],
                                           ((uint32_t *)u32(this, 0x58))[i]);
            _ae_TransformRemoveChild((PaintCanvas *)u32(this, 0x2c),
                                     ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
            count = i32(this, 0x50);
        }
    }
}

// ---- rotate_a4880.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::rotate(float, float, float)

struct AEGeometry { void rotate(float x, float y, float z); };
void AEGeometry::rotate(float x, float y, float z)
{
    char buf[60];
    f32(this, 0x30) = f32(this, 0x30) + x;
    f32(this, 0x34) = f32(this, 0x34) + y;
    f32(this, 0x38) = f32(this, 0x38) + z;
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetRotation(buf, loc, f32(this, 0x30), f32(this, 0x34), f32(this, 0x38),
                          i32(this, 0x4c));
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling(buf, loc, f32(this, 0x3c), f32(this, 0x40), f32(this, 0x44));
}

// ---- AEGeometry_a4468.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::AEGeometry(PaintCanvas*)
struct AEGeometry { AEGeometry(PaintCanvas *canvas); };

struct V4 { float a, b, c, d; };
__attribute__((visibility("hidden"))) static const V4 kDir = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
    u32(this, 0x80) = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    *(uint16_t *)((char *)this + 8) = 0;
    pp(this, 0x2c) = canvas;
    PaintCanvas::TransformCreate(canvas, &u32(this, 0x18));

    *(V4 *)((char *)this + 0x30) = kDir;
    *(V4 *)((char *)this + 0x54) = (V4){0.0f, 0.0f, 0.0f, 0.0f};

    f32(this, 0x40) = 1.0f;
    f32(this, 0x44) = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    u32(this, 0x4c) = 0;
    u32(this, 0x64) = 0;
    u32(this, 0x1c) = 0xffffffff;
    u32(this, 0x20) = 0xffffffff;
    u32(this, 0x24) = 0xffffffff;
    u32(this, 0x28) = 0;
    u32(this, 4) = 0;
    u32(this, 0xc) = u32(this, 0x18);
    u32(this, 0x10) = 0xffffffff;
    u32(this, 0x14) = 0xffffffff;
}

// ---- setMesh_a4558.cpp ----
// AEGeometry::setMesh(unsigned short)
//   r3=this; r1=*(this+0xc) [pre-indexed, r3->this+0xc]; r0=*(this+0x2c)
//   if (*(this+0xc)!=0)  ext1(canvas, *(this+0xc), mesh, 0)
//   else                 ext2(canvas, mesh, &this+0xc)

struct AEGeometry { void setMesh(uint16_t mesh); };
void AEGeometry::setMesh(uint16_t mesh)
{
    uint32_t *tfp = &u32(this, 0xc);
    uint32_t canvas = tfp[(0x2c - 0xc) / 4];
    if (*tfp != 0)
        return _ae_setmesh_a(canvas, *tfp, mesh, 0);
    return _ae_setmesh_b(canvas, mesh, tfp);
}

// ---- translate_a492c.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::translate(float, float, float)

struct AEGeometry { void translate(float x, float y, float z); };
void AEGeometry::translate(float x, float y, float z)
{
    char buf[60];
    char src[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    __aeabi_memcpy(src, (void *)loc, 0x3c);
    _ae_MatrixSetTranslation(buf, *(float *)(src + 0x0c) + z, *(float *)(src + 0x1c),
                             *(float *)(src + 0x2c) + y);
    _ae_TransformSetLocal(u32(this, 0x2c), u32(this, 0xc), buf);
    (void)x;
}

// ---- AEGeometry_a4398.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::AEGeometry(unsigned short mesh, PaintCanvas* canvas, bool flag)

struct AEGeometry { AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag); };

struct V4b { float a, b, c, d; };
__attribute__((visibility("hidden"))) static const V4b kDir2 = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
{
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
    u32(this, 0x80) = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    pp(this, 0x2c) = canvas;
    *(uint16_t *)((char *)this + 8) = mesh;
    u32(this, 0xc) = 0;
    u32(this, 0x18) = 0;
    PaintCanvas::TransformCreate(canvas, &u32(this, 0x18));
    _ae_MeshCreate(canvas, mesh, &u32(this, 0x1c), flag);
    _ae_TransformAddMeshId(canvas, u32(this, 0x18), u32(this, 0x1c));

    *(V4b *)((char *)this + 0x30) = kDir2;
    *(V4b *)((char *)this + 0x54) = (V4b){0, 0, 0, 0};
    f32(this, 0x40) = 1.0f;
    f32(this, 0x44) = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    u32(this, 0x4c) = 0;
    u32(this, 0x64) = 0;
    u32(this, 0x20) = 0xffffffff;
    u32(this, 0x24) = 0xffffffff;
    u32(this, 0x28) = 0;
    u32(this, 0xc) = u32(this, 0x18);
    u32(this, 0x10) = 0xffffffff;
    u32(this, 0x14) = 0xffffffff;
    uint32_t loc = PaintCanvas::TransformGetLocal((uint32_t)canvas, 0);
    *(Matrix *)((char *)this + 0x84) = *(Matrix *)loc;
    u32(this, 4) = 0;
}

// ---- setScaling_a4800.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setScaling(float, float, float)

struct AEGeometry { void setScaling(float x, float y, float z); };
void AEGeometry::setScaling(float x, float y, float z)
{
    char buf[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetRotation(buf, loc, f32(this, 0x30), f32(this, 0x34), f32(this, 0x38),
                          i32(this, 0x4c));
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling(buf, loc, x, y, z);
    f32(this, 0x3c) = x;
    f32(this, 0x40) = y;
    f32(this, 0x44) = z;
}

// ---- moveForward_a49b8.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::moveForward(float dist)
//   getDirection(&dir); VectorNormalize(&n,&dir); getPosition(&pos);
//   pos += n*dist; setPosition(&pos)

struct AEGeometry { void moveForward(float dist); };
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
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::updateLod(const Vector& camPos, float screenScale)
//   Recomputes the camera-distance of this object, decides whether it is within
//   its last-visible range, and -- if so -- selects the appropriate LOD level by
//   comparing the squared distance against each level's threshold (scaled by a
//   detail factor derived from the screen scale), swapping in that level's local
//   matrix and notifying the LodMeshMerger.
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n);     // 0x6f1e4
extern "C" Vector AEMath_MatrixGetPosition_ret(const void *m);                 // 0x6f16c
extern "C" unsigned long long __aeabi_f2ulz_(float f);                          // 0x73078
extern "C" float __aeabi_ul2f_(unsigned long long v);                          // 0x73084
extern "C" uint32_t Transform_GetTransform(uint32_t tf);                       // 0x72088
extern "C" void Transform_SetCurrentAnimationTime(uint32_t t, uint32_t a, uint32_t b); // 0x73090
extern "C" void LodMeshMerger_setLod(void *lmm, uint32_t idx, int lod);        // 0x7309c
extern "C" void _ae_matrix_assign(void *dst, const void *src);                 // 0x6f148
extern "C" void _ae_vector_assign(void *dst, const void *src);                 // 0x6eb3c

struct AEGeometry { void updateLod(const Vector &camPos, float screenScale); };

void AEGeometry::updateLod(const Vector &camPos, float screenScale)
{
    u8(this, 0x48) = u8(this, 0x49);

    char matrixCopy[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    __aeabi_memcpy_b(matrixCopy, (void *)loc, 0x3c);

    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    Vector pos = AEMath_MatrixGetPosition_ret((void *)loc);
    Vector delta = AbyssEngine::AEMath::operator-(camPos, pos);
    _ae_vector_assign((char *)this + 0x78, &delta);

    float dx = f32(this, 0x78), dy = f32(this, 0x7c), dz = f32(this, 0x80);
    unsigned long long distSq = __aeabi_f2ulz_(dy * dy + dx * dx + dz * dz);
    *(unsigned long long *)((char *)this + 0x68) = distSq;

    unsigned long long lastVis = *(unsigned long long *)((char *)this + 0x70);
    bool visible;
    if (lastVis == 0) {
        visible = true;            // no clamp configured -> always considered visible
    } else {
        visible = distSq < lastVis;
        u8(this, 0x48) = visible ? 1 : 0;
    }

    if (!visible) {
        i32(this, 0x28) = -1;
        return;
    }

    Transform_GetTransform(u32(this, 0x2c));

    // Detail factor: tighter LOD thresholds when the object is small on screen.
    float factor = (screenScale <= 0.0625f) ? 0.75f : 1.0f;
    float detail = (0.03125f < screenScale) ? factor : 0.5f;

    int count = i32(this, 0x50);
    int level = count;
    char *distances = (char *)pp(this, 0x64);   // per-level squared distance (u64 each)

    // Walk levels high->low; stop at the first whose scaled threshold is met.
    while (level >= 1) {
        int idx = level - 1;
        float thresh = __aeabi_ul2f_(*(unsigned long long *)(distances + idx * 8));
        float d = __aeabi_ul2f_(*(unsigned long long *)((char *)this + 0x68));
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }
        // This level is the one to use.
        void **lodMats = (void **)pp(this, 0x54);
        void *lodMat = lodMats[idx];
        if (lodMat != pp(this, 0xc)) {
            PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)u32(this, 0x2c),
                                           u32(this, 0xc), (Matrix *)lodMat);
            pp(this, 0xc) = ((void **)pp(this, 0x54))[idx];
            uint32_t t = Transform_GetTransform(u32(this, 0x2c));
            Transform_SetCurrentAnimationTime(t, 0, 0);
            t = Transform_GetTransform(u32(this, 0x2c));
            Transform_SetCurrentAnimationTime(t, 0, 0);
            i32(this, 0x28) = level;
            _ae_matrix_assign((char *)this + 0x84, matrixCopy);
            void *lmm = pp(this, 0x4);
            if (lmm != 0)
                LodMeshMerger_setLod(lmm, u32(this, 0x0), (signed char)level);
        }
        return;
    }

    // No level matched: fall back to the base mesh.
    PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)u32(this, 0x2c),
                                   u32(this, 0xc), (Matrix *)pp(this, 0x18));
    i32(this, 0x28) = 0;
    pp(this, 0xc) = pp(this, 0x18);
    void *lmm = pp(this, 0x4);
    if (lmm != 0)
        LodMeshMerger_setLod(lmm, u32(this, 0x0), 0);
}

// ---- setDirection_a4688.cpp ----
using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setDirection(const Vector& dir, const Vector& up)
//   Builds an orthonormal orientation from the requested forward direction and
//   up hint (Gram-Schmidt), writes it as the transform's local matrix while
//   preserving the existing translation, then re-applies the stored scaling.
extern "C" void VectorCross(Vector *out, const Vector *b);                 // 0x6ec8c (out = out x b)
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n); // 0x6f1e4

struct AEGeometry { void setDirection(const Vector &dir, const Vector &up); };

void AEGeometry::setDirection(const Vector &dir, const Vector &up)
{
    char local[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
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

    PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)u32(this, 0x2c),
                                   u32(this, 0xc), &m);
    loc = PaintCanvas::TransformGetLocal(u32(this, 0x2c), u32(this, 0xc));
    _ae_MatrixSetScaling((void *)local, loc, f32(this, 0x3c), f32(this, 0x40), f32(this, 0x44));
}

// ---- rotate_a4914.cpp ----
using namespace AbyssEngine::AEMath;

// AEGeometry::rotate(Vector const&) -> tail-call rotate(v.x, v.y, v.z)
struct AEGeometry {
    void rotate(float, float, float);
    void rotate(const Vector &v);
};
void AEGeometry::rotate(const Vector &v)
{
    return rotate(v.x, v.y, v.z);
}

// ---- setLodChildTransform_a4b3c.cpp ----
using namespace AbyssEngine;

// AEGeometry::setLodChildTransform(unsigned int)
struct AEGeometry { void setLodChildTransform(uint32_t param); };
void AEGeometry::setLodChildTransform(uint32_t param)
{
    int count = i32(this, 0x50);
    if (count > 0) {
        uint32_t *arr = new uint32_t[count];
        pp(this, 0x58) = arr;
        for (int i = 0; i < count; i++) {
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], param);
            count = i32(this, 0x50);
        }
    }
}
