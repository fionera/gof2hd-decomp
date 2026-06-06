#include "class.h"

extern "C" int Level_checkObjective_call(int objective);

int Level::checkObjective() {
    int objective = objectivesA;
    if (objective != 0) {
        return Level_checkObjective_call(objective);
    }
    return 0;
}
