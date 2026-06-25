#include "game/weapons/AbstractGun.h"

// AbstractGun's virtuals are defined inline in the header (no key function here),
// matching the original: no _ZTV11AbstractGun is emitted for the abstract base
// (only the 7 concrete derived guns get vtables).
