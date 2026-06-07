#include "class.h"

// BluePrint::isCompleted() -> true once every ingredient counter has dropped below 1.
extern "C" bool BluePrint_isCompleted(BluePrint *self)
{
    Array<int> *a = F<Array<int> *>(self, 0x0);
    for (uint32_t i = 0; i < a->length; i++)
        if (a->data[i] >= 1)
            return false;
    return true;
}
