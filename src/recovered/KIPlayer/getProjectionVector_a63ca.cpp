#include "class.h"

extern "C" void KIPlayer_getProjectionVector(Vector *out)
{
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}
