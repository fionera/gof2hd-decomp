#include "class.h"

// PendingProduct::PendingProduct(BluePrint*)
extern "C" PendingProduct *PendingProduct_ctor_BluePrint(PendingProduct *self, BluePrint *bp)
{
    void *volatile cookie = __stack_chk_guard;

    // Construct the embedded String (station name) at +0x00.
    _ZN11AbyssEngine6StringC1Ev((String *)self);

    i32(self, 0x14) = BluePrint_getIndex(bp);

    {
        String name;
        BluePrint_getStationName(&name, bp);
        F<String>(self, 0x0) = name;
    }

    i32(self, 0x0c) = BluePrint_getStationIndex(bp);
    i32(self, 0x10) = BluePrint_getQuantity(bp);

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
    return self;
}
