#include "class.h"

struct Player {
    static void setAlwaysEnemy(bool enemy);
    static void turnEnemy();
};

struct Wanted {
    static int getNumWingmen(Wanted *self);
};

__attribute__((visibility("hidden"))) extern int **g_uncoverWanted;

void Level::uncoverWanted(int index) {
    if (field_29c == 0) {
        createRadioMessage(0x12, index);
        int **g = g_uncoverWanted;
        for (int i = 1;
             i - 1 < Wanted::getNumWingmen((Wanted *)((int *)(*(int *)(*g) + 4))[index]);
             i = i + 1) {
            Player::setAlwaysEnemy((bool)((int *)((int *)(((int *)enemies)[1]))[i])[1]);
            Player::turnEnemy();
        }
    }
}
