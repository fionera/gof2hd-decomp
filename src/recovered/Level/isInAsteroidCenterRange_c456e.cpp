#include "class.h"

void Level::isInAsteroidCenterRange(Vector v) {
    int *vol = *(int **)((char *)this + 0xc4);
    return (*(void (**)(int *, Vector))(*vol + 8))(vol, v);
}
