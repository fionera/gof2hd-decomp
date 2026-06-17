#include "engine/render/ParticleSystemManager.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/PaintCanvas.h"

// Engine helper forwarders (resolved by a later externs pass).
extern "C" void  _psm_ArrayReleaseSprites(void *arr);
extern "C" void  _psm_ReleaseSpriteSystemResource(void *canvas, unsigned res);
extern "C" void  _psm_renderMeshes(void *self);
extern "C" void  _psm_renderSpritesExt(void *self);
extern "C" void  _psm_releaseSprites(void *self);
extern "C" void  _psm_releaseMeshArray(void *arr);
extern "C" void  _psm_constructAfterCamera(void *self);
extern "C" void  _ips_emitManual(void *sys, float x, float y, float z);
extern "C" void  _psm_spriteRender4(void *canvas, unsigned a, unsigned b, unsigned c);
extern "C" void  _psm_spriteRender2(void *canvas, unsigned a);
extern "C" void  _psm_arraySpriteCtor(void *arr);   // Array<ParticleSystemSprite*>::Array()
extern "C" void  _psm_arrayMeshCtor(void *arr);      // Array<ParticleSystemMesh*>::Array()
extern "C" void  _ips_enableUpdate(void *sys, bool enable);
extern "C" short _ips_getParticleCount16(void *sys);
extern "C" int   _psm_addSpriteSystem(void *self, const void *matrix, unsigned int set, bool flag);
extern "C" int   _psm_firstUpdate(void *self, int a, int b, int c);
extern "C" void  _ips_reset(void *sys);
extern "C" void  _psm_meshRender4(void *canvas, unsigned a, unsigned b, unsigned c);
extern "C" void  _psm_meshRender2(void *canvas, unsigned a);
extern "C" void *_psmesh_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                              bool b4, bool b5);     // ParticleSystemMesh ctor
extern "C" void  _psm_arrayMeshAdd(void *sys, void *arr);    // ArrayAdd<ParticleSystemMesh*>
extern "C" void *_pss_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                           bool b4, bool b5);        // ParticleSystemSprite ctor
extern "C" void  _psm_arraySpriteAdd(void *sys, void *arr);  // ArrayAdd<ParticleSystemSprite*>
extern "C" int   _ips_getParticleCount(void *sys);  // IParticleSystem::getParticleCount

// Active particle-set descriptor (the engine resolves it from a global table).
__attribute__((visibility("hidden"))) extern char *g_activeParticleSet;

// Resolve a system handle to its slot in either the mesh or sprite array. Bit 17 of the shifted
// handle selects the mesh array; -1 means "no system" and yields a null pointer.
static IParticleSystem *resolveSystem(ParticleSystemManager *self, int handle)
{
    if (handle == -1)
        return nullptr;
    IParticleSystem **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (IParticleSystem **)self->meshSystems;
        idx = handle & 0x3fffffff;
    } else {
        arr = (IParticleSystem **)self->spriteSystems;
        idx = handle;
    }
    return arr[idx];
}

ParticleSystemManager::ParticleSystemManager(
    void *canvas, int cameraSet, unsigned short spriteTex, bool spriteFlag,
    unsigned short meshTex, bool meshFlag)
{
    this->cameraSet = cameraSet;
    this->canvas = canvas;

    _psm_arraySpriteCtor(&this->spriteSystemCount);
    this->spriteUvId = 0xffff;
    this->spriteTextureId = spriteTex;
    this->spriteBlendMode = 0;
    this->spriteUsesExtra = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor(&this->meshSystemCount);
    this->meshUvId = 0xffff;
    this->meshTextureId = meshTex;
    this->meshBlendMode = 0;
    this->meshUsesExtra = meshFlag ? 1 : 0;

    construct();
}

// 8-argument overload that also takes explicit blend modes. Same field layout as the 6-arg ctor but
// the texture slots are forced to 0xffff and the supplied texture goes to the "uv" half, with the
// blend modes stored explicitly.
ParticleSystemManager::ParticleSystemManager(
    void *canvas, int cameraSet, unsigned short spriteTex, int spriteBlend, bool spriteFlag,
    unsigned short meshTex, int meshBlend, bool meshFlag)
{
    this->cameraSet = cameraSet;
    this->canvas = canvas;

    _psm_arraySpriteCtor(&this->spriteSystemCount);
    this->spriteUvId = spriteTex;
    this->spriteTextureId = 0xffff;
    this->spriteBlendMode = (unsigned int)spriteBlend;
    this->spriteUsesExtra = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor(&this->meshSystemCount);
    this->meshUvId = meshTex;
    this->meshTextureId = 0xffff;
    this->meshBlendMode = (unsigned int)meshBlend;
    this->meshUsesExtra = meshFlag ? 1 : 0;

    construct();
}

void ParticleSystemManager::update(long long dt)
{
    int d = (int)dt;
    if (this->enabled == 0)
        return;
    int accum = this->accumulatedDt + d;
    this->accumulatedDt = accum;

    IParticleSystem **sprites = (IParticleSystem **)this->spriteSystems;
    for (unsigned i = 0; i < this->spriteSystemCount; i++) {
        IParticleSystem *p = sprites[i];
        if (p != nullptr) {
            p->update(d);
            p = sprites[i];
            if (p->canvas == nullptr) {
                if (accum > 9 || p->emitterVelocityDirty != 0) {
                    p->calcEmitterVelocity(this->accumulatedDt);
                    p = sprites[i];
                }
                p->emit(d);
            } else {
                p->resetEmitterVelocity();
            }
        }
    }

    IParticleSystem **meshes = (IParticleSystem **)this->meshSystems;
    for (unsigned i = 0; i < this->meshSystemCount; i++) {
        IParticleSystem *p = meshes[i];
        if (p != nullptr) {
            p->update(d);
            p = meshes[i];
            if (p->canvas == nullptr) {
                if (accum > 9 || p->emitterVelocityDirty != 0) {
                    p->calcEmitterVelocity(this->accumulatedDt);
                    p = meshes[i];
                }
                p->emit(d);
            } else {
                p->resetEmitterVelocity();
            }
        }
    }

    if (accum > 9)
        this->accumulatedDt = 0;
}

void ParticleSystemManager::reset()
{
    IParticleSystem **sprites = (IParticleSystem **)this->spriteSystems;
    for (unsigned i = 0; i < this->spriteSystemCount; i++) {
        IParticleSystem *p = sprites[i];
        if (p != nullptr)
            _ips_reset(p);
    }
    IParticleSystem **meshes = (IParticleSystem **)this->meshSystems;
    for (unsigned i = 0; i < this->meshSystemCount; i++) {
        IParticleSystem *p = meshes[i];
        if (p != nullptr)
            _ips_reset(p);
    }
}

void ParticleSystemManager::releaseSprites()
{
    _psm_ArrayReleaseSprites(&this->spriteSystemCount);
    if (this->spriteSystemId != 0xffffffff) {
        _psm_ReleaseSpriteSystemResource(this->canvas, this->spriteSystemId);
        this->spriteSystemId = 0xffffffff;
    }
}

// Reset the runtime state shared by both constructors.
void ParticleSystemManager::construct()
{
    this->accumulatedDt = 0;
    this->enabled = 0;
    this->spriteMeshId = 0xffffffff;
    this->spriteSystemId = 0xffffffff;
    this->spriteParticleCount = 0;
    this->meshId = 0xffffffff;
    this->transformId = 0xffffffff;
    this->meshParticleCount = 0;
    this->flags = 0x101;
}

void ParticleSystemManager::render3d()
{
    if (this->enabled == 0)
        return;
    bool meshActive   = (this->flags & 0xff00) != 0;
    bool spriteActive = (this->flags & 0x00ff) != 0;
    if (meshActive)
        _psm_renderMeshes(this);
    if (spriteActive)
        _psm_renderSpritesExt(this);
}

void ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        sys->setParticleSetIndex(setIndex);
}

void ParticleSystemManager::enableSystemRender(int handle, bool enable)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        sys->enableRender(enable);
}

void ParticleSystemManager::release()
{
    _psm_releaseSprites(this);
    this->canvas = nullptr;
    _psm_releaseMeshArray(&this->meshSystemCount);
}

void ParticleSystemManager::cameraToggle(int cam)
{
    if (this->cameraSet == cam)
        return;
    this->cameraSet = cam;
    _psm_releaseSprites(this);
    _psm_constructAfterCamera(this);
}

// Allocates a ParticleSystemMesh, constructs it against the manager's canvas and mesh flag, appends
// it to the mesh array, accumulates its particle count, and returns the new system's handle: the
// mesh-array index with bit 0x4000 set to mark it as a mesh-array handle.
unsigned int ParticleSystemManager::addMeshSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = ::operator new(0xa0);
    _psmesh_ctor(sys, this->canvas, matrix, sets, flag, this->meshUsesExtra != 0);
    _psm_arrayMeshAdd(sys, &this->meshSystemCount);

    this->meshParticleCount += _ips_getParticleCount(sys);

    return (this->meshSystemCount - 1) | 0x4000;
}

// 4-argument overload: bit-17 sprite/mesh dispatch, emits one burst at `pos`. Returns the
// pass-through `ret` selector packed into the low word (the high word is -1 on the no-system path).
unsigned long long ParticleSystemManager::emitManual(int handle, const float *pos, int ret, float p4)
{
    (void)p4;
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys == nullptr)
        return ((unsigned long long)0xffffffffu << 32) | (unsigned int)(unsigned long)this;

    _ips_emitManual(sys, pos[0], pos[1], pos[2]);
    return (unsigned int)ret;
}

void ParticleSystemManager::renderSprites()
{
    if (this->spriteTextureId != -1)
        _psm_spriteRender2(this->canvas, this->spriteSystemId);
    else if (this->spriteUvId != -1)
        _psm_spriteRender4(this->canvas, this->spriteSystemId, this->spriteMeshId, this->spriteBlendMode);
}

void ParticleSystemManager::systemSetMatrix(int handle, const void *matrix)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        sys->setMatrix((Matrix const *)matrix);
}

// Particle-set descriptor passed by value as a single word in this build.
void ParticleSystemManager::setParticleSetBySet(unsigned int handle, unsigned int set)
{
    IParticleSystem *sys = resolveSystem(this, (int)handle);
    if (sys != nullptr)
        sys->setParticleSet(set);
}

void ParticleSystemManager::enableSystemUpdate(int handle, bool enable)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        _ips_enableUpdate(sys, enable);
}

// When the sprite array is non-empty and a camera set is active, create the shared sprite system on
// the canvas -- either from an existing texture id or by also creating a texture from a uv id --
// store its handle, prime the system-wide size/UV from the active particle-set's frame rect, then
// walk every sub-system binding it to the sprite-system handle at its running particle offset.
void ParticleSystemManager::initSprites()
{
    if (this->spriteSystemCount == 0)
        return;

    this->spriteSystemId = 0xffffffff;
    if (this->cameraSet == 0)
        return;

    PaintCanvas *canvas = (PaintCanvas *)this->canvas;
    if ((unsigned short)this->spriteTextureId == 0xffff) {
        if (this->spriteUvId != -1) {
            canvas->SpriteSystemCreate((unsigned short)this->spriteParticleCount, false,
                                       &this->spriteSystemId);
            canvas->TextureCreate((unsigned short)this->spriteUvId, &this->spriteSystemId,
                                  (((char)(unsigned long)this + ',') != 0));
        }
    } else {
        canvas->SpriteSystemCreate((unsigned short)this->spriteParticleCount, false,
                                   (unsigned short)this->spriteTextureId, &this->spriteSystemId);
    }

    short offset = 0;
    canvas->SpriteSystemSetAllSize((unsigned int)(short)this->spriteSystemId, 0);

    float u = *(float *)(g_activeParticleSet + 0x90);
    float w = *(float *)(g_activeParticleSet + 0x94);
    canvas->SpriteSystemSetAllUv(this->spriteSystemId, u, 0.0f, w, 0.0f);

    IParticleSystem **sprites = (IParticleSystem **)this->spriteSystems;
    for (unsigned i = 0; i < this->spriteSystemCount; ++i) {
        IParticleSystem *sys = sprites[i];
        sys->init(this->spriteSystemId, (uint16_t)offset);   // slot 0
        offset += _ips_getParticleCount16(sprites[i]);
    }
}

// Allocates a ParticleSystemSprite, constructs it against the manager's canvas and sprite flag,
// appends it to the sprite array, accumulates its particle count, and returns its sprite-array handle.
int ParticleSystemManager::addSpriteSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = ::operator new(0x78);
    _pss_ctor(sys, this->canvas, matrix, sets, flag, this->spriteUsesExtra != 0);
    _psm_arraySpriteAdd(sys, &this->spriteSystemCount);
    this->spriteParticleCount += _ips_getParticleCount(sys);
    return this->spriteSystemCount - 1;
}

// Mesh analogue of initSprites. When the mesh array is non-empty it creates the shared particle mesh
// on the canvas from the packed vertex/index counts (format 0x1b), optionally creating its texture,
// stores the mesh handle, builds a transform bound to that mesh, then binds every sub-system at its
// running vertex offset (4 bytes per particle).
void ParticleSystemManager::initMesh()
{
    if (this->meshSystemCount == 0)
        return;

    this->meshId = 0xffffffff;
    this->transformId = 0xffffffff;

    PaintCanvas *canvas = (PaintCanvas *)this->canvas;
    int verts   = (int)((this->meshParticleCount & 0x3fff) << 2);
    int indices = (int)((this->meshParticleCount & 0x7fff) << 1);

    if (this->meshTextureId == -1) {
        if (this->meshUvId != -1) {
            canvas->MeshCreate((unsigned short)verts, (unsigned short)indices, (signed char)0x1b,
                               &this->meshId);
            canvas->TextureCreate((unsigned short)this->meshUvId, &this->meshId,
                                  (((char)(unsigned long)this + 'P') != 0));
        }
    } else {
        canvas->MeshCreate((unsigned short)verts, (unsigned short)indices, (signed char)0x1b,
                           (unsigned short)this->meshTextureId, &this->meshId);
    }

    canvas->TransformCreate(&this->transformId);
    canvas->TransformAddMeshId(this->transformId, this->meshId);

    short offset = 0;
    IParticleSystem **meshes = (IParticleSystem **)this->meshSystems;
    for (unsigned i = 0; i < this->meshSystemCount; ++i) {
        IParticleSystem *sys = meshes[i];
        sys->init(this->meshId, (uint16_t)offset);   // slot 0

        short count = _ips_getParticleCount16(meshes[i]);
        offset += (short)(count * 4);
    }
}

void ParticleSystemManager::enableSystemEmit(int handle, bool enable)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        sys->enableEmit(enable);
}

// Thin forwarder that constructs a particle system for the given set and registers it; modelled as a
// tail-call to the sprite-system add path (the default add route), returning its handle.
int ParticleSystemManager::addSystem(const void *matrix, unsigned int set, bool flag)
{
    return _psm_addSpriteSystem(this, matrix, set, flag);
}

// Builds the sprite and mesh sub-systems, marks the manager active, and runs the first update tick.
int ParticleSystemManager::init()
{
    initSprites();
    initMesh();
    this->enabled = 1;
    return _psm_firstUpdate(this, 0, 0, 0);
}

void ParticleSystemManager::resetSystem(int handle)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        _ips_reset(sys);
}

void ParticleSystemManager::renderMeshes()
{
    if (this->meshTextureId != -1)
        _psm_meshRender2(this->canvas, this->transformId);
    else if (this->spriteUvId != -1)
        _psm_meshRender4(this->canvas, this->transformId, this->meshExtraId, this->meshBlendMode);
}

// Binds a previously added sub-system (identified by its handle) into the active set via its
// emit-enable entry, exactly like the sibling enableSystem* dispatchers.
void ParticleSystemManager::attachSystem(int handle, bool enable)
{
    IParticleSystem *sys = resolveSystem(this, handle);
    if (sys != nullptr)
        sys->enableEmit(enable);
}

// Secondary entry of the emit-enable dispatcher, used by the ego-craft thruster bookkeeping.
void ParticleSystemManager::enableSystemEmit2(int handle, bool enable)
{
    enableSystemEmit(handle, enable);
}

// Tertiary entry of the emit-enable dispatcher; identical handle dispatch.
void ParticleSystemManager::enableSystemEmit3(int handle, bool enable)
{
    enableSystemEmit(handle, enable);
}
