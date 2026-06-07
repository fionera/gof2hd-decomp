#include "class.h"

int HackingGame::solvableInNSteps(int steps, int depth, int leftCount, int rightCount, int *state)
{
    char leftBytes[24];
    char rightBytes[24];
    int *leftState = (int *)leftBytes;
    int *rightState = (int *)rightBytes;

    for (int i = 0; i != 6; ++i) {
        int value = state[i];
        rightState[i] = value;
        leftState[i] = value;
    }

    unsigned i = 0;
    do {
        if (i > 5)
            return 1;
        unsigned idx = i;
        ++i;
        if (state[idx] != I(this, 4 + idx * 4))
            break;
    } while (true);

    if (depth < steps) {
        unsigned move = 0;
        do {
            if (move > 1)
                return 0;

            if (leftCount < 3 && move == 0) {
                int a = leftState[0];
                int b = leftState[1];
                int c = leftState[3];
                int d = leftState[4];
                leftState[0] = c;
                leftState[1] = a;
                leftState[3] = d;
                leftState[4] = b;
                I(this, 0x12c) = 0;
                B(this, 0x128) = 1;
                return solvableInNSteps(steps, depth + 1, leftCount + 1, 0, leftState);
            }

            ++move;
            if (rightCount <= 2) {
                int a = rightState[1];
                int b = rightState[2];
                int c = rightState[4];
                int d = rightState[5];
                rightState[1] = c;
                rightState[2] = a;
                rightState[4] = d;
                rightState[5] = b;
                I(this, 0x12c) = 0;
                B(this, 0x129) = 1;
                return solvableInNSteps(steps, depth + 1, 0, rightCount + 1, rightState);
            }
        } while (true);
    }

    return 0;
}
