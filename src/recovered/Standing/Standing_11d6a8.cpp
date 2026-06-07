#include "class.h"

// Standing::Standing(): allocate a 2-int standings array {0x1e, 0}; currentRace = -1.
Standing::Standing() {
    int *p = new int[2];
    p[0] = 0x1e;
    p[1] = 0;
    F<int *>(this, 0) = p;
    F<int>(this, 4) = -1;
}
