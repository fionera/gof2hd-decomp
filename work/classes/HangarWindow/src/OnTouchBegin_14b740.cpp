#include "class.h"
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
int Layout_OnTouchBegin(void *layout, int touch);
int TouchButton_OnTouchBegin(void *btn, int touch);
void ChoiceWindow_OnTouchBegin(void *win, int touch);
void ChoiceWindow_setMsg(void *win, String12 *msg, bool flag);
void ListItemWindow_OnTouchBegin(void *win, int touch, int coord);
int __aeabi_idiv(int a, int b);
void *HangarList_getCurrentItem(void *list);
int HangarList_getCurrentLength(void *list);
int HangarList_getCurrentTab(void *list);
unsigned int HangarList_getCurrentItemIndex(void *list);
void HangarList_setCurrentItemIndex(void *list, int idx);
int ListItem_isShip(void *item);
int Item_getIndex(void *item);
int Item_getBlueprintAmount(void *item);
void *Item_extractItems(void *arr, bool flag);
void Ship_setCargo(void *ship, void *arr);
void *Status_getShip();
void *Status_getStation();
void Status_replaceHash(String12 *out, void *globals, String12 *a, String12 *b, String12 *c);
void Station_setItems(void *station, void *arr, bool flag);
int Station_getIndex(void *station);
int BluePrint_isEmpty(void *bp);
int BluePrint_getStationIndex(void *bp);
void *BluePrint_getStationName(void *bp);
void *GameText_getText(int id);
void AEString_ctor_empty(String12 *self);
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void Layout_formatCredits(String12 *out, int amount);
void HangarWindow_highlightItem(HangarWindow *self, void *item);
void HangarWindow_autoEquipSecondaryWeapons(HangarWindow *self, int row);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern int *g_hw_bpTextId;
extern "C" extern const char hw_otb_fmt1[], hw_otb_fmt2[];

extern "C" void HangarWindow_OnTouchBegin(HangarWindow *self, int touch, int coord)
{
    F<int>(self, 0x6c) = 0;
    F<int>(self, 0x70) = 0;
    unsigned char handled = (unsigned char)Layout_OnTouchBegin(*g_hw_layout, touch);

    if (F<uint8_t>(self, 0x3c) != 0) {
        if (F<uint8_t>(self, 0xae) != 0) {
            for (int i = 0xc; i != 0x11; i++)
                TouchButton_OnTouchBegin(G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4), touch);
            TouchButton_OnTouchBegin(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x44), touch);
        } else if (F<uint8_t>(self, 0xb0) != 0) {
            for (int i = 0x12; i != 0x17; i++)
                TouchButton_OnTouchBegin(G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4), touch);
        }
        ChoiceWindow_OnTouchBegin(F<void *>(self, 0x20), touch);
        return;
    }

    F<int>(self, 0xcc) = coord;
    F<int>(self, 0xb8) = coord;
    F<int>(self, 0xc0) = 0;
    F<uint8_t>(self, 0xd0) = 1;

    if (F<int>(self, 0x58) == 1) {
        ListItemWindow_OnTouchBegin(F<void *>(self, 0x18), touch, coord);
        return;
    }

    void *layout = *g_hw_layout;
    unsigned char skip = 1;
    if (G<int>(layout, 0xc) < coord && coord < *g_hw_screenWidth - G<int>(layout, 0x10)) {
        int row = __aeabi_idiv(
            ((coord - G<int>(layout, 0xc)) - G<int>(layout, 0x20)) - F<int>(self, 0x10c) -
                F<int>(self, 0xb4),
            G<int>(layout, 0x70) + F<int>(self, 0x10c));
        if (HangarList_getCurrentLength(F<void *>(self, 0x14)) > row) {
            HangarList_setCurrentItemIndex(F<void *>(self, 0x14), row);
            HangarWindow_highlightItem(self, HangarList_getCurrentItem(F<void *>(self, 0x14)));
            skip = 0;
            if (F<uint8_t>(self, 0x11d) != 0) {
                void *ci = HangarList_getCurrentItem(F<void *>(self, 0x14));
                if (ListItem_isShip(ci) != 0 &&
                    HangarList_getCurrentTab(F<void *>(self, 0x14)) == 1) {
                    Ship_setCargo(Status_getShip(), Item_extractItems(F<void *>(self, 0x10), true));
                    Station_setItems(Status_getStation(),
                                     Item_extractItems(F<void *>(self, 0x10), false), false);
                }
            }
        }
    }

    if (HangarList_getCurrentTab(F<void *>(self, 0x14)) == 4 && F<uint8_t>(self, 0x88) != 0 &&
        !(!skip && F<void *>(self, 0x68) == F<void *>(self, 0x84)) && F<int>(self, 0x94) > 0 &&
        F<uint8_t>(self, 0x3c) == 0 && BluePrint_isEmpty(F<void *>(self, 0x80)) == 0) {
        if (BluePrint_getStationIndex(F<void *>(self, 0x80)) != Station_getIndex(Status_getStation())) {
            void *globals = *g_hw_globals;
            int bpIdx = Item_getIndex(G<void *>(F<void *>(self, 0x84), 0x10));
            bool localBp = (bpIdx == 0xd1) ||
                           (Item_getIndex(G<void *>(F<void *>(self, 0x84), 0x10)) == 0xcc);
            F<uint8_t>(self, 0x11e) = localBp;

            String12 msg;
            AEString_ctor_empty(&msg);
            String12 line;
            AEString_ctor(&line, (const char *)GameText_getText(*g_hw_bpTextId), false);
            AEString_assign(&msg, &line);
            AEString_dtor(&line);

            if (F<uint8_t>(self, 0x11e) == 0) {
                String12 copy, sname, fmt, result;
                AEString_ctor_str(&copy, &msg, false);
                BluePrint_getStationName(F<void *>(self, 0x80));
                AEString_ctor(&sname, hw_otb_fmt1, false);
                AEString_ctor(&fmt, hw_otb_fmt1, false);
                Status_replaceHash(&result, globals, &copy, &sname, &fmt);
                AEString_assign(&msg, &result);
                AEString_dtor(&result);
                AEString_dtor(&fmt);
                AEString_dtor(&sname);
                AEString_dtor(&copy);

                String12 copy2, priceStr, fmt2, result2;
                AEString_ctor_str(&copy2, &msg, false);
                Layout_formatCredits(&priceStr,
                                     Item_getBlueprintAmount(G<void *>(F<void *>(self, 0x84), 0x10)));
                AEString_ctor(&fmt2, hw_otb_fmt2, false);
                Status_replaceHash(&result2, globals, &copy2, &priceStr, &fmt2);
                AEString_assign(&msg, &result2);
                AEString_dtor(&result2);
                AEString_dtor(&fmt2);
                AEString_dtor(&priceStr);
                AEString_dtor(&copy2);
            }
            bool flag = (F<uint8_t>(self, 0x11e) == 0);
            ChoiceWindow_setMsg(F<void *>(self, 0x20), &msg, flag);
            F<uint8_t>(self, 0x3c) = 1;
            F<uint8_t>(self, 0xac) = 1;
            F<uint8_t>(self, 0xd1) = 1;
            AEString_dtor(&msg);
            return;
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->length; i++)
        handled |= (unsigned char)TouchButton_OnTouchBegin(tabs->data[i], touch);

    Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
    for (unsigned int i = 0; i < buttons->length; i++) {
        void *btn = buttons->data[i];
        if (btn != 0)
            TouchButton_OnTouchBegin(btn, touch);
    }

    if (F<uint8_t>(self, 0xf8) != 0 && HangarList_getCurrentTab(F<void *>(self, 0x14)) == 1) {
        int idx = F<int>(self, 0xfc);
        if (idx >= 0 &&
            !(!handled && (unsigned int)idx == HangarList_getCurrentItemIndex(F<void *>(self, 0x14)))) {
            F<uint8_t>(self, 0xf8) = 0;
            HangarWindow_autoEquipSecondaryWeapons(self, idx);
        }
    }
}
