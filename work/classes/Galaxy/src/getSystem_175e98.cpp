#include "class.h"

int Galaxy::getSystem(int index)
{
    if (index < 0)
        return 0;
    void *systems = this->systems;
    void *data = systems->systems;
    return ((int *)data)[index];
}
