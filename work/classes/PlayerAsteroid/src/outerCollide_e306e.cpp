#include "class.h"

typedef void (*OuterCollideFn)(PlayerAsteroid *, float, float, float);

void PlayerAsteroid::outerCollide(float x, float y, float z)
{
    return (*(OuterCollideFn *)((char *)*(void **)this + 0x38))(this, x, y, z);
}
