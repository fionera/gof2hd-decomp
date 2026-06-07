#include "class.h"

struct PlayerCreature {
    void unhook();
};

void PlayerCreature::unhook()
{
    int maxEndurance = F<int>(this, 0x134);
    *(volatile uint8_t *)((char *)this + 0x12a) = 0;
    *(volatile int *)((char *)this + 0x130) = 0;
    *(volatile uint16_t *)((char *)this + 0x128) = 0;
    *(volatile int *)((char *)this + 0x138) = maxEndurance;
}
