#include "class.h"

void *operator new(uint32_t size);

extern "C" void Array_Player_ctor(Array<Player *> *array);
extern "C" void ArrayAdd_Player(Player *value, Array<Player *> *array);
extern "C" void Player_setEnemies(Player *self, Array<Player *> *enemies);
extern "C" void *Array_Player_dtor(Array<Player *> *array);
extern "C" void Player_operator_delete_tail(void *p);

__attribute__((minsize)) extern "C" void Player_setEnemy(Player *self, Player *enemy)
{
    Array<Player *> *tmp = static_cast<Array<Player *> *>(operator new(sizeof(Array<Player *>)));
    Array_Player_ctor(tmp);
    ArrayAdd_Player(enemy, tmp);
    Player_setEnemies(self, tmp);
    Array_Player_dtor(tmp);
    return Player_operator_delete_tail(tmp);
}
