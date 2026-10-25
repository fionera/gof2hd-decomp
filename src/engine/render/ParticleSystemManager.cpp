

#include "engine/core/Array.h"
#include "engine/render/ParticleSettingsRef.h"

#include "engine/render/ParticleSystemMesh.h"
#define GOF2_ENUM_BlendMode

#include "engine/render/ParticleSystemManager.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSystemSprite.h"
#include "engine/render/PaintCanvas.h"

ParticleSystemManager::ParticleSystemManager(
    PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet, unsigned short spriteTex,
    bool spriteFlag, unsigned short meshTex, bool meshFlag)
    : canvas(canvas), cameraSet(cameraSet),
      spriteTextureId(spriteTex), spriteUvId(0xffff), spriteBlendMode(0), spriteUsesExtra(spriteFlag),
      meshTextureId(meshTex), meshUvId(0xffff), meshBlendMode(0), meshUsesExtra(meshFlag) {
    construct();
}

ParticleSystemManager::ParticleSystemManager(
    PaintCanvas *canvas, ParticleSettings::CameraSet cameraSet, unsigned short spriteTex,
    AbyssEngine::BlendMode spriteBlend, bool spriteFlag,
    unsigned short meshTex, AbyssEngine::BlendMode meshBlend, bool meshFlag)
    : canvas(canvas), cameraSet(cameraSet),
      spriteTextureId(0xffff), spriteUvId(spriteTex), spriteBlendMode(spriteBlend), spriteUsesExtra(spriteFlag),
      meshTextureId(0xffff), meshUvId(meshTex), meshBlendMode(meshBlend), meshUsesExtra(meshFlag) {
    construct();
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
    this->flags = 1;
    this->flagsHigh = 1;
}

void ParticleSystemManager::init() {
    initSprites();
    initMesh();
    this->enabled = 1;
    update(0);
}

void ParticleSystemManager::initSprites() {
    if (this->spriteSystems.count == 0)
        return;

    this->spriteSystemId = 0xffffffff;
    if (this->cameraSet == 0)
        return;

    if (this->spriteTextureId == -1) {
        if (this->spriteUvId != -1) {
            this->canvas->SpriteSystemCreate((unsigned short) this->spriteParticleCount, false,
                                             this->spriteSystemId);
            this->canvas->TextureCreate(this->spriteUvId, this->spriteMeshId, false);
        }
    } else {
        this->canvas->SpriteSystemCreate((unsigned short) this->spriteParticleCount, false,
                                         this->spriteTextureId, this->spriteSystemId);
    }

    this->canvas->SpriteSystemSetAllSize(this->spriteSystemId, 0);

    const ParticleSettings::SetDefinition &definition = ParticleSettingsRef::cur.sets[0];
    this->canvas->SpriteSystemSetAllUv(this->spriteSystemId, definition.uvU0, definition.uvV0,
                                       definition.uvU1, definition.uvV1);

    for (unsigned i = 0, offset = 0; i < this->spriteSystems.count; ++i) {
        IParticleSystem *sys = this->spriteSystems[i];
        sys->init(this->spriteSystemId, offset);
        offset += this->spriteSystems[i]->getParticleCount();
    }
}

void ParticleSystemManager::initMesh() {
    if (this->meshSystems.count == 0)
        return;

    this->meshId = 0xffffffff;
    this->transformId = 0xffffffff;

    if (this->meshTextureId == -1) {
        if (this->meshUvId != -1) {
            this->canvas->MeshCreate((unsigned short) (this->meshParticleCount * 4),
                                     (unsigned short) (this->meshParticleCount * 2), 0x1b, this->meshId);
            this->canvas->TextureCreate(this->meshUvId, this->meshExtraId, false);
        }
    } else {
        this->canvas->MeshCreate((unsigned short) (this->meshParticleCount * 4),
                                 (unsigned short) (this->meshParticleCount * 2), 0x1b,
                                 this->meshTextureId, this->meshId);
    }

    this->canvas->TransformCreate(this->transformId);
    this->canvas->TransformAddMeshId(this->transformId, this->meshId);

    for (unsigned i = 0, offset = 0; i < this->meshSystems.count; ++i) {
        IParticleSystem *sys = this->meshSystems[i];
        sys->init(this->meshId, offset);
        offset += this->meshSystems[i]->getQuadCount() * 4;
    }
}

void ParticleSystemManager::update(long long dt) {
    if (this->enabled == 0)
        return;
    int d = (int) dt;
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

    for (unsigned i = 0; i < this->meshSystems.count; i++) {
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

int ParticleSystemManager::addSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                     ParticleSettings::ParticleSet set, bool flag) {
    Array<ParticleSettings::ParticleSet> sets;
    ArrayAdd(set, sets);

    ParticleSettings::SetDefinition &definition = ParticleSettingsRef::cur.sets[(int) set];

    int handle;
    if ((definition.flags & 1) != 0) {
        handle = addSpriteSystem(matrix, sets, flag);
    } else if ((definition.flags & 2) != 0) {
        handle = (int) addMeshSystem(matrix, sets, flag);
    } else {
        return -1;
    }

    if ((definition.flags & 0x01000000u) != 0)
        enableSystemUpdate(handle, false);
    return handle;
}

int ParticleSystemManager::addSpriteSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                           Array<ParticleSettings::ParticleSet> const &sets, bool flag) {
    ParticleSystemSprite *sys =
        new ParticleSystemSprite(this->canvas, matrix, sets, flag, this->spriteUsesExtra);
    ArrayAdd(sys, this->spriteSystems);
    this->spriteParticleCount += sys->getParticleCount();
    return this->spriteSystems.count - 1;
}

unsigned int ParticleSystemManager::addMeshSystem(AbyssEngine::AEMath::Matrix const *matrix,
                                                  Array<ParticleSettings::ParticleSet> const &sets,
                                                  bool flag) {
    ParticleSystemMesh *sys =
        new ParticleSystemMesh(this->canvas, matrix, sets, flag, this->meshUsesExtra);
    ArrayAdd(sys, this->meshSystems);

    this->meshParticleCount += sys->getQuadCount();

    return (this->meshSystems.count - 1) | 0x4000;
}

void ParticleSystemManager::enableSystemUpdate(int handle, bool enable) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->enableUpdate(enable);
}

void ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->setParticleSetIndex(setIndex);
}

void ParticleSystemManager::setParticleSetBySet(int handle, ParticleSettings::ParticleSet set) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->setParticleSet(set);
}

void ParticleSystemManager::enableSystemEmit(int handle, bool enable) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->enableEmit(enable);
}

void ParticleSystemManager::enableSystemRender(int handle, bool enable) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->enableRender(enable);
}

void ParticleSystemManager::resetSystem(int handle) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->reset();
}

ParticleSystemManager::~ParticleSystemManager() {
    release();
}

void ParticleSystemManager::release() {
    releaseSprites();
    this->canvas = nullptr;
    ArrayReleaseClasses(this->meshSystems);
}

void ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet cam) {
    if (this->cameraSet == cam)
        return;
    this->cameraSet = cam;
    releaseSprites();
    initSprites();
}

void ParticleSystemManager::releaseSprites() {
    ArrayReleaseClasses(this->spriteSystems);
    if (this->spriteSystemId != 0xffffffff) {
        this->canvas->ReleaseSpriteSystemResource(this->spriteSystemId);
        this->spriteSystemId = 0xffffffff;
    }
}

void ParticleSystemManager::render3d() {
    if (this->enabled == 0)
        return;
    if (this->flagsHigh != 0)
        renderMeshes();
    if (this->flags != 0)
        renderSprites();
}

void ParticleSystemManager::renderMeshes() {
    if (this->meshTextureId != -1)
        ParticleSystemMesh::render(this->canvas, this->transformId);
    else if (this->spriteUvId != -1)
        ParticleSystemMesh::render(
            this->canvas, this->transformId, this->meshExtraId,
            static_cast<AbyssEngine::BlendMode>(this->meshBlendMode));
}

void ParticleSystemManager::renderSprites() {
    if (this->spriteTextureId != -1)
        ParticleSystemSprite::render(this->canvas, this->spriteSystemId);
    else if (this->spriteUvId != -1)
        ParticleSystemSprite::render(
            this->canvas, this->spriteSystemId, this->spriteMeshId,
            static_cast<AbyssEngine::BlendMode>(this->spriteBlendMode));
}

void ParticleSystemManager::renderPost3d() {
}

void ParticleSystemManager::reset() {
    for (unsigned i = 0; i < this->spriteSystems.count; i++) {
        IParticleSystem *p = this->spriteSystems[i];
        if (p != nullptr)
            p->reset();
    }
    for (unsigned i = 0; i < this->meshSystems.count; i++) {
        IParticleSystem *p = this->meshSystems[i];
        if (p != nullptr)
            p->reset();
    }
}

void ParticleSystemManager::emitManual(int handle, AbyssEngine::AEMath::Vector const &pos,
                                                     int ret, AbyssEngine::AEMath::Vector const &velocity,
                                                     float p5) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->emitManual(pos, ret, &velocity, p5);
}

void ParticleSystemManager::emitManual(int handle, AbyssEngine::AEMath::Vector const &pos,
                                                     int ret, float p4) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->emitManual(pos, ret, nullptr, p4);
}

void ParticleSystemManager::systemSetMatrix(int handle, AbyssEngine::AEMath::Matrix const *matrix) {
    if (handle == -1)
        return;
    IParticleSystem *sys = getSystem(handle);
    sys->setMatrix(matrix);
}
