#include "game/weapons/AbstractGun.h"

// Base-class defaults: a plain AbstractGun is none of the specialized weapon
// kinds. Concrete subclasses override exactly the predicate that applies.
int AbstractGun::isRocketGun()  { return 0; }
int AbstractGun::isBombGun()    { return 0; }
int AbstractGun::isMineGun()    { return 0; }
int AbstractGun::isAutoTurret() { return 0; }
