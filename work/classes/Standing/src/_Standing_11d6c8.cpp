#include "class.h"

// Standing::~Standing(): delete[] the standings array, null the pointer.
Standing::~Standing() {
    int *p = this->f_0;
    if (p) operator delete[](p);
    this->f_0 = 0;
}
