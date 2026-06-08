#include "class.h"

// ParticleSystemManager::addMeshSystem(AEMath::Matrix const*, Array<ParticleSet> const&, bool)
// Allocates a ParticleSystemMesh (0xa0 bytes), constructs it against the manager's canvas
// (+0x04) and the mesh flag (+0x60), appends it to the mesh array (+0x3c), accumulates its
// particle count (virtual call, vtable+0x10) into +0x5c, and returns the new system's handle:
// the mesh-array index with bit 0x4000 set to mark it as a mesh-array handle.

void *operator new(uint32_t size);

extern "C" void *_psmesh_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                              bool b4, bool b5);                    // ParticleSystemMesh ctor
extern "C" void _psm_arrayMeshAdd(void *sys, void *arr);           // ArrayAdd<ParticleSystemMesh*>

struct ParticleSystemManager {
    unsigned int addMeshSystem(const void *matrix, const void *sets, bool flag);
};
unsigned int ParticleSystemManager::addMeshSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = operator new(0xa0);
    _psmesh_ctor(sys, pp(this, 0x04), matrix, sets, flag, u8(this, 0x60) != 0);
    _psm_arrayMeshAdd(sys, (char *)this + 0x3c);

    // getParticleCount lives at vtable offset 0x10 for the mesh system.
    typedef int (*pfn)(void *);
    int count = ((pfn *)*(void **)sys)[4](sys);
    i32(this, 0x5c) = count + i32(this, 0x5c);

    return (unsigned int)(i32(this, 0x3c) - 1) | 0x4000;
}
