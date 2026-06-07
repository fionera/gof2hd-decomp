#include "class.h"

struct PlayerCreature {
    int getEndurance();
};

int PlayerCreature::getEndurance()
{
    return F<int>(this, 0x138);
}
