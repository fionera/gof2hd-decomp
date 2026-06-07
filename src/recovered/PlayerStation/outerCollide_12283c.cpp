#include "class.h"

typedef void (*OuterCollideVectorFn)(PlayerStation *self, float x, float y, float z);

void PlayerStation::outerCollide(const Vector &position)
{
    OuterCollideVectorFn fn = *(OuterCollideVectorFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, position.x, position.y, position.z);
}
