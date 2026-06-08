#include "class.h"

void LodMeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)this->f_2c;
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)this->f_30 + index) == 0) {
            return;
        }
        this->f_3c = 1;
    }
}
