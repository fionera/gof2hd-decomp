#include "class.h"

extern "C" bool ListItem_isCargo(ListItem *self) {
    return self->f_10 != 0;
}
