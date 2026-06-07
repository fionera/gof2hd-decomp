#include "class.h"

void MeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)pp(this, 0x28);
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)pp(this, 0x2c) + index) == 0) {
            return;
        }
        u8(this, 0x34) = 1;
    }
}
