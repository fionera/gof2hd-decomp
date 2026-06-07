#include "class.h"

typedef bool (*CollideFn)(PlayerStation *self, float x, float y, float z);

bool PlayerStation::collide(float x, float y, float z)
{
    CollideFn fn = *(CollideFn *)((char *)P(this, 0x0) + 0x3c);
    return fn(this, x, y, z);
}
