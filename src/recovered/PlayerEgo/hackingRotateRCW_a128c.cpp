#include "class.h"
extern "C" int HackingGame_isRotating(int);
extern "C" int HackingGame_gameWon(int);
extern "C" void PlayerEgo_hackingRotateRCW_ext(int, int);
void PlayerEgo_hackingRotateRCW(PlayerEgo* self) {
  if (I(self, 0x1e8) != 0 && HackingGame_isRotating(I(self, 0x1e8)) == 0
      && HackingGame_gameWon(I(self, 0x1e8)) == 0)
    PlayerEgo_hackingRotateRCW_ext(I(self, 0x1e8), 1);
}
