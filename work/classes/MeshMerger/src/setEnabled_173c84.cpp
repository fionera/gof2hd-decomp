#include "class.h"

void MeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)this->f_28;
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)this->f_2c + index) == 0) {
            return;
        }
        this->f_34 = 1;
    }
}
