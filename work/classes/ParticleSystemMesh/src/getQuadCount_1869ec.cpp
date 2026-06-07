#include "class.h"

struct ParticleSystemMesh {
    int getQuadCount();
};

int ParticleSystemMesh::getQuadCount()
{
    return i32(this, 0x70) >> 2;
}
