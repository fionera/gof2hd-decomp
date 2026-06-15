#ifndef GOF2_PENDINGPRODUCT_H
#define GOF2_PENDINGPRODUCT_H
#include "gof2/common.h"

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
    PendingProduct(int blueprintIndex, const String *stationName,
                   int stationIndex, int quantity);

    // Append this pending product to the player's pending-product list.
    void add(Array<PendingProduct*> &list);

    // Heap-allocating factory used by the save-game reader when materialising the
    // pending-product list off disk.
    static PendingProduct *make(int blueprintIndex, const String *stationName,
                                int stationIndex, int quantity);
};
#endif
