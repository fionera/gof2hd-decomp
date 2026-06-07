#include "class.h"

struct PlayerCreature {
    int getItemIndex();
};

int PlayerCreature::getItemIndex()
{
    return F<int>(this, 0x140);
}
