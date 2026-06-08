#include "class.h"

extern "C" void *MeshMerger_transformMesh(void *mesh, const Matrix &m);   // 0x77e00
extern "C" void PaintCanvas_MeshCreate2(void *canvas, uint16_t nv, uint16_t ni, int flags); // 0x75da8
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);   // 0x72370
extern "C" void PaintCanvas_TransformCreate(void *canvas, uint32_t *out); // 0x720ac
extern "C" void PaintCanvas_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh); // 0x73030
extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);                // 0x6ec2c (__aeabi_uidiv)
extern "C" int MeshMerger_init_tail(MeshMerger *self, int r1, uint16_t flags, uint32_t *meshId); // 0x1ac6c8

int MeshMerger::init()
{
    if (this->f_6 != 0) {
        return this->f_6;
    }

    int rows;
    for (int i = 0; i < (rows = this->f_0); i++) {
        int8_t *lods = (int8_t *)this->f_24;
        int lod = lods[i];
        if (lod >= -1 && this->f_30 <= lod) {
            lods[i] = 0;
        }
        for (int c = 0; c < this->f_30; c++) {
            void *mesh = ((void **)this->f_8)[this->f_0 * c + i];
            if (mesh != 0) {
                void *t = MeshMerger_transformMesh(
                    mesh, *(const Matrix *)((char *)this->f_1c + i * 0x3c));
                ((void **)this->f_18)[this->f_0 * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        void *m0 = ((void **)this->f_8)[i];
        uint16_t v = *(uint16_t *)((char *)m0 + 2);
        uint16_t idiv = aeabi_uidiv16(*(uint16_t *)((char *)m0 + 0x28), 3);
        ni = ni + idiv;
        nv = nv + v;
    }

    uint16_t flags = this->f_4;
    PaintCanvas_MeshCreate2(this->f_c, nv, ni,
                            (int)*(int8_t *)*(void **)this->f_8);
    void *ptr = PaintCanvas_MeshGetPointer(this->f_c, this->f_10);
    this->f_20 = (int)ptr;
    PaintCanvas_TransformCreate(this->f_c, (uint32_t *)((char *)this + 0x14));
    PaintCanvas_TransformAddMeshId(this->f_c, this->f_14, this->f_10);
    this->f_34 = 1;
    return MeshMerger_init_tail(this, 0, flags, (uint32_t *)((char *)this + 0x10));
}
