#include "class.h"

__attribute__((minsize)) extern "C" bool Player_gunAvailable(Player *self, unsigned int slot)
{
    if (slot < 4) {
        Array<Gun *> *slotArray = self->guns->data[slot];
        if (slotArray != 0 && slotArray->length != 0) {
            return *(int *)slotArray->data != 0;
        }
    }
    return false;
}
