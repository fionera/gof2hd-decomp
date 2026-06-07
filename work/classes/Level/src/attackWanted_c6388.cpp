#include "class.h"

extern "C" void Level_setAlwaysEnemy(int obj, int flag);
extern "C" void Level_turnEnemy(int obj);
extern "C" int Level_getNumWingmen(int wanted);

__attribute__((visibility("hidden"))) extern int **g_attackWanted;

void Level::attackWanted(int index) {
    if (field_29c == 0) {
        field_29c = 1;
        createRadioMessage(0x10, index);
        int **slot = g_attackWanted;
        for (int i = 1;
             i - 1 < Level_getNumWingmen(((int *)(*(int *)((*(int *)*slot) + 4)))[index]);
             i = i + 1) {
            Level_setAlwaysEnemy(*(int *)(((int *)((Array *)enemies)->data)[i] + 4), 1);
            Level_turnEnemy(*(int *)(((int *)((Array *)enemies)->data)[i] + 4));
        }
    }
}
