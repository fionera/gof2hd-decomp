#ifndef GOF2_PENDINGPRODUCT_H
#define GOF2_PENDINGPRODUCT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class BluePrint;

// Galaxy on Fire 2 -- a pending item-production order.
// Begins with the embedded station-name String, followed by the order's scalar
// fields. Created either from a BluePrint or directly from saved field values.
class PendingProduct {
public:
    String stationName;     // station the product is being built at
    int    stationIndex;    // index of that station
    int    quantity;        // number of units ordered
    int    blueprintIndex;  // produced item / blueprint index

    explicit PendingProduct(BluePrint *bp);
    PendingProduct(int blueprintIndex, String stationName,
                   int stationIndex, int quantity);

    // Destroys the embedded stationName String. Non-virtual: PendingProduct has no
    // vtable, so the original is just a thunk to String::~String() on the first member.
    ~PendingProduct();
};
#endif
