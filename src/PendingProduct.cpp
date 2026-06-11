#include "gof2/PendingProduct.h"
#include "gof2/BluePrint.h"




// ---- _PendingProduct_12d480.cpp ----
// PendingProduct::~PendingProduct() -> destroys the embedded String subobject.
// (Tail-call to the String destructor; PendingProduct adds no other owned members.)
void PendingProduct::dtor() {
    PendingProduct *self = this;
    _ZN11AbyssEngine6StringD1Ev((String *)self);
}

// ---- PendingProduct_12d448.cpp ----
// PendingProduct::PendingProduct(int blueprintIndex, String stationName, int stationIndex, int quantity)
PendingProduct * PendingProduct::ctor_fields(int blueprintIndex, const String *stationName, int stationIndex, int quantity) {
    PendingProduct *self = this;
    // Construct embedded String, then fill scalar fields.
    _ZN11AbyssEngine6StringC1Ev((String *)self);
    self->blueprintIndex = blueprintIndex;
    self->stationName = *stationName;
    self->stationIndex = stationIndex;
    self->quantity = quantity;
    return self;
}

// ---- PendingProduct_12d3c8.cpp ----
// PendingProduct::PendingProduct(BluePrint*)
PendingProduct * PendingProduct::ctor_BluePrint(BluePrint *bp) {
    PendingProduct *self = this;
    void *volatile cookie = __stack_chk_guard;

    // Construct the embedded String (station name) at +0x00.
    _ZN11AbyssEngine6StringC1Ev((String *)self);

    self->blueprintIndex = BluePrint_getIndex(bp);

    {
        String name;
        ((BluePrint *)(&name))->getStationName();
        self->stationName = name;
    }

    self->stationIndex = BluePrint_getStationIndex(bp);
    self->quantity = BluePrint_getQuantity(bp);

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
    return self;
}
