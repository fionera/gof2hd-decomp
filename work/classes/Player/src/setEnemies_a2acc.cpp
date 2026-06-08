#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete(void *p);
extern "C" void Array_Player_ctor(Array<Player *> *array);
extern "C" void ArrayAdd_PlayerArray(Array<Player *> *src, Array<Player *> *dst);
extern "C" void Gun_setEnemies(void *gun);

__attribute__((minsize)) extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies)
{
    if (self->enemies != 0) {
        Player_operator_delete(Array_Player_dtor(self->enemies));
    }
    self->enemies = 0;
    if (enemies != 0) {
        Array<Player *> *copy = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
        Array_Player_ctor(copy);
        self->enemies = copy;
        ArrayAdd_PlayerArray(enemies, copy);
    }
    Array<Array<Gun *> *> *guns = self->guns;
    if (guns != 0) {
        for (unsigned int i = 0; i < guns->length; i++) {
            Array<Gun *> *slot = guns->data[i];
            if (slot != 0) {
                for (unsigned int j = 0; j < slot->length; j++) {
                    Gun *gun = slot->data[j];
                    if (gun != 0) {
                        Gun_setEnemies(gun);
                        guns = self->guns;
                        slot = guns->data[i];
                    }
                }
            }
        }
    }
}
