#include "class.h"

extern "C" void Level_render2D_call(int starSystem);

void Level::render2D() {
    return Level_render2D_call(starSystem);
}
