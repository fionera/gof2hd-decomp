#include "class.h"

int ResourceLoaded(AbyssEngine::PaintCanvas *self, unsigned int index, unsigned int type)
{
    unsigned int count;
    switch (type) {
    case 1: {
        char *res = (*(char ***)((char *)self + 0x138))[index];
        if (*(int *)(res + 0x4) == 2) {
            int handle = *(int *)(res + 0x8);
            return handle + 1 != 0 ? 1 : 0;
        }
        return 0;
    }
    case 2:
        count = *(unsigned int *)((char *)self + 0x140);
        break;
    case 3:
        count = *(unsigned int *)((char *)self + 0x14c);
        break;
    case 4:
        count = *(unsigned int *)((char *)self + 0x24);
        break;
    case 5:
        count = *(unsigned int *)((char *)self + 0x158);
        break;
    case 6:
        count = *(unsigned int *)((char *)self + 0x174);
        break;
    default:
        return 0;
    }
    return index < count ? 1 : 0;
}
