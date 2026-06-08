#include "class.h"

extern "C" void BluePrint_lock(BluePrint *self)
{
    self->f_8 = 1;
}
