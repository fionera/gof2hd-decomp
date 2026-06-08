#include "class.h"

extern "C" void ObjectGun_render(MineGun *self);
extern "C" void Explosion_render(Explosion *self);

void MineGun::render()
{
    ObjectGun_render(this);
    for (uint32_t i = 0; i < U(P(this, 0x8), 0x8); ++i) {
        if (F<uint8_t>(P(P(this, 0x8), 0x40), i) != 0) {
            Array<Explosion *> *explosions = (Array<Explosion *> *)P(this, 0xb4);
            Explosion_render(explosions->data[i]);
        }
    }
}
