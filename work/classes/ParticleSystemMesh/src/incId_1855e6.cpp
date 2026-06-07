#include "class.h"

struct ParticleSystemMesh {
    void incId();
};

void ParticleSystemMesh::incId()
{
    int id = i32(this, 0x50) + 1;
    if (id >= i32(this, 0x48))
        id = 0;
    i32(this, 0x50) = id;
}
