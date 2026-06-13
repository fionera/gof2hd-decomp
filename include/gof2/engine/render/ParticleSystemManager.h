#ifndef GOF2_PARTICLESYSTEMMANAGER_H
#define GOF2_PARTICLESYSTEMMANAGER_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- ParticleSystemManager (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "ParticleSystemManager::..." (the class is NOT in a
// namespace; only its argument types like AbyssEngine::AEMath::Matrix are).
//
// Layout recovered from the constructor (0x0019357c) and per-method field usage. The sprite and
// mesh sub-system arrays are inline 3-word arrays (count / data / capacity), default-constructed
// by Array<ParticleSystemSprite*>::Array() / Array<ParticleSystemMesh*>::Array() against the
// address of their first word.

void *operator new(__SIZE_TYPE__ size);

class ParticleSystemManager {
public:
    uint16_t flags;                  // +0x0   low byte = sprite-active, high byte = mesh-active
    uint8_t  pad_0x2[2];             // +0x2
    void*    canvas;                 // +0x4   PaintCanvas*
    uint8_t  pad_0x8[4];             // +0x8
    int32_t  cameraSet;              // +0xc   ParticleSettings::CameraSet
    int32_t  accumulatedDt;          // +0x10  accumulated update delta
    uint8_t  enabled;                // +0x14  manager active/enabled flag
    uint8_t  pad_0x15[3];            // +0x15
    uint32_t spriteSystemCount;      // +0x18  sprite array: element count
    void*    spriteSystems;          // +0x1c  sprite array: data pointer (IParticleSystem*[])
    uint32_t spriteSystemCapacity;   // +0x20  sprite array: capacity
    int16_t  spriteTextureId;        // +0x24  sprite texture id (0xffff = create from uv id)
    int16_t  spriteUvId;             // +0x26  sprite uv/texture-source id
    uint32_t spriteBlendMode;        // +0x28  sprite blend mode
    uint32_t spriteMeshId;           // +0x2c  sprite render handle (init 0xffffffff)
    uint32_t spriteSystemId;         // +0x30  created sprite-system handle (init 0xffffffff)
    uint32_t spriteParticleCount;    // +0x34  accumulated sprite particle count
    uint8_t  spriteUsesExtra;        // +0x38  sprite extra/mirror flag
    uint8_t  pad_0x39[3];            // +0x39
    uint32_t meshSystemCount;        // +0x3c  mesh array: element count
    void*    meshSystems;            // +0x40  mesh array: data pointer (IParticleSystem*[])
    uint32_t meshSystemCapacity;     // +0x44  mesh array: capacity
    int16_t  meshTextureId;          // +0x48  mesh texture id (0xffff = create from uv id)
    int16_t  meshUvId;               // +0x4a  mesh uv/texture-source id
    uint32_t meshBlendMode;          // +0x4c  mesh blend mode
    uint32_t meshExtraId;            // +0x50  mesh render handle
    uint32_t meshId;                 // +0x54  created particle-mesh handle (init 0xffffffff)
    uint32_t transformId;            // +0x58  created transform handle (init 0xffffffff)
    uint32_t meshParticleCount;      // +0x5c  packed mesh vertex/index particle count
    uint8_t  meshUsesExtra;          // +0x60  mesh extra/mirror flag

    void update(long long dt);
    void reset();
    void releaseSprites();
    void construct();
    void render3d();
    void setParticleSetByIndex(int handle, unsigned char setIndex);
    void enableSystemRender(int handle, bool enable);
    void release();
    void cameraToggle(int cam);
    unsigned int addMeshSystem(const void *matrix, const void *sets, bool flag);
    unsigned long long emitManual(int handle, const float *pos, int ret, float p4);
    void renderSprites();
    void systemSetMatrix(int handle, const void *matrix);
    void setParticleSetBySet(unsigned int handle, unsigned int set);
    void enableSystemUpdate(int handle, bool enable);
    void initSprites();
    int  addSpriteSystem(const void *matrix, const void *sets, bool flag);
    void initMesh();
    void enableSystemEmit(int handle, bool enable);
    void enableSystemEmit2(int handle, bool enable);
    void enableSystemEmit3(int handle, bool enable);
    void attachSystem(int handle, bool enable);
    int  addSystem(const void *matrix, unsigned int set, bool flag);
    int  init();
    void resetSystem(int handle);
    void renderMeshes();
};
#endif
