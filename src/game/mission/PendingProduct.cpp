#include "game/mission/PendingProduct.h"
#include "game/mission/BluePrint.h"

PendingProduct::PendingProduct(int blueprintIndex, String stationName,
                               int stationIndex, int quantity) {
    this->blueprintIndex = blueprintIndex;
    this->stationName = stationName;
    this->stationIndex = stationIndex;
    this->quantity = quantity;
}

PendingProduct::PendingProduct(BluePrint *bp) {
    this->blueprintIndex = bp->getIndex();
    this->stationName = bp->getStationName();
    this->stationIndex = bp->getStationIndex();
    this->quantity = bp->getQuantity();
}

PendingProduct::~PendingProduct() {
}
