#include "class.h"

extern "C" uint8_t PaintCanvas_CameraIsSphereInViewFrustum(void *canvas, const Vector *center, float r);
extern "C" void aeabi_memcpy4(void *dst, const void *src, uint32_t n);  // __aeabi_memcpy4

void MeshMerger::update()
{
    int rows = this->f_0;
    for (int i = 0; i < rows; i++) {
        void *sph = ((void **)this->f_18)[i];
        uint8_t vis = PaintCanvas_CameraIsSphereInViewFrustum(
            this->f_c,
            (const Vector *)((char *)sph + 0x3c),
            *(float *)((char *)sph + 0x48));
        int8_t *visArr = (int8_t *)this->f_2c;
        if (vis != (uint8_t)visArr[i]) {
            visArr[i] = (int8_t)vis;
            if (*((int8_t *)this->f_28 + i) != 0) {
                this->f_34 = 1;
            }
        }
        rows = this->f_0;
    }

    if (this->f_34 != 0) {
        int idxOff = 0;   // iVar9 (index accumulator)
        int vtxOff = 0;   // iVar7 (vertex accumulator)
        for (int j = 0; j < rows; j++) {
            if (*((int8_t *)this->f_28 + j) != 0 &&
                *((int8_t *)this->f_2c + j) != 0) {
                uint8_t *out = (uint8_t *)this->f_20;
                uint8_t mask = out[0];
                signed char lod = *((int8_t *)this->f_24 + j);
                void *src = ((void **)this->f_18)[rows * lod + j];

                if (mask & 1) {
                    aeabi_memcpy4(*(char **)(out + 4) + vtxOff * 0xc,
                                  *(void **)((char *)src + 4),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) * 0xc);
                    out = (uint8_t *)this->f_20;
                    mask = out[0];
                }
                if (mask & 4) {
                    aeabi_memcpy4(*(char **)(out + 0x10) + vtxOff * 0xc,
                                  *(void **)((char *)src + 0x10),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) * 0xc);
                    out = (uint8_t *)this->f_20;
                    mask = out[0];
                }
                if (mask & 8) {
                    aeabi_memcpy4(*(char **)(out + 0xc) + vtxOff * 0x10,
                                  *(void **)((char *)src + 0xc),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) << 4);
                    out = (uint8_t *)this->f_20;
                    mask = out[0];
                }
                if (mask & 2) {
                    aeabi_memcpy4(*(char **)(out + 8) + vtxOff * 8,
                                  *(void **)((char *)src + 8),
                                  (uint32_t)*(uint16_t *)((char *)src + 2) << 3);
                    out = (uint8_t *)this->f_20;
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
                rows = this->f_0;
                idxOff += (uint16_t)*(uint16_t *)((char *)src + 0x28);
                vtxOff += (uint16_t)*(uint16_t *)((char *)src + 2);
            }
        }
        uint8_t *out = (uint8_t *)this->f_20;
        *(int16_t *)(out + 0x28) = (int16_t)idxOff;
        *(int16_t *)(out + 2) = (int16_t)vtxOff;
        this->f_34 = 0;
    }
}
