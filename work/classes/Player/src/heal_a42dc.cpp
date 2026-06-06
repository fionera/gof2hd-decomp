#include "class.h"

extern "C" void Player_regenerateHull(Player *self);

__attribute__((minsize)) extern "C" void Player_heal(Player *self, float amount)
{
    float f = *(float *)((char *)self + 0x110) + amount;
    *(float *)((char *)self + 0x110) = f;
    if (f > 1.0f) {
        int count = (int)f;
        for (int i = 0; i < count; i++) {
            Player_regenerateHull(self);
        }
        *(float *)((char *)self + 0x110) = *(float *)((char *)self + 0x110) - (float)count;
    }
}
