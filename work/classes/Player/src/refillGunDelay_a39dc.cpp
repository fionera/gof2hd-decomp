#include "class.h"

__attribute__((minsize)) extern "C" void Player_refillGunDelay(Player *self, int slot)
{
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0 && slot >= 0 && (unsigned int)slot < guns->length) {
        Array<Gun *> *arr = guns->data[slot];
        if (arr != 0) {
            int n = arr->length;
            for (int i = 0; n != i; i++) {
                Gun *gun = arr->data[i];
                *(int *)((char *)gun + 0x6c) = *(int *)((char *)gun + 0x48);
            }
        }
    }
}
