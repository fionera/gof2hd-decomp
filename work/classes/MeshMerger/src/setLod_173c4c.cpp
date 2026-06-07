#include "class.h"

void MeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)pp(this, 0x24);
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)pp(this, 0x28) + index) == 0) {
            return;
        }
        u8(this, 0x34) = 1;
    }
}
