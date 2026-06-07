#include "class.h"

struct PendingProduct;
extern "C" void Array_PendingProduct_ctor(int *a);
extern "C" int BluePrint_getIndex(BluePrint *bp);
extern "C" int BluePrint_getStationIndex(BluePrint *bp);
extern "C" int BluePrint_getQuantity(BluePrint *bp);
extern "C" void PendingProduct_ctor(int *pp, BluePrint *bp);
extern "C" void PendingProduct_add(int *pp, int arr);

// Merges `bp` into the pending-products list, bumping an existing matching entry's quantity
// or appending a freshly constructed PendingProduct.
void Status::addPendingProduct(BluePrint *bp) {
    unsigned *arr = (unsigned *)(uint32_t)pendingProducts;
    if (arr == 0) {
        int *na = (int *)operator new(0xc);
        Array_PendingProduct_ctor(na);
        pendingProducts = (int32_t)(uint32_t)na;
    } else {
        for (unsigned i = 0; i < *arr; i = i + 1) {
            int e = ((int *)arr[1])[i];
            if (e != 0 && *(int *)(e + 0x14) == BluePrint_getIndex(bp) &&
                *(int *)(((int *)((int *)(uint32_t)pendingProducts)[1])[i] + 0xc) ==
                    BluePrint_getStationIndex(bp)) {
                int q = BluePrint_getQuantity(bp);
                int slot = ((int *)((int *)(uint32_t)pendingProducts)[1])[i];
                *(int *)(slot + 0x10) = q + *(int *)(slot + 0x10);
                return;
            }
            arr = (unsigned *)(uint32_t)pendingProducts;
        }
    }
    int *pp = (int *)operator new(0x18);
    PendingProduct_ctor(pp, bp);
    PendingProduct_add(pp, pendingProducts);
}
