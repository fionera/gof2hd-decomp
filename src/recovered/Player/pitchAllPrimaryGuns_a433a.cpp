#include "class.h"

__attribute__((minsize)) extern "C" void Player_pitchAllPrimaryGuns(Player *self, float pitch)
{
    if (self->guns != 0) {
        Array<Gun *> *prim = self->guns->data[0];
        if (prim != 0) {
            int n = prim->length;
            for (int i = 0; n != i; i++) {
                *(float *)((char *)prim->data[i] + 0xb0) = pitch;
            }
        }
    }
}
