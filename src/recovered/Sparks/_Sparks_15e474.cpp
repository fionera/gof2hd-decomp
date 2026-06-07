#include "class.h"

struct Sparks {
    ~Sparks();
};

extern "C" void operator_delete(void *ptr);

Sparks::~Sparks()
{
    void *p = F<void *>(this, 0x0);
    if (p != 0)
        operator_delete(p);
    F<void *>(this, 0x0) = 0;
}
