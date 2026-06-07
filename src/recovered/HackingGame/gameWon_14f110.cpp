#include "class.h"

int HackingGame::gameWon()
{
    if (I(this, 0x130) < 0x5dd)
        return 0;

    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (I(this, 0x1c + idx * 4) != I(this, 4 + idx * 4))
            return 0;
    } while (true);
}
