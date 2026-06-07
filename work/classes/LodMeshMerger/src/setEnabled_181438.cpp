#include "class.h"

void LodMeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)pp(this, 0x30);
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)pp(this, 0x34) + index) == 0) {
            return;
        }
        u8(this, 0x3c) = 1;
    }
}
