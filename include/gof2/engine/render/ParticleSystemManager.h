#ifndef GOF2_PARTICLESYSTEMMANAGER_H
#define GOF2_PARTICLESYSTEMMANAGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ParticleSystemManager (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "ParticleSystemManager::..." (the class is NOT in a
// namespace; only its argument types like AbyssEngine::AEMath::Matrix are).
//
// Layout recovered from the constructor (0x0019357c) and per-method field usage:
//   0x00 u16 state/flags word (low byte = active marker, high byte = secondary flag)
//   0x04 PaintCanvas*           (render target / paint canvas)
//   0x0c ParticleSettings::CameraSet (camera set selector)
//   0x10 int accumulated dt
//   0x14 u8 enabled flag
//   0x18 Array<ParticleSystemSprite*> : 0x18 size, 0x1c data, 0x20 capacity
//   0x24 i16 sprite texture id
//   0x26 i16 sprite uv id
//   0x28/0x2c/0x30 sprite render params (count/uv/size handles)
//   0x34 u32 misc
//   0x38 u8 sprite-uses-extra flag
//   0x3c Array<ParticleSystemMesh*> : 0x3c size, 0x40 data, 0x44 capacity
//   0x48 i16 mesh texture id
//   0x4a i16 mesh uv id
//   0x4c/0x50/0x54/0x58 mesh render params (mesh/transform handles)
//   0x5c u32 mesh vertex/index packed count
//   0x60 u8 mesh-uses-extra flag

void *operator new(__SIZE_TYPE__ size);

class ParticleSystemManager {
public:
    uint16_t field_0x0;                 // +0x0   state/flags word
    uint8_t  pad_0x2[2];                // +0x2
    void*    field_0x4;                 // +0x4   PaintCanvas*
    uint8_t  pad_0x8[4];                // +0x8
    int32_t  field_0xc;                 // +0xc   CameraSet
    int32_t  field_0x10;                // +0x10  accumulated dt
    uint8_t  field_0x14;                // +0x14  enabled flag
    uint8_t  pad_0x15[3];               // +0x15
    uint32_t field_0x18;                // +0x18  sprite array size
    void*    field_0x1c;                // +0x1c  sprite array data
    uint32_t field_0x20;                // +0x20  sprite array capacity
    int16_t  field_0x24;                // +0x24  sprite texture id
    int16_t  field_0x26;                // +0x26  sprite uv id
    uint32_t field_0x28;                // +0x28
    uint32_t field_0x2c;                // +0x2c
    uint32_t field_0x30;                // +0x30
    uint32_t field_0x34;                // +0x34
    uint8_t  field_0x38;                // +0x38  sprite-uses-extra flag
    uint8_t  pad_0x39[3];               // +0x39
    uint32_t field_0x3c;                // +0x3c  mesh array size
    void*    field_0x40;                // +0x40  mesh array data
    uint32_t field_0x44;                // +0x44  mesh array capacity
    int16_t  field_0x48;                // +0x48  mesh texture id
    int16_t  field_0x4a;                // +0x4a  mesh uv id
    uint32_t field_0x4c;                // +0x4c
    uint32_t field_0x50;                // +0x50
    uint32_t field_0x54;                // +0x54
    uint32_t field_0x58;                // +0x58
    uint32_t field_0x5c;                // +0x5c  mesh vertex/index packed count
    uint8_t  field_0x60;                // +0x60  mesh-uses-extra flag

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
