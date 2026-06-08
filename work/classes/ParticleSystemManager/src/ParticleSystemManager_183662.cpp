#include "class.h"

// ParticleSystemManager::ParticleSystemManager(PaintCanvas*, CameraSet, unsigned short spriteTex,
//     BlendMode spriteBlend, bool spriteFlag, unsigned short meshTex, BlendMode meshBlend,
//     bool meshFlag)
// 8-argument overload that also takes explicit blend modes. Same field layout as the 6-arg ctor
// but the texture slots (+0x24/+0x48) are forced to 0xffff and the supplied texture goes to the
// "blend" half (+0x26/+0x4a), with the blend modes stored at +0x28/+0x4c.

extern "C" void _psm_arraySpriteCtor(void *arr);
extern "C" void _psm_arrayMeshCtor(void *arr);
extern "C" void _psm_construct(void *self);

extern "C" void *ParticleSystemManager_ctor8(
    void *self, void *canvas, int cameraSet, unsigned short spriteTex, int spriteBlend,
    bool spriteFlag, unsigned short meshTex, int meshBlend, bool meshFlag)
{
    i32(self, 0x0c) = cameraSet;
    pp(self, 0x04) = canvas;

    _psm_arraySpriteCtor((char *)self + 0x18);
    u16(self, 0x26) = spriteTex;
    u16(self, 0x24) = 0xffff;
    u32(self, 0x28) = (unsigned int)spriteBlend;
    u8(self, 0x38) = spriteFlag ? 1 : 0;

    _psm_arrayMeshCtor((char *)self + 0x3c);
    u16(self, 0x4a) = meshTex;
    u16(self, 0x48) = 0xffff;
    u32(self, 0x4c) = (unsigned int)meshBlend;
    u8(self, 0x60) = meshFlag ? 1 : 0;

    _psm_construct(self);
    return self;
}
