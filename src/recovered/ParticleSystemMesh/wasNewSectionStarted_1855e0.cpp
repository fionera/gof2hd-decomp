#include "class.h"

struct ParticleSystemMesh {
    uint8_t wasNewSectionStarted();
};

uint8_t ParticleSystemMesh::wasNewSectionStarted()
{
    return u8(this, 0x90);
}
