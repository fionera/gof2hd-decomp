#include "class.h"

extern "C" void *LodMeshMerger_transformMesh(void *mesh, const Matrix &m);            // 0x7882c
extern "C" void PaintCanvas_MeshCreate2(void *canvas, uint16_t nv, uint16_t ni, int flags); // 0x75da8
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);               // 0x72370
extern "C" void PaintCanvas_TransformCreate(void *canvas, uint32_t *out);             // 0x720ac
extern "C" void PaintCanvas_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh); // 0x73030
extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);                            // 0x6ec2c (__aeabi_uidiv)
extern "C" int LodMeshMerger_init_tail(LodMeshMerger *self, int r1, uint16_t flags, uint32_t *meshId); // 0x1ac878

int LodMeshMerger::init()
{
    if (u8(this, 0x6) != 0) {
        return u8(this, 0x6);
    }

    int rows;
    for (int i = 0; i < (rows = i32(this, 0x0)); i++) {
        int8_t *lods = (int8_t *)pp(this, 0x2c);
        int lod = lods[i];
        if (lod >= -1 && i32(this, 0x38) <= lod) {
            lods[i] = 0;
        }
        for (int c = 0; c < i32(this, 0x38); c++) {
            void *mesh = ((void **)pp(this, 0xc))[i32(this, 0x0) * c + i];
            if (mesh != 0) {
                void *t = LodMeshMerger_transformMesh(
                    mesh, *(const Matrix *)((char *)pp(this, 0x28) + i * 0x3c));
                ((void **)pp(this, 0x24))[i32(this, 0x0) * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        void *m0 = ((void **)pp(this, 0xc))[i];
        uint16_t v = *(uint16_t *)((char *)m0 + 2);
        uint16_t idiv = aeabi_uidiv16(*(uint16_t *)((char *)m0 + 0x28), 3);
        ni = ni + idiv;
        nv = nv + v;
    }

    uint16_t flags = u16(this, 0x4);
    PaintCanvas_MeshCreate2(pp(this, 0x14), nv, ni,
                            (int)*(int8_t *)*(void **)pp(this, 0xc));
    void *ptr = PaintCanvas_MeshGetPointer(pp(this, 0x14), u32(this, 0x18));
    i32(this, 0x20) = (int)ptr;
    PaintCanvas_TransformCreate(pp(this, 0x14), (uint32_t *)((char *)this + 0x1c));
    PaintCanvas_TransformAddMeshId(pp(this, 0x14), u32(this, 0x1c), u32(this, 0x18));
    u8(this, 0x3c) = 1;
    return LodMeshMerger_init_tail(this, 0, flags, (uint32_t *)((char *)this + 0x18));
}
