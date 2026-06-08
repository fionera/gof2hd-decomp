#include "class.h"
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
unsigned int HangarList_getCurrentTab(void *list);
int Item_isUnsaleable(void *item);
unsigned int Item_transaction(void *item, bool buy, int amount, bool flag);
int Item_getIndex(void *item);
int Item_getSinglePrice(void *item);
int Item_getAmount(void *item);
void Item_setAmount(void *item, int amount);
int Item_getBlueprintAmount(void *item);
void Item_setBlueprintAmount(void *item, int amount);
int Item_transactionBlueprint(void *item, int z);
void *Item_makeItem(void *item);
int Status_getCredits();
void Status_changeCredits(void *globals);
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void *Status_getShip();
int BluePrint_getRemainingAmount(void *bp, int index);
void Ship_changeLoad(void *ship, int delta);
void *Ship_getCargo(void *ship);
void *GameText_getText(int id);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);
void Layout_formatCredits(String12 *out, int price, int credits);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void TouchButton_resetTouch(void *btn);
void ChoiceWindow_setText(void *win, void *text);
int ListItem_getIndex(void *item);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_unsaleableTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_notEnoughTextId;
extern "C" extern const char hw_tx_fmt[], hw_tx_suffix[];

extern "C" void HangarWindow_transaction(HangarWindow *self, bool buy)
{
    unsigned int tab = HangarList_getCurrentTab(F<void *>(self, 0x14));
    void *cur = G<void *>(F<void *>(self, 0x68), 0x10);

    if (tab < 2) {
        if (Item_isUnsaleable(cur) != 0) {
            GameText_getText(*g_hw_unsaleableTextId);
            ChoiceWindow_setText(F<void *>(self, 0x20), 0);
            F<uint8_t>(self, 0x88) = 0;
            F<uint8_t>(self, 0x3c) = 1;
            return;
        }

        unsigned int result = Item_transaction(cur, buy, F<int>(self, 0xa8), F<uint8_t>(self, 0x11d));
        unsigned int idx = Item_getIndex(cur);
        void *globals = *g_hw_globals;
        unsigned int *avail = G<unsigned int *>(globals, 0x54);
        if (idx < avail[0])
            *((uint8_t *)avail[1] + Item_getIndex(cur)) = 1;

        if (result >= 0x80000000 && buy) {
            F<int>(self, 0xa8) = F<int>(self, 0xa8) + 1;
            Ship_changeLoad(Status_getShip(), 1);
            int li = ListItem_getIndex(F<void *>(self, 0x68));
            if (li >= 0x84 && ListItem_getIndex(F<void *>(self, 0x68)) < 0x9a) {
                int base = G<int>(globals, 0xac);
                *((uint8_t *)G<int>((void *)(uintptr_t)base, 4) +
                  ListItem_getIndex(F<void *>(self, 0x68)) - 0x84) = 1;
            }
        } else if (result == 0 && buy) {
            if (Status_getCredits() < Item_getSinglePrice(cur)) {
                if (F<uint8_t>(self, 0x11d) != 0)
                    return;
                String12 line, priceStr, fmt, msg, suffix, combined;
                AEString_ctor(&line, (const char *)GameText_getText(*g_hw_notEnoughTextId), false);
                Layout_formatCredits(&priceStr, Item_getSinglePrice(cur), Status_getCredits());
                AEString_ctor_str(&line, &priceStr, false);
                AEString_ctor(&fmt, hw_tx_fmt, false);
                Status_replaceHash(&msg, globals, &line, &priceStr, &fmt);
                AEString_dtor(&fmt);
                AEString_dtor(&priceStr);
                AEString_ctor(&suffix, hw_tx_suffix, false);
                GameText_getText(*g_hw_notEnoughTextId);
                AEString_add(&combined, &suffix, &suffix);
                AEString_addAssign(&msg, &combined);
                AEString_dtor(&combined);
                AEString_dtor(&suffix);
                ChoiceWindow_setMsg(F<void *>(self, 0x20), &msg, true);
                F<uint8_t>(self, 0x3c) = 1;
                F<uint8_t>(self, 0xaf) = 1;
                TouchButton_resetTouch(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x20));
                TouchButton_resetTouch(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x24));
                AEString_dtor(&msg);
            }
        } else if ((int)result > 0 && !buy) {
            F<int>(self, 0xa8) = F<int>(self, 0xa8) - 1;
            Ship_changeLoad(Status_getShip(), -1);
        }

        if (F<uint8_t>(self, 0x11d) == 0)
            Status_changeCredits(globals);
    } else if (tab == 4) {
        if (buy) {
            int bpAmt = Item_getBlueprintAmount(cur);
            void *bp = F<void *>(self, 0x80);
            int remaining = BluePrint_getRemainingAmount(bp, Item_getIndex(cur));
            if (bpAmt < remaining) {
                int r = Item_transactionBlueprint(cur, 0);
                if (r < 0) {
                    F<int>(self, 0xa8) = F<int>(self, 0xa8) + 1;
                } else if (r != 0) {
                    F<int>(self, 0x94) = F<int>(self, 0x94) + 1;
                    Ship_changeLoad(Status_getShip(), -1);
                }
            }
        }
        void *cargo = Ship_getCargo(Status_getShip());
        if (cargo != 0) {
            Array<void *> *arr = (Array<void *> *)cargo;
            for (unsigned int i = 0; i < arr->length; i++) {
                if (Item_getIndex(arr->data[i]) == Item_getIndex(cur)) {
                    Item_setAmount(arr->data[i], Item_getAmount(arr->data[i]));
                    Item_setBlueprintAmount(arr->data[i], Item_getBlueprintAmount(cur));
                }
            }
        }
    }
}
