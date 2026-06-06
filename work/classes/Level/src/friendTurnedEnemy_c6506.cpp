#include "class.h"

extern "C" void Level_friendTurnedEnemy_action(Level *self, int a, int b);

void Level::friendTurnedEnemy() {
    if ((unsigned char)field_188 == 0) {
        *(unsigned char *)&field_188 = 1;
        return Level_friendTurnedEnemy_action(this, 0, 0);
    }
}
