#include "class.h"

Trail::~Trail()
{
    void *first = ptr(this, 0x18);
    if (first != 0) {
        operator delete(first);
    }
    ptr(this, 0x18) = 0;

    void *second = ptr(this, 0x1c);
    if (second != 0) {
        operator delete(second);
    }
    ptr(this, 0x1c) = 0;
}
