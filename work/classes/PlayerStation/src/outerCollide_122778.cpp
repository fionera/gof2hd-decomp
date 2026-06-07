#include "class.h"

typedef bool (*BoundingVolumeCollideFn)(void *volume, uint32_t x, uint32_t y, uint32_t z);

bool PlayerStation::outerCollide(float x, float y, float z)
{
    uint32_t rawX = F<uint32_t>(&x, 0);
    uint32_t rawY = F<uint32_t>(&y, 0);
    uint32_t rawZ = F<uint32_t>(&z, 0);
    float radius = (float)F<int32_t>(this, 0x154);

    if (x < F<float>(this, 0x58) + radius) {
        if (F<float>(this, 0x58) - radius < x) {
            if (y < F<float>(this, 0x5c) + radius) {
                if (F<float>(this, 0x5c) - radius < y) {
                    if (z < F<float>(this, 0x60) + radius) {
                        if (F<float>(this, 0x60) - radius < z) {
                            void *volumes = P(this, 0x130);
                            if (volumes != 0) {
                                for (uint32_t i = 0; i < F<uint32_t>(volumes, 0x0);
                                     ++i) {
                                    void *volume =
                                        *(void **)((char *)P(volumes, 0x4) + i * 4);
                                    BoundingVolumeCollideFn fn =
                                        *(BoundingVolumeCollideFn *)((char *)P(volume, 0x0) +
                                                                    0xc);
                                    if (fn(volume, rawX, rawY, rawZ)) {
                                        F<uint32_t>(this, 0x150) = i;
                                        return true;
                                    }
                                    volumes = P(this, 0x130);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}
