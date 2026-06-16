#include "game/mission/PendingProduct.h"
#include "game/mission/BluePrint.h"

PendingProduct::PendingProduct(int blueprintIndex, const String *stationName,
                               int stationIndex, int quantity)
    : stationName(*stationName),
      stationIndex(stationIndex),
      quantity(quantity),
      blueprintIndex(blueprintIndex) {
}

PendingProduct::PendingProduct(BluePrint *bp)
    : stationName(bp->getStationName()),
      stationIndex(bp->getStationIndex()),
      quantity(bp->getQuantity()),
      blueprintIndex(bp->getIndex()) {
}

// Append this pending product to the player's pending-product list.
void PendingProduct::add(Array<PendingProduct*> &list) {
    list.push_back(this);
}

// Allocating factory used by RecordHandler when reconstructing the pending-product
// list from a save file.
PendingProduct *PendingProduct::make(int blueprintIndex, const String *stationName,
                                     int stationIndex, int quantity) {
    return new PendingProduct(blueprintIndex, stationName, stationIndex, quantity);
}
