#include "class.h"

struct FModSound {
    void play(int id, void *pos, void *vel, float volume);
};

__attribute__((visibility("hidden"))) extern FModSound **g_HackingGame_update_sound;

int HackingGame::update(int dt)
{
    if (B(this, 0x128) != 0 || B(this, 0x129) != 0) {
        int timer = I(this, 0x12c) + dt;
        I(this, 0x12c) = timer;
        if (timer > 300) {
            this->f_128 = 0;
            I(this, 0x12c) = 0;
            for (int i = 0; i != 6; ++i)
                I(this, 0x1c + i * 4) = I(this, 0x34 + i * 4);
        }
    }

    int wonTimer;
    for (unsigned i = 0; i < 6; ++i) {
        if (I(this, 0x1c + i * 4) != I(this, 4 + i * 4))
            goto running;
    }

    wonTimer = I(this, 0x130);
    if (wonTimer == 0) {
        (*g_HackingGame_update_sound)->play(0x8e1, 0, 0, 0.0f);
        wonTimer = I(this, 0x130);
    }
    wonTimer += dt;
    if (wonTimer > 0x5dc) {
        I(this, 0x130) = wonTimer;
        return 0;
    }
    I(this, 0x130) = wonTimer;
running:
    return 1;
}
