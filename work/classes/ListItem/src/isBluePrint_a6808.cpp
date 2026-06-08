#include "class.h"

extern "C" bool ListItem_isBluePrint(ListItem *self) {
    return self->f_8 != 0;
}
