#include "class.h"

extern "C" void _psm_mesh_base_dtor(ParticleSystemMesh *self);
extern "C" uint32_t g_ParticleSystemMesh_vtable[];

extern "C" void _ZN18ParticleSystemMeshD1Ev(ParticleSystemMesh *self)
{
    *(uint32_t **)self = g_ParticleSystemMesh_vtable + 2;
    return _psm_mesh_base_dtor(self);
}
