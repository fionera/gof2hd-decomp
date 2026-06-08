#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" void Array_Player_ctor(Array<Player *> *array);
extern "C" void ArrayAdd_Player(Player *value, Array<Player *> *array);
extern "C" void ArrayAdd_PlayerArray(Array<Player *> *src, Array<Player *> *dst);
extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies);
extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete_tail(void *p);
extern "C" void Player_setEnemy_tail(Player *self, Player *enemy);

__attribute__((minsize)) extern "C" void Player_addEnemy(Player *self, Player *enemy)
{
    if (self->enemies == 0) {
        return Player_setEnemy_tail(self, enemy);
    }
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    if (self->enemies->length != 0) {
        ArrayAdd_PlayerArray(self->enemies, tmp);
    }
    ArrayAdd_Player(enemy, tmp);
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}
