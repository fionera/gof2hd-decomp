#include "class.h"

void Radar::update(KIPlayer *player)
{
    char position[sizeof(AbyssEngine::AEMath::Vector)];
    typedef void (*GetPosition)(void *, KIPlayer *);
    void **vtable = *(void ***)player;
    ((GetPosition)vtable[0x28 / 4])(position, player);
    update(*(AbyssEngine::AEMath::Vector *)position);
}
