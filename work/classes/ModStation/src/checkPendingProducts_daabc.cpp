#include "class.h"

struct Station;
struct Item;
struct PendingProduct;

__attribute__((visibility("hidden"))) extern int  *g_cpp_stack;     // [DAT_000ead34]
__attribute__((visibility("hidden"))) extern int **g_cpp_status;    // [DAT_000ead38]
__attribute__((visibility("hidden"))) extern int  *g_cpp_textId;    // [DAT_000ead3c]
__attribute__((visibility("hidden"))) extern int **g_cpp_itemTable; // [DAT_000ead44]

extern "C" {
int   Status_getCurrentCampaignMission_cpp();
int   GameText_getText_cpp(int id);
int   Status_getPendingProducts_cpp(int status);
int   Status_getStation_cpp();
int   Station_getIndex_cpp(Station *s);
int   Status_getShip_cpp();
int   Item_makeItem_cpp(int itemDesc);
void  Ship_addCargo_cpp(int ship, Item *it);
int   Item_getAmount_cpp();
int   Item_getIndex_cpp(Item *it);
void *PendingProduct_dtor_cpp(PendingProduct *p);
void  operator_delete_cpp(void *p);
void  ChoiceWindow_setNotice_cpp(void *cw);
// Appends one "received N x <item>" line to the running notification string (the original builds
// this with a long chain of AbyssEngine::String temporaries the decompiler mangled).
void  ModStation_cpp_appendDeliveryLine(int amount, int itemTextId);
}

// ModStation::checkPendingProducts() — on docking, delivers any queued station-production goods to
// the player's cargo hold and shows a summary notice.
extern "C" void ModStation_checkPendingProducts(ModStation *self)
{
    int camp = Status_getCurrentCampaignMission_cpp();
    int textId = *(int *)g_cpp_textId;
    GameText_getText_cpp(textId); // seed the summary header string

    unsigned *products = (unsigned *)Status_getPendingProducts_cpp(*(int *)g_cpp_status);
    if (products != 0) {
        int byteOff = 0;
        int *itemTable = *g_cpp_itemTable;
        for (unsigned i = 0; i < *products; i = i + 1) {
            int pp = *(int *)(products[1] + byteOff);
            if (pp != 0) {
                int dstStation = *(int *)(pp + 0xc);
                Station *here = (Station *)Status_getStation_cpp();
                if (dstStation == Station_getIndex_cpp(here)) {
                    if (camp == 0x92 && *(int *)(pp + 0x14) == 0xd2) {
                        // mission 0x92 special-cases this product: drop it silently.
                        PendingProduct *o = *(PendingProduct **)(products[1] + byteOff);
                        if (o != 0)
                            operator_delete_cpp(PendingProduct_dtor_cpp(o));
                        *(int *)(products[1] + byteOff) = 0;
                        goto done;
                    }
                    Item *it = (Item *)Item_makeItem_cpp(
                        *(int *)(*(int *)(*itemTable + 4) + *(int *)(pp + 0x14) * 4));
                    Ship_addCargo_cpp(Status_getShip_cpp(), it);
                    ModStation_cpp_appendDeliveryLine(Item_getAmount_cpp(), Item_getIndex_cpp(it));

                    PendingProduct *o = *(PendingProduct **)(products[1] + byteOff);
                    if (o != 0)
                        operator_delete_cpp(PendingProduct_dtor_cpp(o));
                    *(int *)(products[1] + byteOff) = 0;
                    C(self, 0x63) = 1; // any-delivered flag
                }
            }
            byteOff = byteOff + 4;
        }
    }

    if (C(self, 0x63) != 0)
        ChoiceWindow_setNotice_cpp(P(self, 0x70));
done:;
}
