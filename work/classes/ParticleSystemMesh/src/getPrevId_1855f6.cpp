#include "class.h"

struct ParticleSystemMesh {
    int getPrevId(int id);
};

int ParticleSystemMesh::getPrevId(int id)
{
    if (id == 0)
        id = i32(this, 0x48);
    return id - 1;
}
