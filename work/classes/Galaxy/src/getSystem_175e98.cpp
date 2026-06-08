#include "class.h"

int Galaxy::getSystem(int index)
{
    if (index < 0)
        return 0;
    void *systems = P(this, 0x4);
    void *data = P(systems, 0x4);
    return ((int *)data)[index];
}
