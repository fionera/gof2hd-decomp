#include "class.h"

struct PlayerCreature {
    void calmDown();
};

void PlayerCreature::calmDown()
{
    int maxEndurance = F<int>(this, 0x134);
    *(volatile int *)((char *)this + 0x130) = 0;
    *(volatile uint16_t *)((char *)this + 0x128) = 0;
    F<int>(this, 0x138) = maxEndurance;
}
