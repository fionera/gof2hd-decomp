#include "class.h"

extern "C" void Level_enableMovingStars_call(int skybox, int index, bool enable);

void Level::enableMovingStars(bool enable) {
    int index = movingStarsIndex;
    if (index < 0) {
        return;
    }
    Level_enableMovingStars_call(skybox2Mesh, index, enable);
}
