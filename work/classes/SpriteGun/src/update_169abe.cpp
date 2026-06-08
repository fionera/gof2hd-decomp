#include "class.h"

// SpriteGun::update(int) — load this->field_8, tail-call Gun::update on the base.
extern "C" void SpriteGun_Gun_update(void *base);

extern "C" void SpriteGun_update(SpriteGun *this_)
{
    return SpriteGun_Gun_update(this_->f_8);
}
