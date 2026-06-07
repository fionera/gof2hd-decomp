#include "class.h"

// BluePrint::complete() -> zero every quantity counter in the ingredient array at +0x00.
extern "C" void BluePrint_complete(BluePrint *self)
{
    Array<int> *a = F<Array<int> *>(self, 0x0);
    for (uint32_t i = 0; i < a->length; i++)
        a->data[i] = 0;
}
