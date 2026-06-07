#include "class.h"

void HackingGame::rotateRightCW(int *state)
{
    int a = state[1];
    int b = state[2];
    int c = state[4];
    int d = state[5];
    state[1] = c;
    state[2] = a;
    state[4] = d;
    state[5] = b;
    I(this, 0x12c) = 0;
    B(this, 0x129) = 1;
}
