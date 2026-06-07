#include "class.h"

void *FindResource(AbyssEngine::PaintCanvas *self, unsigned short id)
{
    unsigned int count = *(unsigned int *)((char *)self + 0x134);
    void *found = 0;
    for (unsigned int i = 0; i < count; ++i) {
        char *res = (*(char ***)((char *)self + 0x138))[i];
        if (res && *(unsigned short *)res == id) {
            found = res;
            break;
        }
    }
    return found;
}
