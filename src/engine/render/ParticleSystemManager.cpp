#include "gof2/engine/render/ParticleSystemManager.h"
#include "gof2/engine/render/IParticleSystem.h"
#include "gof2/engine/render/PaintCanvas.h"

extern "C" void _psm_ArrayReleaseSprites(void *arr);
extern "C" void _psm_ReleaseSpriteSystemResource(void *canvas, unsigned res);
extern "C" void _psm_renderMeshes(void *self);
extern "C" void _psm_renderSpritesExt(void *self);
extern "C" void _psm_construct(void *self);
extern "C" void _psm_releaseSprites(void *self);
extern "C" void _psm_releaseMeshArray(void *arr);
extern "C" void _psm_constructAfterCamera(void *self);
extern "C" void _ips_emitManual(void *sys, float x, float y, float z);
extern "C" void _psm_spriteRender4(void *canvas, unsigned a, unsigned b, unsigned c);
extern "C" void _psm_spriteRender2(void *canvas, unsigned a);
extern "C" void _psm_arraySpriteCtor(void *arr);
extern "C" void _psm_arrayMeshCtor(void *arr);
extern "C" void _ips_enableUpdate(void *sys, bool enable);
extern "C" short _ips_getParticleCount16(void *sys);
extern "C" int _psm_addSpriteSystem(void *self, const void *matrix, unsigned int set, bool flag);
extern "C" void _psm_initSprites(void *self);
extern "C" void _psm_initMesh(void *self);
extern "C" int  _psm_firstUpdate(void *self, int a, int b, int c);
extern "C" void _ips_reset(void *sys);
extern "C" void _psm_meshRender4(void *canvas, unsigned a, unsigned b, unsigned c);
extern "C" void _psm_meshRender2(void *canvas, unsigned a);

// ParticleSystemManager::update(long long)
void ParticleSystemManager::update(long long dt)
{
    int d = (int)dt;
    if (this->field_0x14 == 0)
        return;
    int accum = this->field_0x10 + d;
    this->field_0x10 = accum;
    for (unsigned i = 0; i < this->field_0x18; i++) {
        void *p = ((void **)this->field_0x1c)[i];
        if (p != 0) {
            ((IParticleSystem*)(p))->update(d);
            p = ((void **)this->field_0x1c)[i];
            if (((IParticleSystem*)p)->field_0x4 == 0) {
                if (accum > 9 || ((IParticleSystem*)p)->emitterVelocityDirty != 0) {
                    ((IParticleSystem*)(p))->calcEmitterVelocity(this->field_0x10);
                    p = ((void **)this->field_0x1c)[i];
                }
                (*(void (**)(void *, int))(*(int *)p + 4))(p, d);
            } else {
                ((IParticleSystem*)(p))->resetEmitterVelocity();
            }
        }
    }
    for (unsigned i = 0; i < this->field_0x3c; i++) {
        void *p = ((void **)this->field_0x40)[i];
        if (p != 0) {
            ((IParticleSystem*)(p))->update(d);
            p = ((void **)this->field_0x40)[i];
            if (((IParticleSystem*)p)->field_0x4 == 0) {
                if (accum > 9 || ((IParticleSystem*)p)->emitterVelocityDirty != 0) {
                    ((IParticleSystem*)(p))->calcEmitterVelocity(this->field_0x10);
                    p = ((void **)this->field_0x40)[i];
                }
                (*(void (**)(void *, int))(*(int *)p + 4))(p, d);
            } else {
                ((IParticleSystem*)(p))->resetEmitterVelocity();
            }
        }
    }
    if (accum > 9)
        this->field_0x10 = 0;
}

// ParticleSystemManager::reset()
void ParticleSystemManager::reset()
{
    for (unsigned i = 0; i < this->field_0x18; i++) {
        int *p = ((int **)this->field_0x1c)[i];
        if (p != 0)
            (*(void (**)(int *))(*p + 8))(p);
    }
    for (unsigned i = 0; i < this->field_0x3c; i++) {
        int *p = ((int **)this->field_0x40)[i];
        if (p != 0)
            (*(void (**)(int *))(*p + 8))(p);
    }
}

// ParticleSystemManager::releaseSprites()
void ParticleSystemManager::releaseSprites()
{
    _psm_ArrayReleaseSprites(&this->field_0x18);
    if (this->field_0x30 != 0xffffffff) {
        _psm_ReleaseSpriteSystemResource(this->field_0x4, this->field_0x30);
        this->field_0x30 = 0xffffffff;
    }
}

// ParticleSystemManager::construct()
void ParticleSystemManager::construct()
{
    this->field_0x10 = 0;
    this->field_0x14 = 0;
    this->field_0x2c = 0xffffffff;
    this->field_0x30 = 0xffffffff;
    this->field_0x34 = 0;
    this->field_0x54 = 0xffffffff;
    this->field_0x58 = 0xffffffff;
    this->field_0x5c = 0;
    this->field_0x0 = 0x101;
}

// ParticleSystemManager::render3d()
void ParticleSystemManager::render3d()
{
    if (this->field_0x14 == 0)
        return;
    if ((*reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(&this->field_0x0) + 1)) != 0)
        _psm_renderMeshes(this);
    if ((*reinterpret_cast<uint8_t*>(&this->field_0x0)) == 0)
        return;
    return _psm_renderSpritesExt(this);
}

// ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex)
// Same bit-17 sprite/mesh dispatch as the other per-system setters; forwards the set index to
// the selected IParticleSystem.

void ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem*)(arr[idx]))->setParticleSetIndex(setIndex);
}

// ParticleSystemManager::enableSystemRender(int handle, bool enable)
// Bad-data ARM veneer that mirrors the other per-system setters' bit-17 sprite/mesh dispatch,
// forwarding to the IParticleSystem render-enable method.

void ParticleSystemManager::enableSystemRender(int handle, bool enable)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem*)(arr[idx]))->enableRender(enable);
}

// ParticleSystemManager::ParticleSystemManager(PaintCanvas*, CameraSet, unsigned short spriteTex,
//     bool spriteFlag, unsigned short meshTex, bool meshFlag)
// Stores the canvas and camera set, default-constructs the sprite (+0x18) and mesh (+0x3c)
// system arrays, seeds their texture/blend slots (the "blend" half left at 0xffff here), records
// the per-array flags at +0x38 / +0x60, then runs construct() to reset the runtime state.

extern "C" void _psm_arraySpriteCtor(void *arr);  // Array<ParticleSystemSprite*>::Array()
extern "C" void _psm_arrayMeshCtor(void *arr);     // Array<ParticleSystemMesh*>::Array()

void *ParticleSystemManager_ctor6(
    void *self, void *canvas, int cameraSet, unsigned short spriteTex, bool spriteFlag,
    unsigned short meshTex, bool meshFlag)
{
    ((ParticleSystemManager*)self)->field_0xc = cameraSet;
    ((ParticleSystemManager*)self)->field_0x4 = canvas;

    _psm_arraySpriteCtor(&((ParticleSystemManager*)self)->field_0x18);
    ((ParticleSystemManager*)self)->field_0x26 = 0xffff;
    ((ParticleSystemManager*)self)->field_0x24 = spriteTex;
    ((ParticleSystemManager*)self)->field_0x28 = 0;
    ((ParticleSystemManager*)self)->field_0x38 = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor(&((ParticleSystemManager*)self)->field_0x3c);
    ((ParticleSystemManager*)self)->field_0x4a = 0xffff;
    ((ParticleSystemManager*)self)->field_0x48 = meshTex;
    ((ParticleSystemManager*)self)->field_0x4c = 0;
    ((ParticleSystemManager*)self)->field_0x60 = meshFlag ? 1 : 0;

    _psm_construct(self);
    return self;
}

// Array<ParticleSystemMesh*>::release-ish, via PLT veneer; takes &mesh-array (this+0x3c).

// ParticleSystemManager::release()
void ParticleSystemManager::release()
{
    _psm_releaseSprites(this);
    this->field_0x4 = 0;
    return _psm_releaseMeshArray(&this->field_0x3c);
}

// re-init after camera change (PLT veneer); takes this.

// ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet)
void ParticleSystemManager::cameraToggle(int cam)
{
    if (this->field_0xc == cam)
        return;
    this->field_0xc = cam;
    _psm_releaseSprites(this);
    return _psm_constructAfterCamera(this);
}

// ParticleSystemManager::addMeshSystem(AEMath::Matrix const*, Array<ParticleSet> const&, bool)
// Allocates a ParticleSystemMesh (0xa0 bytes), constructs it against the manager's canvas
// (+0x04) and the mesh flag (+0x60), appends it to the mesh array (+0x3c), accumulates its
// particle count (virtual call, vtable+0x10) into +0x5c, and returns the new system's handle:
// the mesh-array index with bit 0x4000 set to mark it as a mesh-array handle.

extern "C" void *_psmesh_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                              bool b4, bool b5);                    // ParticleSystemMesh ctor
extern "C" void _psm_arrayMeshAdd(void *sys, void *arr);           // ArrayAdd<ParticleSystemMesh*>

unsigned int ParticleSystemManager::addMeshSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = operator new(0xa0);
    _psmesh_ctor(sys, this->field_0x4, matrix, sets, flag, this->field_0x60 != 0);
    _psm_arrayMeshAdd(sys, &this->field_0x3c);

    // getParticleCount lives at vtable offset 0x10 for the mesh system.
    typedef int (*pfn)(void *);
    int count = ((pfn *)*(void **)sys)[4](sys);
    this->field_0x5c = count + this->field_0x5c;

    return (unsigned int)(this->field_0x3c - 1) | 0x4000;
}

// ParticleSystemManager::emitManual(int handle, Vector const& pos, int ret, float)
// 4-argument overload: bit-17 sprite/mesh dispatch, emits one burst at `pos`. Returns the
// pass-through `ret` selector packed into the low word (the high word is -1 on the no-system
// path, matching the original's CONCAT44).

unsigned long long ParticleSystemManager::emitManual(int handle, const float *pos, int ret, float p4)
{
    (void)p4;
    if (handle == -1)
        return ((unsigned long long)0xffffffffu << 32) | (unsigned int)(unsigned long)this;

    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    _ips_emitManual(arr[idx], pos[0], pos[1], pos[2]);
    return (unsigned int)ret;
}

// 4-arg sprite renderer (DAT_001ac994 veneer): canvas, a, b, c
// 2-arg sprite renderer (DAT_001ac9a4 veneer): canvas, a

// ParticleSystemManager::renderSprites()
void ParticleSystemManager::renderSprites()
{
    if (this->field_0x24 != -1)
        return _psm_spriteRender2(this->field_0x4, this->field_0x30);
    if (this->field_0x26 != -1)
        return _psm_spriteRender4(this->field_0x4, this->field_0x30, this->field_0x2c, this->field_0x28);
}

// ParticleSystemManager::ParticleSystemManager(PaintCanvas*, CameraSet, unsigned short spriteTex,
//     BlendMode spriteBlend, bool spriteFlag, unsigned short meshTex, BlendMode meshBlend,
//     bool meshFlag)
// 8-argument overload that also takes explicit blend modes. Same field layout as the 6-arg ctor
// but the texture slots (+0x24/+0x48) are forced to 0xffff and the supplied texture goes to the
// "blend" half (+0x26/+0x4a), with the blend modes stored at +0x28/+0x4c.

void *ParticleSystemManager_ctor8(
    void *self, void *canvas, int cameraSet, unsigned short spriteTex, int spriteBlend,
    bool spriteFlag, unsigned short meshTex, int meshBlend, bool meshFlag)
{
    ((ParticleSystemManager*)self)->field_0xc = cameraSet;
    ((ParticleSystemManager*)self)->field_0x4 = canvas;

    _psm_arraySpriteCtor(&((ParticleSystemManager*)self)->field_0x18);
    ((ParticleSystemManager*)self)->field_0x26 = spriteTex;
    ((ParticleSystemManager*)self)->field_0x24 = 0xffff;
    ((ParticleSystemManager*)self)->field_0x28 = (unsigned int)spriteBlend;
    ((ParticleSystemManager*)self)->field_0x38 = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor(&((ParticleSystemManager*)self)->field_0x3c);
    ((ParticleSystemManager*)self)->field_0x4a = meshTex;
    ((ParticleSystemManager*)self)->field_0x48 = 0xffff;
    ((ParticleSystemManager*)self)->field_0x4c = (unsigned int)meshBlend;
    ((ParticleSystemManager*)self)->field_0x60 = meshFlag ? 1 : 0;

    _psm_construct(self);
    return self;
}

// ParticleSystemManager::systemSetMatrix(int handle, AEMath::Matrix const* matrix)
// Bit-17 sprite/mesh dispatch; forwards the world matrix to the selected IParticleSystem.

void ParticleSystemManager::systemSetMatrix(int handle, const void *matrix)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem*)(arr[idx]))->setMatrix((Matrix const*)(matrix));
}

// ParticleSystemManager::setParticleSetBySet(int handle, ParticleSettings::ParticleSet set)
// Bit-17 sprite/mesh dispatch; forwards a particle-set descriptor (passed by value as a single
// word in this build) to the selected IParticleSystem.

void ParticleSystemManager::setParticleSetBySet(unsigned int handle, unsigned int set)
{
    if (handle == 0xffffffffu)
        return;
    void **arr;
    unsigned int idx;
    if ((int)(handle << 0x11) < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem*)(arr[idx]))->setParticleSet(set);
}

// ParticleSystemManager::enableSystemUpdate(int handle, bool enable)
// Handles pack the array selector in bit 17: when set, the system lives in the mesh array
// (+0x40) and the low bits index it; otherwise it lives in the sprite array (+0x1c). -1 means
// "no system". The selected IParticleSystem's update-enable setter is then invoked.

void ParticleSystemManager::enableSystemUpdate(int handle, bool enable)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    _ips_enableUpdate(arr[idx], enable);
}

// ParticleSystemManager::initSprites()
// When the sprite array (+0x18) is non-empty and a camera set (+0xc) is active, this creates the
// shared sprite system on the canvas (+0x04) -- either from an existing texture id (+0x24) or by
// also creating a texture from id (+0x26) -- stores its handle at +0x30, primes the system-wide
// size/UV from the active particle-set's frame rect, then walks every sub-system, binding it to
// the sprite-system handle at its running particle offset.

// Active particle-set descriptor (the engine resolves it from a global table).
__attribute__((visibility("hidden"))) extern char *g_activeParticleSet;

void ParticleSystemManager::initSprites()
{
    if (this->field_0x18 == 0)
        return;

    this->field_0x30 = 0xffffffff;
    if (this->field_0xc == 0)
        return;

    void *canvas = this->field_0x4;
    if (this->field_0x24 == 0xffff) {
        if ((short)this->field_0x26 != -1) {
            ((PaintCanvas *)canvas)->SpriteSystemCreate((unsigned short)this->field_0x34, false,
                                           (unsigned int *)(&this->field_0x30));
            ((PaintCanvas *)canvas)->TextureCreate((unsigned short)this->field_0x26,
                                      (unsigned int *)(&this->field_0x30),
                                      (((char)(unsigned long)this + ',') != 0));
        }
    } else {
        ((PaintCanvas *)canvas)->SpriteSystemCreate((unsigned short)this->field_0x34, false,
                                          (unsigned short)this->field_0x24,
                                          (unsigned int *)(&this->field_0x30));
    }

    short offset = 0;
    ((PaintCanvas *)this->field_0x4)->SpriteSystemSetAllSize((unsigned int)(short)this->field_0x30, 0);

    float u = *(float *)(g_activeParticleSet + 0x90);
    float w = *(float *)(g_activeParticleSet + 0x94);
    ((PaintCanvas *)this->field_0x4)->SpriteSystemSetAllUv(this->field_0x30, u, 0.0f, w, 0.0f);

    for (unsigned i = 0; i < this->field_0x18; ++i) {
        void *sys = ((void **)this->field_0x1c)[i];
        typedef void (*pfn)(void *, unsigned int, short, void *, float, float);
        pfn fn = (pfn)(*(void ***)sys)[0];
        fn(sys, this->field_0x30, offset, (void *)fn, u, w);
        offset = offset + _ips_getParticleCount16(((void **)this->field_0x1c)[i]);
    }
}

// ParticleSystemManager::addSpriteSystem(AEMath::Matrix const*, Array<ParticleSet> const&, bool)
// Allocates a ParticleSystemSprite (0x78 bytes), constructs it against the manager's canvas
// (+0x04) and the sprite flag (+0x38), appends it to the sprite array (+0x18), accumulates its
// particle count into +0x34, and returns the new system's sprite-array handle (index, no flag).

extern "C" void *_pss_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                           bool b4, bool b5);                       // ParticleSystemSprite ctor
extern "C" void _psm_arraySpriteAdd(void *sys, void *arr);          // ArrayAdd<ParticleSystemSprite*>
extern "C" int  _ips_getParticleCount(void *sys);                  // IParticleSystem::getParticleCount

int ParticleSystemManager::addSpriteSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = operator new(0x78);
    _pss_ctor(sys, this->field_0x4, matrix, sets, flag, this->field_0x38 != 0);
    _psm_arraySpriteAdd(sys, &this->field_0x18);
    this->field_0x34 = _ips_getParticleCount(sys) + this->field_0x34;
    return this->field_0x18 - 1;
}

// ParticleSystemManager::initMesh()
// Mesh analogue of initSprites. When the mesh array (+0x3c) is non-empty it creates the shared
// particle mesh on the canvas (+0x04) from the packed vertex/index counts in +0x5c (low 14 bits
// << 2 verts, low 15 bits << 1 indices, format 0x1b), optionally creating its texture (+0x4a),
// stores the mesh handle at +0x54, builds a transform (+0x58) bound to that mesh, then binds
// every sub-system at its running vertex offset (4 bytes per particle).

void ParticleSystemManager::initMesh()
{
    if (this->field_0x3c == 0)
        return;

    this->field_0x54 = 0xffffffff;
    this->field_0x58 = 0xffffffff;

    void *canvas = this->field_0x4;
    int verts   = ((*reinterpret_cast<uint16_t*>(&this->field_0x5c)) & 0x3fff) << 2;
    int indices = ((*reinterpret_cast<uint16_t*>(&this->field_0x5c)) & 0x7fff) << 1;

    if ((short)this->field_0x48 == -1) {
        if ((short)this->field_0x4a != -1) {
            ((PaintCanvas *)canvas)->MeshCreate((unsigned short)verts, (unsigned short)indices,
                                   (signed char)0x1b, (unsigned int *)(&this->field_0x54));
            ((PaintCanvas *)canvas)->TextureCreate((unsigned short)this->field_0x4a,
                                      (unsigned int *)(&this->field_0x54),
                                      (((char)(unsigned long)this + 'P') != 0));
        }
    } else {
        ((PaintCanvas *)canvas)->MeshCreate((unsigned short)verts, (unsigned short)indices,
                                  (signed char)0x1b, (unsigned short)this->field_0x48,
                                  (unsigned int *)(&this->field_0x54));
    }

    ((PaintCanvas *)this->field_0x4)->TransformCreate((unsigned int *)(&this->field_0x58));
    ((PaintCanvas *)this->field_0x4)->TransformAddMeshId(this->field_0x58, this->field_0x54);

    short offset = 0;
    for (unsigned i = 0; i < this->field_0x3c; ++i) {
        void *sys = ((void **)this->field_0x40)[i];
        typedef void (*pfn)(void *, unsigned int, short);
        ((pfn)(*(void ***)sys)[0])(sys, this->field_0x54, offset);

        void *sys2 = ((void **)this->field_0x40)[i];
        typedef short (*pfn2)(void *);
        short count = ((pfn2 *)*(void **)sys2)[4](sys2);   // vtable+0x10
        offset = offset + (short)(count * 4);
    }
}

// ParticleSystemManager::emitManual(int handle, Vector const& pos, int ret, Vector const& vel, float)
// Bit-17 sprite/mesh dispatch; emits one burst at the given position into the selected system.
// The decompiled return is the pass-through `ret` selector word when a system was hit, else
// `this`. Modeled as the dispatch + emit; we return the selector for fidelity.
//
// NB: the first explicit param is `this` (in r0); the engine passes the manager pointer there.

void *ParticleSystemManager_emitManual_v(
    void *self, int handle, const float *pos, void *ret, const float *vel, float p5)
{
    (void)vel; (void)p5;
    void *result = self;
    if (handle != -1) {
        void **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = (void **)((ParticleSystemManager*)self)->field_0x40;
            idx = handle & 0x3fffffff;
        } else {
            arr = (void **)((ParticleSystemManager*)self)->field_0x1c;
            idx = handle;
        }
        _ips_emitManual(arr[idx], pos[0], pos[1], pos[2]);
        result = ret;
    }
    return result;
}

// ParticleSystemManager::enableSystemEmit(int handle, bool enable)
// The target lifts as bad ARM data (a tail-call veneer), but it is the same bit-17 sprite/mesh
// handle dispatch as the sibling setters, forwarding to the IParticleSystem emit-enable method.

void ParticleSystemManager::enableSystemEmit(int handle, bool enable)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem*)(arr[idx]))->enableEmit(enable);
}

// ParticleSystemManager::addSystem(AEMath::Matrix const*, ParticleSettings::ParticleSet, bool)
// The 16-byte target lifts as bad ARM data (a tail-call veneer). It is a thin forwarder that
// constructs a particle system for the given set and registers it; we model it as a tail-call to
// the sprite-system add path (the default add route), returning its handle.

int ParticleSystemManager::addSystem(const void *matrix, unsigned int set, bool flag)
{
    return _psm_addSpriteSystem(this, matrix, set, flag);
}

// ParticleSystemManager::init()
// Builds the sprite and mesh sub-systems, marks the manager active (+0x14), and runs the first
// update tick (via the virtual update at vtable+? -> here the resolved emitter-update entry).

int ParticleSystemManager::init()
{
    _psm_initSprites(this);
    _psm_initMesh(this);
    this->field_0x14 = 1;
    return _psm_firstUpdate(this, 0, 0, 0);
}

// ParticleSystemManager::resetSystem(int handle)
// Bad-data ARM veneer; same bit-17 sprite/mesh handle dispatch, tail-calling the selected
// IParticleSystem's reset method.

void ParticleSystemManager::resetSystem(int handle)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    _ips_reset(arr[idx]);
}

// 4-arg renderer (DAT_001ac974 veneer): canvas, a, b, c
// 2-arg renderer (DAT_001ac984 veneer): canvas, a

// ParticleSystemManager::renderMeshes()
void ParticleSystemManager::renderMeshes()
{
    if (this->field_0x48 != -1)
        return _psm_meshRender2(this->field_0x4, this->field_0x58);
    if (this->field_0x26 != -1)
        return _psm_meshRender4(this->field_0x4, this->field_0x58, this->field_0x50, this->field_0x4c);
}

// ParticleSystemManager::attachSystem(int handle, bool enable)
// Binds a previously added sub-system (identified by its handle) into the active set. The handle's
// top bit (bit 17 of the shifted value) selects the mesh array (+0x40) vs the sprite array (+0x1c);
// the resolved IParticleSystem is then armed via its emit-enable entry, exactly like the sibling
// enableSystem* dispatchers.
void ParticleSystemManager::attachSystem(int handle, bool enable)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)this->field_0x40;
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)this->field_0x1c;
        idx = handle;
    }
    ((IParticleSystem *)(arr[idx]))->enableEmit(enable);
}

// ParticleSystemManager::enableSystemEmit2(int handle, bool enable)
// Secondary entry of the emit-enable dispatcher (same bit-17 sprite/mesh handle split as
// enableSystemEmit), used by the ego-craft thruster bookkeeping.
void ParticleSystemManager::enableSystemEmit2(int handle, bool enable)
{
    enableSystemEmit(handle, enable);
}

// ParticleSystemManager::enableSystemEmit3(int handle, bool enable)
// Tertiary entry of the emit-enable dispatcher; identical handle dispatch.
void ParticleSystemManager::enableSystemEmit3(int handle, bool enable)
{
    enableSystemEmit(handle, enable);
}
