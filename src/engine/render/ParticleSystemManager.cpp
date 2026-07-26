

#include "engine/core/Array.h"
#include "engine/render/ParticleSettingsRef.h"

#include "engine/render/ParticleSystemMesh.h"
#define GOF2_ENUM_BlendMode

#include "engine/render/ParticleSystemManager.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSystemSprite.h"
#include "engine/render/PaintCanvas.h"

// lint: void_ptr -- the following forward-declared shims are UNDEFINED (UND)
// symbols in the linked .so; their mangled names encode `Pv` (void*), so their
// signatures are fixed by symbol parity and must NOT be retyped.
void _psm_ArrayReleaseSprites(void *arr); // lint: void_ptr

void _psm_ReleaseSpriteSystemResource(void *canvas, unsigned res); // lint: void_ptr

void _ips_emitManual(void *sys, float x, float y, float z); // lint: void_ptr

void _psm_arraySpriteCtor(void *arr); // lint: void_ptr

void _psm_arraySpriteDtor(void *arr); // lint: void_ptr

ParticleSystemManager::ParticleSystemManager(
    PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet, unsigned short spriteTex,
    bool spriteFlag, unsigned short meshTex, bool meshFlag)
    : canvas(canvas), cameraSet(cameraSet) {
    this->spriteTextureId = spriteTex;
    this->spriteUvId = 0xffff;
    this->spriteBlendMode = 0;
    this->spriteUsesExtra = spriteFlag;

    new(&meshArray()) Array<ParticleSystemMesh *>();
    this->meshTextureId = meshTex;
    this->meshUvId = 0xffff;
    this->meshBlendMode = 0;
    this->meshUsesExtra = meshFlag;

    construct();
}

ParticleSystemManager::ParticleSystemManager(
    PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet, unsigned short spriteTex,
    AbyssEngine::BlendMode spriteBlend, bool spriteFlag,
    unsigned short meshTex, AbyssEngine::BlendMode meshBlend, bool meshFlag)
    : canvas(canvas), cameraSet(cameraSet) {
    this->spriteUvId = spriteTex;
    this->spriteTextureId = 0xffff;
    this->spriteBlendMode = spriteBlend;
    this->spriteUsesExtra = spriteFlag;

    new(&meshArray()) Array<ParticleSystemMesh *>();
    this->meshTextureId = 0xffff;
    this->meshUvId = meshTex;
    this->meshBlendMode = meshBlend;
    this->meshUsesExtra = meshFlag;

    construct();
}

void ParticleSystemManager::update(long long dt) {
    int d = (int) dt;
    if (this->enabled == 0)
        return;
    int accum = this->accumulatedDt + d;
    this->accumulatedDt = accum;

    for (unsigned i = 0; i < this->spriteSystems.count; i++) {
        IParticleSystem *p = this->spriteSystems[i];
        if (p != nullptr) {
            p->update(d);
            p = this->spriteSystems[i];
            if (p->field_0x4 == 0) {
                if (accum > 9 || p->emitterVelocityDirty != 0) {
                    p->calcEmitterVelocity(this->accumulatedDt);
                    p = this->spriteSystems[i];
                }
                p->emit(d);
            } else {
                p->resetEmitterVelocity();
            }
        }
    }

    for (unsigned i = 0; i < this->meshSystemCount; i++) {
        IParticleSystem *p = this->meshSystems[i];
        if (p != nullptr) {
            p->update(d);
            p = this->meshSystems[i];
            if (p->field_0x4 == 0) {
                if (accum > 9 || p->emitterVelocityDirty != 0) {
                    p->calcEmitterVelocity(this->accumulatedDt);
                    p = this->meshSystems[i];
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

void ParticleSystemManager::reset() {
    IParticleSystem **sprites = (IParticleSystem **) this->spriteSystems.data_;
    for (unsigned i = 0; i < this->spriteSystems.count; i++) {
        IParticleSystem *p = sprites[i];
        if (p != nullptr)
            p->reset();
    }
    IParticleSystem **meshes = (IParticleSystem **) this->meshSystems;
    for (unsigned i = 0; i < this->meshSystemCount; i++) {
        IParticleSystem *p = meshes[i];
        if (p != nullptr)
            p->reset();
    }
}

void ParticleSystemManager::releaseSprites() {
    ArrayReleaseClasses(this->spriteSystems);
    if (this->spriteSystemId != 0xffffffff) {
        this->canvas->ReleaseSpriteSystemResource(this->spriteSystemId);
        this->spriteSystemId = 0xffffffff;
    }
}

void ParticleSystemManager::construct() {
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

void ParticleSystemManager::render3d() {
    if (this->enabled == 0)
        return;
    uint8_t *active = reinterpret_cast<uint8_t *>(&this->flags);
    if (active[1] != 0)
        renderMeshes();
    if (active[0] != 0)
        renderSprites();
}

void ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->setParticleSetIndex(setIndex);
}

void ParticleSystemManager::enableSystemRender(int handle, bool enable) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->enableRender(enable);
}

void ParticleSystemManager::release() {
    releaseSprites();
    this->canvas = nullptr;
    ArrayReleaseClasses<ParticleSystemMesh *>(meshArray());
}

void ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet cam) {
    if (this->cameraSet == cam)
        return;
    this->cameraSet = cam;
    releaseSprites();
    initSprites();
}

unsigned int ParticleSystemManager::addMeshSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                                  Array<ParticleSettings::ParticleSet> const &sets,
                                                  bool flag) {
    ParticleSystemMesh *sys =
        new ParticleSystemMesh(this->canvas, matrix, sets, flag, this->meshUsesExtra);
    ArrayAdd<ParticleSystemMesh *>(sys, meshArray());

    this->meshParticleCount += sys->getQuadCount();

    return (this->meshSystemCount - 1) | 0x4000;
}

unsigned long long ParticleSystemManager::emitManual(int handle, AbyssEngine::AEMath::Vector const &pos,
                                                     int ret, float p4) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys == nullptr)
        return ((unsigned long long) 0xffffffffu << 32) | (unsigned int) (unsigned long) this;

    sys->emitManual(pos, ret, nullptr, p4);
    return (unsigned int) ret;
}

unsigned long long ParticleSystemManager::emitManual(int handle, AbyssEngine::AEMath::Vector const &pos,
                                                     int ret, AbyssEngine::AEMath::Vector const &velocity,
                                                     float p5) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys == nullptr)
        return ((unsigned long long) 0xffffffffu << 32) | (unsigned int) (unsigned long) this;

    sys->emitManual(pos, ret, &velocity, p5);
    return (unsigned int) ret;
}

void ParticleSystemManager::renderSprites() {
    if (this->spriteTextureId != -1)
        ParticleSystemSprite::render(this->canvas, this->spriteSystemId);
    else if (this->spriteUvId != -1)
        ParticleSystemSprite::render(
            this->canvas, this->spriteSystemId, this->spriteMeshId,
            static_cast<AbyssEngine::BlendMode>(this->spriteBlendMode));
}

void ParticleSystemManager::systemSetMatrix(int handle, AbyssEngine::AEMath::Matrix const *matrix) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->setMatrix(matrix);
}

void ParticleSystemManager::setParticleSetBySet(int handle, ParticleSettings::ParticleSet set) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->setParticleSet(set);
}

void ParticleSystemManager::enableSystemUpdate(int handle, bool enable) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->enableUpdate(enable);
}

void ParticleSystemManager::initSprites() {
    if (this->spriteSystems.count == 0)
        return;

    this->spriteSystemId = 0xffffffff;
    if (this->cameraSet == 0)
        return;

    PaintCanvas *canvas = (PaintCanvas *) this->canvas;
    if ((unsigned short) this->spriteTextureId == 0xffff) {
        if (this->spriteUvId != -1) {
            canvas->SpriteSystemCreate((unsigned short) this->spriteParticleCount, false,
                                       this->spriteSystemId);
            canvas->TextureCreate((unsigned short) this->spriteUvId, this->spriteSystemId,
                                  (((char) (unsigned long) this + ',') != 0));
        }
    } else {
        canvas->SpriteSystemCreate((unsigned short) this->spriteParticleCount, false,
                                   (unsigned short) this->spriteTextureId, this->spriteSystemId);
    }

    short offset = 0;
    canvas->SpriteSystemSetAllSize((unsigned int) (short) this->spriteSystemId, 0);

    float u = *(float *) ((char *) ParticleSettingsRef::cur + 0x90);
    float w = *(float *) ((char *) ParticleSettingsRef::cur + 0x94);
    canvas->SpriteSystemSetAllUv(this->spriteSystemId, u, 0.0f, w, 0.0f);

    IParticleSystem **sprites = (IParticleSystem **) this->spriteSystems.data_;
    for (unsigned i = 0; i < this->spriteSystems.count; ++i) {
        IParticleSystem *sys = sprites[i];
        sys->init(this->spriteSystemId, (uint16_t) offset);
        offset += sprites[i]->getParticleCount();
    }
}

int ParticleSystemManager::addSpriteSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                           Array<ParticleSettings::ParticleSet> const &sets, bool flag) {
    ParticleSystemSprite *sys =
        new ParticleSystemSprite(this->canvas, matrix, sets, flag, this->spriteUsesExtra);
    ArrayAdd<ParticleSystemSprite *>(sys, spriteArray());
    this->spriteParticleCount += sys->getParticleCount();
    return this->spriteSystems.count - 1;
}

void ParticleSystemManager::initMesh() {
    if (this->meshSystemCount == 0)
        return;

    this->meshId = 0xffffffff;
    this->transformId = 0xffffffff;

    PaintCanvas *canvas = (PaintCanvas *) this->canvas;
    int verts = (int) ((this->meshParticleCount & 0x3fff) << 2);
    int indices = (int) ((this->meshParticleCount & 0x7fff) << 1);

    if (this->meshTextureId == -1) {
        if (this->meshUvId != -1) {
            canvas->MeshCreate((unsigned short) verts, (unsigned short) indices, (signed char) 0x1b,
                               this->meshId);
            canvas->TextureCreate((unsigned short) this->meshUvId, this->meshId,
                                  (((char) (unsigned long) this + 'P') != 0));
        }
    } else {
        canvas->MeshCreate((unsigned short) verts, (unsigned short) indices, (signed char) 0x1b,
                           (unsigned short) this->meshTextureId, this->meshId);
    }

    canvas->TransformCreate(this->transformId);
    canvas->TransformAddMeshId(this->transformId, this->meshId);

    short offset = 0;
    IParticleSystem **meshes = (IParticleSystem **) this->meshSystems;
    for (unsigned i = 0; i < this->meshSystemCount; ++i) {
        IParticleSystem *sys = meshes[i];
        sys->init(this->meshId, (uint16_t) offset);

        short count = meshes[i]->getQuadCount();
        offset += (short) (count * 4);
    }
}

void ParticleSystemManager::enableSystemEmit(int handle, bool enable) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->enableEmit(enable);
}

int ParticleSystemManager::addSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                     ParticleSettings::ParticleSet set, bool flag) {
    Array<ParticleSettings::ParticleSet> sets;
    ArrayAdd<ParticleSettings::ParticleSet>(set, sets);

    ParticleSettings::SetDefinition *definitions =
        reinterpret_cast<ParticleSettings::SetDefinition *>(ParticleSettingsRef::cur);
    uint32_t *setFlags = &definitions[(int) set].flags;

    int handle;
    if ((*setFlags & 1) != 0) {
        handle = addSpriteSystem(matrix, sets, flag);
    } else if ((*setFlags & 2) != 0) {
        handle = (int) addMeshSystem(matrix, sets, flag);
    } else {
        return -1;
    }

    if ((*setFlags & 0x01000000u) != 0)
        enableSystemUpdate(handle, false);
    return handle;
}

void ParticleSystemManager::init() {
    initSprites();
    initMesh();
    this->enabled = 1;
    update(0);
}

void ParticleSystemManager::resetSystem(int handle) {
    IParticleSystem *sys;
    if (handle == -1) {
        sys = nullptr;
    } else {
        IParticleSystem **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = reinterpret_cast<IParticleSystem **>(this->meshSystems);
            idx = handle & 0x3fffffff;
        } else {
            arr = reinterpret_cast<IParticleSystem **>(this->spriteSystems.data_);
            idx = handle;
        }
        sys = arr[idx];
    }
    if (sys != nullptr)
        sys->reset();
}

void ParticleSystemManager::renderMeshes() {
    if (this->meshTextureId != -1)
        ParticleSystemMesh::render(this->canvas, this->transformId);
    else if (this->spriteUvId != -1)
        ParticleSystemMesh::render(
            this->canvas, this->transformId, this->meshExtraId,
            static_cast<AbyssEngine::BlendMode>(this->meshBlendMode));
}

void ParticleSystemManager::renderPost3d() {
}

ParticleSystemManager::~ParticleSystemManager() {
    release();
    meshArray().~Array();
}
