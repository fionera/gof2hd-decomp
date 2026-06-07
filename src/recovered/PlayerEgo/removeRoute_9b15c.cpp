#include "class.h"
struct Route { ~Route(); };
void PlayerEgo_removeRoute(PlayerEgo* self) {
  Route* r = (Route*)P(self, 0xfc);
  if (r != 0) delete r;
  P(self, 0xfc) = 0;
}
