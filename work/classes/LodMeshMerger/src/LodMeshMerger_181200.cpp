#include "class.h"

extern "C" void *operator_new_array(uint32_t size);              // 0x6ec08
extern "C" void ArrayMesh_ctor(void *a);                         // 0x6f724  Array<Mesh*>::Array()
extern "C" void ArraySetLengthMesh(uint32_t n, void *a);         // 0x78814  ArraySetLength<Mesh*>
extern "C" void aeabi_memclr4(void *p, uint32_t n);              // 0x6ec14
extern "C" void aeabi_memclr(void *p, uint32_t n);              // 0x6ec20

inline void *operator new(unsigned int, void *p) { return p; }

// LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
{
    ArrayMesh_ctor((char *)this + 8);
    this->f_20 = 0;
    this->f_24 = 0;
    this->f_28 = 0;
    this->f_2c = 0;
    this->f_14 = canvas;
    this->f_0 = rows;
    this->f_38 = cols;
    ArraySetLengthMesh((uint32_t)(cols * rows), (char *)this + 8);

    uint32_t n = this->f_0;
    void *slots = operator_new_array(n * cols * 4);
    this->f_24 = slots;
    aeabi_memclr4(slots, n * cols * 4);

    void *lods = operator_new_array(n | ((int)n >> 31));
    this->f_2c = lods;
    aeabi_memclr(lods, n);

    char *mats = (char *)operator_new_array(n * 0x3c);
    for (uint32_t off = 0; off != n * 0x3c; off += 0x3c) {
        new ((Matrix *)(mats + off)) Matrix();
    }
    this->f_28 = mats;

    for (int i = 0; i < (int)this->f_0; i++) {
        Matrix tmp;
        tmp.m[0] = 1.0f;
        tmp.m[1] = 0.0f; tmp.m[2] = 0.0f; tmp.m[3] = 0.0f;
        tmp.m[4] = 1.0f;
        tmp.m[5] = 0.0f; tmp.m[6] = 0.0f; tmp.m[7] = 0.0f; tmp.m[8] = 0.0f;
        tmp.m[9] = 0.0f;
        tmp.m[10] = 1.0f;
        tmp.m[11] = 0.0f; tmp.m[12] = 0.0f; tmp.m[13] = 0.0f;
        tmp.m[14] = 1.0f;
        *(Matrix *)((char *)this->f_28 + i * 0x3c) = tmp;
    }

    uint32_t rn = this->f_0;
    uint32_t signedN = rn | ((int)rn >> 31);
    char *enabled = (char *)operator_new_array(signedN);
    this->f_30 = enabled;
    for (int i = 0; i < (int)rn; i++) enabled[i] = 1;

    char *visible = (char *)operator_new_array(signedN);
    this->f_34 = visible;
    for (int i = 0; i < (int)rn; i++) visible[i] = 1;

    this->f_3c = 0;
    this->f_4 = flags;
    this->f_6 = 0;
}
