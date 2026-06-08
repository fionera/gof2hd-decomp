#include "class.h"

extern "C" Array<int> *BluePrint_getQuantityList(BluePrint *self);
extern "C" void Status_incGoodsProduced(void *status, int n);
// Hidden PC-relative pointer-to-the-global-Status pointer.
extern void *const gStatusPtr __attribute__((visibility("hidden")));

// BluePrint::reset() -> bump production count, refill the ingredient counters, clear state.
extern "C" void BluePrint_reset(BluePrint *self)
{
    F<int32_t>(self, 0xc) += 1;
    Status_incGoodsProduced(*(void **)gStatusPtr, 1);
    Array<int> *ql = BluePrint_getQuantityList(self);
    Array<int> *counters = F<Array<int> *>(self, 0x0);
    for (uint32_t i = 0; i < counters->length; i++)
        counters->data[i] = ql->data[i];
    F<int32_t>(self, 0x10) = -1;
    F<int32_t>(self, 0x28) = F<int32_t>(self, 0x24);
    F<int32_t>(self, 0x4) = 0;
}
