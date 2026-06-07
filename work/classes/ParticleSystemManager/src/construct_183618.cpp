#include "class.h"

// ParticleSystemManager::construct()
struct ParticleSystemManager { void construct(); };
void ParticleSystemManager::construct()
{
    u32(this, 0x10) = 0;
    u8(this, 0x14) = 0;
    u32(this, 0x2c) = 0xffffffff;
    u32(this, 0x30) = 0xffffffff;
    u32(this, 0x34) = 0;
    u32(this, 0x54) = 0xffffffff;
    u32(this, 0x58) = 0xffffffff;
    u32(this, 0x5c) = 0;
    u16(this, 0x0) = 0x101;
}
