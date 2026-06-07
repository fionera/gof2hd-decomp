#include "class.h"

void *PlayerStation::getRoot()
{
    return P(this, 0x140);
}
