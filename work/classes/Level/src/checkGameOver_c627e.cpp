#include "class.h"

extern "C" int Level_checkGameOver_call(int objective);

int Level::checkGameOver() {
    int objective = objectivesB;
    if (objective == 0) {
        return 0;
    }
    return Level_checkGameOver_call(objective);
}
