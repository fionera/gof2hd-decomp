#include "class.h"

extern "C" void Player_stopShootSound(Player *self, int a, int b);

__attribute__((minsize)) extern "C" void Player_stopShooting(Player *self, int slot, int channel)
{
    if ((unsigned int)(channel - 0x16) >= 9) {
        return;
    }
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns == 0) {
        return;
    }
    if (slot < 0) {
        return;
    }
    if ((unsigned int)slot >= guns->length) {
        return;
    }
    Array<Gun *> *arr = guns->data[slot];
    if (arr == 0) {
        return;
    }
    for (unsigned int i = 0; i < arr->length; i++) {
        Gun *gun = arr->data[i];
        Player_stopShootSound(self, *(int *)((char *)gun + 0x58), *(int *)((char *)gun + 0x5c));
        arr = self->guns->data[slot];
    }
}
