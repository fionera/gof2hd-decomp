#include "class.h"

extern "C" void BluePrint_unlock(BluePrint *self)
{
    self->f_8 = 1;
}
