#include "class.h"

extern "C" void *operator_new(uint32_t size);                       // 0x6eb24
extern "C" void *operator_new_array(uint32_t size);                 // 0x6ec08
extern "C" void AEMath_MatrixTransformVector(Vector *out, const Vector *v); // 0x6f688
extern "C" void AEMath_MatrixRotateVector(Vector *out, const Vector *v);    // 0x6f694
extern "C" void AEMath_VectorNormalize(void *out, const Vector *v);         // 0x6ec80
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);       // 0x6eb3c
extern "C" void AEMath_BSphere_assign(void *dst, const void *src);          // 0x6eb18

void *MeshMerger::transformMesh(Mesh *mesh, const Matrix &m)
{
    char *out = (char *)operator_new(0x88);
    // Zero/identity initialize the new Mesh header.
    *(uint32_t *)(out + 0x82) = 0;
    *(uint32_t *)(out + 0x7e) = 0;
    out[0x38] = 0;
    *(uint32_t *)(out + 0x4c) = 0x3f800000;
    *(uint32_t *)(out + 0x50) = 0;
    *(uint32_t *)(out + 0x54) = 0;
    *(uint32_t *)(out + 0x59) = 0;
    __builtin_memset(out + 0x70, 0, 16);
    __builtin_memset(out + 0x60, 0, 16);
    __builtin_memset(out + 0x28, 0, 16);
    __builtin_memset(out + 0x14, 0, 16);
    __builtin_memset(out + 0x4, 0, 16);
    __builtin_memset(out + 0x3c, 0, 16);
    *(uint32_t *)(out + 0x55) = 0;

    uint16_t nv = *(uint16_t *)((char *)mesh + 2);
    *(uint16_t *)(out + 2) = nv;
    *(uint16_t *)(out + 0x28) = *(uint16_t *)((char *)mesh + 0x28);
    uint8_t flags = *(uint8_t *)mesh;
    *(uint8_t *)out = flags;

    if (flags & 2) {
        *(uint32_t *)(out + 8) = *(uint32_t *)((char *)mesh + 8);
    }
    if (flags & 8) {
        *(uint32_t *)(out + 0xc) = *(uint32_t *)((char *)mesh + 0xc);
    }
    if (flags & 0x10) {
        *(uint32_t *)(out + 0x2c) = *(uint32_t *)((char *)mesh + 0x2c);
    }
    if (flags & 1) {
        void *buf = operator_new_array((uint32_t)nv * 0xc);
        *(void **)(out + 4) = buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector tmp;
            AEMath_MatrixTransformVector(&tmp, (const Vector *)&m);
            AEMath_Vector_assign((Vector *)(*(char **)(out + 4) + off), &tmp);
            off += 0xc;
            nv = *(uint16_t *)((char *)mesh + 2);
        }
        flags = *(uint8_t *)mesh;
    }
    if (flags & 4) {
        void *buf = operator_new_array((uint32_t)nv * 0xc);
        *(void **)(out + 0x10) = buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector rot;
            Vector nrm;
            AEMath_MatrixRotateVector(&rot, (const Vector *)&m);
            AEMath_VectorNormalize(&nrm, &rot);
            AEMath_Vector_assign((Vector *)(*(char **)(out + 0x10) + off), &nrm);
            off += 0xc;
            nv = *(uint16_t *)((char *)mesh + 2);
        }
    }

    // Build the bounding sphere from the transformed center + original radius.
    Vector center;
    AEMath_MatrixTransformVector(&center, (const Vector *)&m);
    float bs[4];
    bs[0] = center.x;
    bs[1] = center.y;
    bs[2] = center.z;
    bs[3] = 1.0f;
    *(uint32_t *)&bs[2] = *(uint32_t *)((char *)mesh + 0x48);  // radius overrides z slot? matches uStack_44
    AEMath_BSphere_assign(out + 0x3c, bs);
    return out;
}
