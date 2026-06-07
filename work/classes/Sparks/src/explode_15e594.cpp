#include "class.h"

using AbyssEngine::AEMath::Vector;

struct Sparks {
    void explode(Vector const &v);
};

extern "C" void Sparks_explode_int(Sparks *self, int x, int y, int z);

void Sparks::explode(Vector const &v)
{
    return Sparks_explode_int(this, (int)v.x, (int)v.y, (int)v.z);
}
