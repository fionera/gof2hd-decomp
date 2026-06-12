#include "gof2/PendingProduct.h"
#include "gof2/BluePrint.h"




// ---- _PendingProduct_12d480.cpp ----
// PendingProduct::~PendingProduct() -> destroys the embedded String subobject.
// (Tail-call to the String destructor; PendingProduct adds no other owned members.)
void PendingProduct::dtor() {
    PendingProduct *self = this;
    self->stationName.~String();
}

// ---- PendingProduct_12d448.cpp ----
// PendingProduct::PendingProduct(int blueprintIndex, String stationName, int stationIndex, int quantity)
PendingProduct * PendingProduct::ctor_fields(int blueprintIndex, const String *stationName, int stationIndex, int quantity) {
    PendingProduct *self = this;
    // Construct embedded String, then fill scalar fields.
    new (&self->stationName) String();
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

    // Construct the embedded String (station name) at +0x00.
    new (&self->stationName) String();

    self->blueprintIndex = ((BluePrint *)(bp))->getIndex();

    {
        String name;
        ((BluePrint *)(&name))->getStationName();
        self->stationName = name;
    }

    self->stationIndex = ((BluePrint *)(bp))->getStationIndex();
    self->quantity = ((BluePrint *)(bp))->getQuantity();


    return self;
}

// ---- ArrayAdd<PendingProduct*>_0xb8fae.cpp ----
// Append this pending product to the player's pending-product list. The shipped
// binary monomorphised the engine's Array<T>::add for PendingProduct*; it bumps
// the element count, reallocs the backing store and stores the pointer at the tail.
void PendingProduct::add(Array<PendingProduct*> &list) {
    list.push_back(this);
}
