#include "class.h"

// PendingProduct::~PendingProduct() -> destroys the embedded String subobject.
// (Tail-call to the String destructor; PendingProduct adds no other owned members.)
extern "C" void PendingProduct_dtor(PendingProduct *self)
{
    _ZN11AbyssEngine6StringD1Ev((String *)self);
}
