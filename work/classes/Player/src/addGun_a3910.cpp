#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" void Array_Gun_ctor(Array<Gun *> *array);
extern "C" void ArrayAdd_Gun(Gun *gun, Array<Gun *> *array);
extern "C" void Player_shoot_tail(Player *self, int soundId);

__attribute__((minsize)) extern "C" void Player_addGun(Player *self, Array<Gun *> *guns, int slot)
{
    if (self->guns != 0) {
        if ((unsigned int)slot < 4) {
            Array<Gun *> *arr = static_cast<Array<Gun *> *>(operator new(sizeof(Array<Gun *>)));
            Array_Gun_ctor(arr);
            self->guns->data[slot] = arr;
            for (unsigned int i = 0; i < guns->length; i++) {
                ArrayAdd_Gun(guns->data[i], self->guns->data[slot]);
            }
        }
        if (self->playShootSound) {
            return Player_shoot_tail(self, self->playShootSoundId);
        }
    }
}
