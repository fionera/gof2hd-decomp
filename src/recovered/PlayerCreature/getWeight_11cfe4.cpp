#include "class.h"

extern "C" int PlayerCreature_weightTable[] __attribute__((visibility("hidden")));

struct PlayerCreature {
    int getWeight();
};

int PlayerCreature::getWeight()
{
    return PlayerCreature_weightTable[F<int>(this, 0xac)];
}
