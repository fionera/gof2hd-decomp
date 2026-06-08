#include "class.h"

// PendingProduct::PendingProduct(int blueprintIndex, String stationName, int stationIndex, int quantity)
extern "C" PendingProduct *PendingProduct_ctor_fields(
    PendingProduct *self, int blueprintIndex, const String *stationName,
    int stationIndex, int quantity)
{
    // Construct embedded String, then fill scalar fields.
    _ZN11AbyssEngine6StringC1Ev((String *)self);
    i32(self, 0x14) = blueprintIndex;
    F<String>(self, 0x0) = *stationName;
    i32(self, 0x0c) = stationIndex;
    i32(self, 0x10) = quantity;
    return self;
}
