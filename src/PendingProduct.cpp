#include "gof2/PendingProduct.h"




// ---- _PendingProduct_12d480.cpp ----
// PendingProduct::~PendingProduct() -> destroys the embedded String subobject.
// (Tail-call to the String destructor; PendingProduct adds no other owned members.)
extern "C" void PendingProduct_dtor(PendingProduct *self)
{
    _ZN11AbyssEngine6StringD1Ev((String *)self);
}

// ---- PendingProduct_12d448.cpp ----
// PendingProduct::PendingProduct(int blueprintIndex, String stationName, int stationIndex, int quantity)
extern "C" PendingProduct *PendingProduct_ctor_fields(
    PendingProduct *self, int blueprintIndex, const String *stationName,
    int stationIndex, int quantity)
{
    // Construct embedded String, then fill scalar fields.
    _ZN11AbyssEngine6StringC1Ev((String *)self);
    self->field_0x14 = blueprintIndex;
    self->field_0x0 = *stationName;
    self->field_0xc = stationIndex;
    self->field_0x10 = quantity;
    return self;
}

// ---- PendingProduct_12d3c8.cpp ----
// PendingProduct::PendingProduct(BluePrint*)
extern "C" PendingProduct *PendingProduct_ctor_BluePrint(PendingProduct *self, BluePrint *bp)
{
    void *volatile cookie = __stack_chk_guard;

    // Construct the embedded String (station name) at +0x00.
    _ZN11AbyssEngine6StringC1Ev((String *)self);

    self->field_0x14 = BluePrint_getIndex(bp);

    {
        String name;
        BluePrint_getStationName(&name, bp);
        self->field_0x0 = name;
    }

    self->field_0xc = BluePrint_getStationIndex(bp);
    self->field_0x10 = BluePrint_getQuantity(bp);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
    return self;
}
