#include "class.h"

struct PlayerWormHole {
    int open(char *__file, int __oflag, ...);
};

int PlayerWormHole::open(char *, int, ...)
{
    F<int>(this, 0x150) = -3000;
    F<int>(this, 0x154) = 0;
    return (int)this;
}
