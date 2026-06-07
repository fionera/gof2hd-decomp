#include "class.h"

struct AEGeometry;

// PlayerFighter::setShipGroup is a thunk that tail-jumps to the base implementation
// (target is a single b.w to a relocated veneer). Model as a tail call with the same args.
extern "C" void PlayerFighter_setShipGroup_base(AEGeometry *self, int a, bool b);

extern "C" void PlayerFighter_setShipGroup(AEGeometry *self, int a, bool b)
{
    return PlayerFighter_setShipGroup_base(self, a, b);
}
