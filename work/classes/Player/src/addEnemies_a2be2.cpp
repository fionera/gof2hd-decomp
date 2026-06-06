#include "class.h"

void *operator new(uint32_t size);

extern "C" void Array_Player_ctor(Array<Player *> *array);
extern "C" void ArrayAdd_Player(Player *value, Array<Player *> *array);
extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies);
extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete_tail(void *p);
extern "C" void Player_setEnemies_tail(Player *self, Array<Player *> *enemies);

__attribute__((minsize)) extern "C" void Player_addEnemies(Player *self, Array<Player *> *enemies)
{
    if (self->enemies == 0) {
        return Player_setEnemies_tail(self, enemies);
    }
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    for (unsigned int i = 0; i < self->enemies->length; i++) {
        ArrayAdd_Player(self->enemies->data[i], tmp);
    }
    for (unsigned int i = 0; i < enemies->length; i++) {
        ArrayAdd_Player(enemies->data[i], tmp);
    }
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}
