#ifndef GOF2_PENDINGPRODUCT_H
#define GOF2_PENDINGPRODUCT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PendingProduct (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (not namespaced). It begins with an AbyssEngine::String subobject
// (the station name) at +0x00, followed by scalar fields:
//   +0x00  String  stationName
//   +0x0c  int     stationIndex
//   +0x10  int     quantity
//   +0x14  int     blueprintIndex


struct PendingProduct;
struct BluePrint;

extern "C" {

// BluePrint accessors used by the ctor.
int BluePrint_getIndex(BluePrint *bp);
void BluePrint_getStationName(String *out, BluePrint *bp);
int BluePrint_getStationIndex(BluePrint *bp);
int BluePrint_getQuantity(BluePrint *bp);

    // ---- methods (converted from free functions) ----
    PendingProduct * ctor_BluePrint(BluePrint *bp);
    PendingProduct * ctor_fields(int blueprintIndex, const String *stationName, int stationIndex, int quantity);
    void dtor();
}

class PendingProduct {
public:
    String stationName;                   // +0x0  station name
    int stationIndex;                      // +0xc  stationIndex
    int quantity;                     // +0x10 quantity
    int blueprintIndex;                     // +0x14 blueprintIndex

    // ---- methods (converted from free functions) ----
    PendingProduct * ctor_BluePrint(BluePrint *bp);
    PendingProduct * ctor_fields(int blueprintIndex, const String *stationName, int stationIndex, int quantity);
    void dtor();

    // Append this pending product to the player's pending-product list. This is the
    // monomorphised Array<PendingProduct*>::add helper: it grows the backing store by
    // one and stores this element pointer at the new tail.
    void add(Array<PendingProduct*> &list);
};
#endif
