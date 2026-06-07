#include "class.h"

int HackingGame::gameWon(int *state)
{
    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (state[idx] != I(this, 4 + idx * 4))
            return 0;
    } while (true);
}
