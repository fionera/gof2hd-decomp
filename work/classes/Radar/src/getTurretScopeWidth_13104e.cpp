#include "class.h"

int Radar::getTurretScopeWidth()
{
    return F<int>(this, 0x12c) << 1;
}
