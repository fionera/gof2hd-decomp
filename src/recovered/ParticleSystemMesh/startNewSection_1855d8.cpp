#include "class.h"

struct ParticleSystemMesh {
    void startNewSection();
};

void ParticleSystemMesh::startNewSection()
{
    u8(this, 0x90) = 1;
}
