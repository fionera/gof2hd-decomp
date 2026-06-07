#include "class.h"

extern "C" bool BluePrint_isEmpty(BluePrint *self)
{
    return F<int32_t>(self, 0x4) == 0;
}
