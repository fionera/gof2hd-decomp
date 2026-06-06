#include "class.h"

__attribute__((minsize)) extern "C" float Player_damageGamma(Player *self, float amount)
{
    if (self->vulnerable) {
        if (self->active) {
            amount = self->gammaHP - amount;
            *((unsigned char *)self + 0x67) = 1;
            self->gammaHP = amount;
            if (!(amount > 0.0f)) {
                *(int *)((char *)self + 0xb8) = 0;
            }
        }
    }
    return amount;
}
