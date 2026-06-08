#include "class.h"

void LodMeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)pp(this, 0x2c);
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)pp(this, 0x30) + index) == 0) {
            return;
        }
        u8(this, 0x3c) = 1;
    }
}
