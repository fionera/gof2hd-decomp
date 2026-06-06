#include "class.h"

extern "C" void Player_setPlayShootSound(Player *self, bool play, int id)
{
    self->playShootSound = play;
    self->playShootSoundId = id;
}
