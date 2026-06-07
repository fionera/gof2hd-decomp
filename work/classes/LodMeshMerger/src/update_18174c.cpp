#include "class.h"

extern "C" uint8_t PaintCanvas_CameraIsSphereInViewFrustum(void *canvas, const Vector *center, float r);
extern "C" void aeabi_memcpy4(void *dst, const void *src, uint32_t n);  // __aeabi_memcpy4

void LodMeshMerger::update()
{
    int rows = i32(this, 0x0);
    for (int i = 0; i < rows; i++) {
        void *sph = ((void **)pp(this, 0x24))[i];
        uint8_t vis = PaintCanvas_CameraIsSphereInViewFrustum(
            pp(this, 0x14),
            (const Vector *)((char *)sph + 0x3c),
            *(float *)((char *)sph + 0x48));
        int8_t *visArr = (int8_t *)pp(this, 0x34);
        if (vis != (uint8_t)visArr[i]) {
            visArr[i] = (int8_t)vis;
            if (*((int8_t *)pp(this, 0x30) + i) != 0) {
                u8(this, 0x3c) = 1;
            }
        }
        rows = i32(this, 0x0);
    }

    if (u8(this, 0x3c) != 0) {
        int sumIdx = 0;   // r6 (index/triangle accumulator for the budget test)
        for (int j = 0; j < rows; j++) {
            if (*((int8_t *)pp(this, 0x30) + j) != 0 &&
                *((int8_t *)pp(this, 0x34) + j) != 0) {
                signed char lod = *((int8_t *)pp(this, 0x2c) + j);
                void *src = ((void **)pp(this, 0x24))[rows * lod + j];
                sumIdx += (uint16_t)*(uint16_t *)((char *)src + 0x28);
            }
        }
        for (int j = 0; sumIdx >= 0x10000 && j < rows; j++) {
            if (*((int8_t *)pp(this, 0x30) + j) != 0 &&
                *((int8_t *)pp(this, 0x34) + j) != 0) {
                signed char lod = *((int8_t *)pp(this, 0x2c) + j);
                if (lod < i32(this, 0x38) - 1) {
                    void *prev = ((void **)pp(this, 0x24))[rows * lod + j];
                    setLod(j, (signed char)(lod + 1));
                    rows = i32(this, 0x0);
                    signed char nlod = *((int8_t *)pp(this, 0x2c) + j);
                    void *cur = ((void **)pp(this, 0x24))[rows * nlod + j];
                    sumIdx = sumIdx - (uint16_t)*(uint16_t *)((char *)prev + 0x28)
                                    + (uint16_t)*(uint16_t *)((char *)cur + 0x28);
                }
            }
        }

        int vtxOff = 0;   // r5 (vertex accumulator)
        int idxOff = 0;   // r8 (index accumulator)
        for (int j = 0; j < rows; j++) {
            if (*((int8_t *)pp(this, 0x30) + j) != 0 &&
                *((int8_t *)pp(this, 0x34) + j) != 0) {
                uint8_t *out = (uint8_t *)pp(this, 0x20);
                uint8_t mask = out[0];
                signed char lod = *((int8_t *)pp(this, 0x2c) + j);
                void *src = ((void **)pp(this, 0x24))[rows * lod + j];

                if (mask & 1) {
                    aeabi_memcpy4(*(char **)(out + 4) + vtxOff * 0xc,
                                  *(void **)((char *)src + 4),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) * 0xc);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 4) {
                    aeabi_memcpy4(*(char **)(out + 0x10) + vtxOff * 0xc,
                                  *(void **)((char *)src + 0x10),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) * 0xc);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 8) {
                    aeabi_memcpy4(*(char **)(out + 0xc) + vtxOff * 0x10,
                                  *(void **)((char *)src + 0xc),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) << 4);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 2) {
                    aeabi_memcpy4(*(char **)(out + 8) + vtxOff * 8,
                                  *(void **)((char *)src + 8),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) << 3);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 0x10) {
                    int16_t *si = *(int16_t **)((char *)src + 0x2c);
                    int16_t *di = (int16_t *)(*(char **)(out + 0x2c) + idxOff * 2);
                    for (int k = -(int)(uint16_t)*(uint16_t *)((char *)src + 0x28); k != 0; k++) {
                        *di = (int16_t)(*si + (int16_t)vtxOff);
                        si++;
                        di++;
                    }
                }
                rows = i32(this, 0x0);
                idxOff += (uint16_t)*(uint16_t *)((char *)src + 0x28);
                vtxOff += (uint16_t)*(uint16_t *)((char *)src + 2);
            }
        }
        uint8_t *out = (uint8_t *)pp(this, 0x20);
        *(int16_t *)(out + 0x28) = (int16_t)idxOff;
        *(int16_t *)(out + 2) = (int16_t)vtxOff;
        u8(this, 0x3c) = 0;
    }
}
