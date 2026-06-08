#include "class.h"

// Standing::~Standing(): delete[] the standings array, null the pointer.
Standing::~Standing() {
    int *p = F<int *>(this, 0);
    if (p) operator delete[](p);
    F<int *>(this, 0) = 0;
}
