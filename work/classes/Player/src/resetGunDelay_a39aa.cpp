#include "class.h"

__attribute__((minsize)) extern "C" void Player_resetGunDelay(Player *self, int slot)
{
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
    if (arr != 0) {
        int n = arr->length;
        for (int i = 0; n != i; i++) {
            *(int *)((char *)arr->data[i] + 0x6c) = 0;
        }
    }
}
