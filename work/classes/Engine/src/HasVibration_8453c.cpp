#include "class.h"

extern "C" bool Engine_HasVibration(Engine *self)
{
    if (*(bool *)((char *)self + 0x480)) {
        return *(bool *)((char *)self + 0x2c) != 0;
    }
    return false;
}
