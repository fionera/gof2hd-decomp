#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);

struct PlayerTurret {
    void setHost(KIPlayer *host, const Vector &offset);
};

void PlayerTurret::setHost(KIPlayer *host, const Vector &offset)
{
    P(this, 0x154) = host;
    Vector_assign((Vector *)B(this, 0x158), &offset);
}
