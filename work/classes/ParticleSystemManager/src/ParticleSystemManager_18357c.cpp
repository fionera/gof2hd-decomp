#include "class.h"

// ParticleSystemManager::ParticleSystemManager(PaintCanvas*, CameraSet, unsigned short spriteTex,
//     bool spriteFlag, unsigned short meshTex, bool meshFlag)
// Stores the canvas and camera set, default-constructs the sprite (+0x18) and mesh (+0x3c)
// system arrays, seeds their texture/blend slots (the "blend" half left at 0xffff here), records
// the per-array flags at +0x38 / +0x60, then runs construct() to reset the runtime state.

extern "C" void _psm_arraySpriteCtor(void *arr);  // Array<ParticleSystemSprite*>::Array()
extern "C" void _psm_arrayMeshCtor(void *arr);     // Array<ParticleSystemMesh*>::Array()
extern "C" void _psm_construct(void *self);

extern "C" void *ParticleSystemManager_ctor6(
    void *self, void *canvas, int cameraSet, unsigned short spriteTex, bool spriteFlag,
    unsigned short meshTex, bool meshFlag)
{
    i32(self, 0x0c) = cameraSet;
    pp(self, 0x04) = canvas;

    _psm_arraySpriteCtor((char *)self + 0x18);
    u16(self, 0x26) = 0xffff;
    u16(self, 0x24) = spriteTex;
    u32(self, 0x28) = 0;
    u8(self, 0x38) = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor((char *)self + 0x3c);
    u16(self, 0x4a) = 0xffff;
    u16(self, 0x48) = meshTex;
    u32(self, 0x4c) = 0;
    u8(self, 0x60) = meshFlag ? 1 : 0;

    _psm_construct(self);
    return self;
}
