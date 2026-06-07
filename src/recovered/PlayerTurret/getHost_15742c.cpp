#include "class.h"

struct PlayerTurret {
    KIPlayer *getHost();
};

KIPlayer *PlayerTurret::getHost()
{
    return TP<KIPlayer>(this, 0x154);
}
