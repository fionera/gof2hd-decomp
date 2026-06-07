#include "class.h"
extern "C" int PlayerEgo_goingToStream(PlayerEgo*);
extern "C" int PlayerEgo_goingToStation(PlayerEgo*);
bool PlayerEgo_goingToPlanet(PlayerEgo* self) {
  if (C(self, 0x158) != 0 && PlayerEgo_goingToStream(self) == 0 && PlayerEgo_goingToStation(self) == 0)
    return C(self, 0x160) == 0;
  return false;
}
