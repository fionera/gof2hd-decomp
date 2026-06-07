#include "class.h"

extern "C" bool Mission_isEmpty(Mission *self) {
    return F<int>(self, 0xc) == -1;
}
