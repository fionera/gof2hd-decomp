#include "class.h"

void LodMeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)this->f_30;
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)this->f_34 + index) == 0) {
            return;
        }
        this->f_3c = 1;
    }
}
