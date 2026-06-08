#include "class.h"

struct BombGun {
    void render();
};

extern "C" void RocketGun_render(BombGun *self);
extern "C" void Explosion_render(Explosion *self);

void BombGun::render()
{
    RocketGun_render(this);
    if (F<uint8_t>(F<void *>(this, 0x8), 0x88) != 0)
        return Explosion_render(F<Explosion *>(this, 0xf0));
}
