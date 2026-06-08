#include "class.h"

// AEGeometry::setMesh(unsigned short)
//   r3=this; r1=*(this+0xc) [pre-indexed, r3->this+0xc]; r0=*(this+0x2c)
//   if (*(this+0xc)!=0)  ext1(canvas, *(this+0xc), mesh, 0)
//   else                 ext2(canvas, mesh, &this+0xc)
extern "C" void _ae_setmesh_a(uint32_t canvas, uint32_t tf, uint32_t mesh, int z);
extern "C" void _ae_setmesh_b(uint32_t canvas, uint32_t mesh, uint32_t *tfp);

struct AEGeometry { void setMesh(uint16_t mesh); };
void AEGeometry::setMesh(uint16_t mesh)
{
    uint32_t *tfp = &this->f_c;
    uint32_t canvas = tfp[(0x2c - 0xc) / 4];
    if (*tfp != 0)
        return _ae_setmesh_a(canvas, *tfp, mesh, 0);
    return _ae_setmesh_b(canvas, mesh, tfp);
}
