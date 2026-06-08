#include "class.h"

extern "C" bool ListItem_isMission(ListItem *self) {
    return self->f_14 != 0;
}
