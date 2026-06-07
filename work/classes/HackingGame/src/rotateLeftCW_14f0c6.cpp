#include "class.h"

void HackingGame::rotateLeftCW(int *state)
{
    int a = state[0];
    int b = state[1];
    int c = state[3];
    int d = state[4];
    state[0] = c;
    state[1] = a;
    state[3] = d;
    state[4] = b;
    I(this, 0x12c) = 0;
    B(this, 0x128) = 1;
}
