#include "class.h"

extern "C" bool ListItem_isPendingProduct(ListItem *self) {
    return self->f_18 != 0;
}
