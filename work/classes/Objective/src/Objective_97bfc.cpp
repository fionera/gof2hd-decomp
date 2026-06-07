#include "class.h"

Objective::Objective(int type, int value, Level *level)
{
    F<int>(this, 0x0) = type;
    F<int>(this, 0x4) = value;
    F<int>(this, 0x8) = 0;
    F<Level *>(this, 0xc) = level;
    F<int>(this, 0x10) = 0;
    F<int>(this, 0x14) = 0;
    F<int>(this, 0x18) = 0;
}
