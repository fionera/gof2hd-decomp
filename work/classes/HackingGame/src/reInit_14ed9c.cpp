#include "class.h"

extern "C" int AERandom_nextInt(void *random, int limit);

__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random2;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random3;
__attribute__((visibility("hidden"))) extern void **g_HackingGame_reinit_random_shuffle;

void HackingGame::reInit()
{
    char localBytes[24];
    int *local = (int *)localBytes;

    int type = I(this, 0);
    I(this, 0x12c) = 0;
    I(this, 0x130) = 0;

    if (type == 1) {
        for (unsigned i = 0; i != 6; ++i)
            I(this, 4 + i * 4) = i >> 1;
    } else if (type == 2) {
        for (int i = 0; i != 4; ++i)
            I(this, 4 + i * 4) = i;
        void **random = g_HackingGame_reinit_random2;
        I(this, 0x14) = AERandom_nextInt(*random, 4);
        I(this, 0x18) = AERandom_nextInt(*random, 4);
    } else if (type == 3) {
        for (int i = 0; i != 5; ++i)
            I(this, 4 + i * 4) = i;
        I(this, 0x18) = AERandom_nextInt(*g_HackingGame_reinit_random3, 5);
    } else {
        for (int i = 0; i != 6; ++i)
            I(this, 4 + i * 4) = i;
    }

    void **random = g_HackingGame_reinit_random_shuffle;
    for (int i = 0x28; i != 0; --i) {
        int a = AERandom_nextInt(*random, 6);
        int b = AERandom_nextInt(*random, 6);
        int value = I(this, 4 + a * 4);
        I(this, 4 + a * 4) = I(this, 4 + b * 4);
        I(this, 4 + b * 4) = value;
    }

    for (int i = 0; i != 6; ++i) {
        int value = I(this, 4 + i * 4);
        I(this, 0x1c + i * 4) = value;
        I(this, 0x34 + i * 4) = value;
    }

    for (unsigned i = 0; (int)i < I(this, 0) * 2; ++i) {
        int count = AERandom_nextInt(*random, 2);
        for (int j = 0; j <= count; ++j) {
            if ((i & 1) == 0)
                rotateRightCW(false);
            else
                rotateLeftCW(false);
        }

        type = I(this, 0);
        if (i == (unsigned)(type * 2 - 1)) {
            for (int j = 0; j != 6; ++j)
                local[j] = I(this, 0x34 + j * 4);
            if (solvableInNSteps(type, 0, 0, 0, local) != 0)
                i = 0;
        }
    }

    for (int i = 0; i != 6; ++i)
        I(this, 0x1c + i * 4) = I(this, 0x34 + i * 4);

    this->f_128 = 0;
}
