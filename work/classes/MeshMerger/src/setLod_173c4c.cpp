#include "class.h"

void MeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)this->f_24;
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)this->f_28 + index) == 0) {
            return;
        }
        this->f_34 = 1;
    }
}
