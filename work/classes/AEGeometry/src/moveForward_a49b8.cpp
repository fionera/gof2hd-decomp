#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::moveForward(float dist)
//   getDirection(&dir); VectorNormalize(&n,&dir); getPosition(&pos);
//   pos += n*dist; setPosition(&pos)
extern "C" void _ae_getDirection(void *self, Vector *out);
extern "C" void _ae_getPosition(void *self, Vector *out);
extern "C" void _ae_setPosition_v(void *self, Vector *v);

struct AEGeometry { void moveForward(float dist); };
void AEGeometry::moveForward(float dist)
{
    Vector pos;
    Vector n;
    _ae_getDirection(this, &pos);
    VectorNormalize(&n, &pos);
    _ae_getPosition(this, &pos);
    pos.x = pos.x + n.x * dist;
    pos.y = pos.y + n.y * dist;
    pos.z = pos.z + n.z * dist;
    _ae_setPosition_v(this, &pos);
}
