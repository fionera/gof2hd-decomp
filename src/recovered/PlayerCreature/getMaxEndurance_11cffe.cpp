#include "class.h"

struct PlayerCreature {
    int getMaxEndurance();
};

int PlayerCreature::getMaxEndurance()
{
    return F<int>(this, 0x134);
}
