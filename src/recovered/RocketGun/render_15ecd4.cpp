#include "class.h"

struct RocketGun {
    void render();
};

extern "C" void RocketGun_render_tail();

void RocketGun::render()
{
    return RocketGun_render_tail();
}
