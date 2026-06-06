#include "class.h"

extern "C" void Player_removeAllGuns_releaseClasses(void *array);
extern "C" void *Player_removeAllGuns_dtor(void *array);
extern "C" void Player_removeAllGuns_delete(void *p);

extern "C" void Player_removeAllGuns(Player *self)
{
    if (self->guns != 0) {
        Player_removeAllGuns_releaseClasses(self->guns);
        if (self->guns != 0) {
            Player_removeAllGuns_delete(Player_removeAllGuns_dtor(self->guns));
        }
    }
    self->guns = 0;
}
