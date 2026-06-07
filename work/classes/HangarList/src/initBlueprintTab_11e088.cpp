#include "class.h"

void HangarList::initBlueprintTab(Array<BluePrint *> *blueprints) {
    Array<Array<ListItem *> *> *tabs =
        F<Array<Array<ListItem *> *> *>(this, 0x0);
    Array<ListItem *> **slot = tabs->data + 2;
    Array<ListItem *> *old = *slot;
    if (old != 0) {
        if (old->length != 0) {
            ArrayReleaseClasses_ListItem(old);
            slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 2;
            old = *slot;
            if (old == 0) {
                goto clear_old;
            }
        }
        operator_delete(Array_ListItem_dtor(old));
        slot = F<Array<Array<ListItem *> *> *>(this, 0x0)->data + 2;
    }
clear_old:
    *slot = 0;

    Array<ListItem *> *list = (Array<ListItem *> *)operator_new(0xc);
    Array_ListItem_ctor(list);

    uint32_t unlocked = 1;
    for (uint32_t i = 0; i < blueprints->length; ++i) {
        unlocked += BluePrint_isUnlocked(blueprints->data[i]);
    }

    Array<PendingProduct *> *pending =
        Status_getPendingProducts(g_HangarList_status);
    uint32_t length = unlocked;
    bool noPending = true;
    if (pending != 0) {
        uint32_t i = 0;
        while (pending->length != i) {
            if (pending->data[i] != 0) {
                ++length;
            }
            ++i;
        }
        noPending = length <= unlocked;
        if (unlocked < length) {
            ++length;
        }
    }

    ArraySetLength_ListItem(length, list);

    ListItem *li = (ListItem *)operator_new(0x48);
    GameText **texts = g_HangarList_gameText;
    ListItem_ctor_String(li, GameText_getText(*texts, 0x111));
    list->data[0] = li;

    uint32_t out = 1;
    for (uint32_t i = 0; i < blueprints->length; ++i) {
        if (BluePrint_isUnlocked(blueprints->data[i]) != 0) {
            li = (ListItem *)operator_new(0x48);
            ListItem_ctor_BluePrint(li, blueprints->data[i]);
            list->data[out] = li;
            ++out;
        }
    }

    if (!noPending) {
        li = (ListItem *)operator_new(0x48);
        ListItem_ctor_String(li, GameText_getText(*texts, 0x112));
        uint32_t pendingOut = out + 1;
        list->data[out] = li;
        for (uint32_t i = 0; i < pending->length; ++i) {
            if (pending->data[i] != 0) {
                li = (ListItem *)operator_new(0x48);
                ListItem_ctor_PendingProduct(li, pending->data[i]);
                list->data[pendingOut] = li;
                ++pendingOut;
            }
        }
    }

    F<Array<Array<ListItem *> *> *>(this, 0x0)->data[2] = list;
}
