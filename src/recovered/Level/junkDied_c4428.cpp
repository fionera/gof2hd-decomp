#include "class.h"

struct JunkObj {
    char pad[0xb0];
    int counter;
};

__attribute__((visibility("hidden"))) extern JunkObj **g_junkDied;

void Level::junkDied() {
    (*g_junkDied)->counter += 1;
    enemiesLeft -= 1;
}
