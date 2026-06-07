#include "class.h"

typedef void (*OuterCollideVectorFn)(PlayerAsteroid *, Vector);

void PlayerAsteroid::outerCollide(Vector value)
{
    return (*(OuterCollideVectorFn *)((char *)*(void **)this + 0x38))(this, value);
}
