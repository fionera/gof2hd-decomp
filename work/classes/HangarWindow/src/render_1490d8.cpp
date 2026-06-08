#include "class.h"
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
void PaintCanvas_SetColor(unsigned int color);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);
int PaintCanvas_GetTextWidth(void *canvas, void *text);
int PaintCanvas_GetTextHeight(void *canvas);
void PaintCanvas_DrawImage2D(void *canvas, int img, int x, int y, char anchor);
void PaintCanvas_DrawRegion2D(void *canvas, unsigned int img, int sx, int sy, int sw, int sh,
                              float fw, int a, int b, int c, int y);
void PaintCanvas_DrawString(void *canvas, void *font, int text, int x, char anchor);

unsigned int HangarList_getCurrentTab(void *list);
void *HangarList_getCurrentTabItems(void *list);

int ListItem_isSelectable(void *item);
int ListItem_isTextButton(void *item);
int ListItem_isItem(void *item);
int ListItem_isShip(void *item);
int ListItem_isSlot(void *item);
int ListItem_isBluePrint(void *item);
int ListItem_isPendingProduct(void *item);
int ListItem_isMoveToCargoButton(void *item);
int ListItem_isSellButton(void *item);

void Layout_drawBG(void *layout);
void Layout_drawBox(void *layout, int kind, int x, int y, int w, int h, String12 *text);
void Layout_drawScrollBar(void *layout, int a, int b, int c, int d, int e);
void Layout_drawHeader(void *layout, String12 *text);
void Layout_drawFooter(void *layout);
void Layout_formatCredits(String12 *out, int amount);

int BluePrint_getIndex(void *bp);
float BluePrint_getCompletionRate(void *bp);
int BluePrint_getCurrentAmount(void *bp, int idx);
int BluePrint_getTotalAmount(void *bp, int idx);

int Item_getIndex(void *item);
int Item_getType(void *item);
int Item_getSort(void *item);
int Item_getAmount(void *item);
int Item_getStationAmount(void *item);
int Item_getBlueprintAmount(void *item);
int Item_getSinglePrice(void *item);
int Item_canBeInstalledMultipleTimes(void *item);
int Item_isInList(void *item, void *arr);

int Ship_getIndex(void *ship);
int Ship_getPrice(void *ship);
int Ship_hasEquipment(void *ship, int idx);
int Ship_getFreeSlots(void *ship, int type);
void *Ship_getFirstEquipmentOfSort(void *ship, int sort);

void *Status_getShip();
void *Status_getStation();
int Status_getCredits();
void *Station_getItems(void *station);

void *GameText_getText(int id);

void ImageFactory_drawShip(void *factory, int idx, int x, int y);
void ImageFactory_drawItem(void *factory, int idx, int type, int x, int y);

void TouchButton_setVisible(void *btn, bool vis);
void TouchButton_setPosition(void *btn, int x, int y, char anchor);
void TouchButton_setPosition2(void *btn, int x, int y);
void TouchButton_setAlwaysPressed(void *btn, bool flag);
void TouchButton_setText(void *btn, void *text);
void TouchButton_setSplitText(void *btn, void *text);
void TouchButton_replaceTextKeepSize(void *btn, void *text);
int TouchButton_getWidth(void *btn);
int TouchButton_getHeight(void *btn);
void TouchButton_draw(void *btn);

void ChoiceWindow_draw(void *win);
void ChoiceWindow_setHeight(void *win, int h);

void ListItemWindow_draw(void *win);

void *AppManager_GetApplicationData();
void *AppManager_GetApplicationModule(unsigned int id);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_ctor_int(String12 *self, int v);
void AEString_ctor_str(String12 *self, String12 *src, bool copy);
void AEString_dtor(String12 *self);
void AEString_assign(String12 *self, String12 *src);
void AEString_add(String12 *out, String12 *a, String12 *b);
void AEString_addAssign(String12 *self, String12 *other);

int __aeabi_idiv(int a, int b);
unsigned int __aeabi_uidiv(unsigned int a, unsigned int b);

float HangarWindow_getRelativeScrollStartPos(HangarWindow *self);
float HangarWindow_getRelativeScrollHeight(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;        // *piVar37
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;        // *puVar19
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;     // *piVar21
__attribute__((visibility("hidden"))) extern int *g_hw_screenHeight;    // *piVar41
__attribute__((visibility("hidden"))) extern void **g_hw_globals;
__attribute__((visibility("hidden"))) extern void **g_hw_font;
__attribute__((visibility("hidden"))) extern void **g_hw_recordHandler;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_optionFlags;
__attribute__((visibility("hidden"))) extern unsigned int *g_hw_dlcModuleId;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_blackMarketHintFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_introHintFlag;
__attribute__((visibility("hidden"))) extern int *g_hw_headerTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_shipNameBase;
__attribute__((visibility("hidden"))) extern int *g_hw_itemNameBase;
__attribute__((visibility("hidden"))) extern float g_hw_lineScale;
extern "C" extern const char hw_rnd_empty[], hw_rnd_a[], hw_rnd_b[], hw_rnd_c[],
    hw_rnd_d[], hw_rnd_e[], hw_rnd_x[];

extern "C" void HangarWindow_render(HangarWindow *self)
{
    void *layout = *g_hw_layout;
    void *canvas = *g_hw_canvas;
    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);

    void *dlc = AppManager_GetApplicationModule(*g_hw_dlcModuleId);
    bool dlcShown = dlc != 0 && G<uint8_t>(dlc, 0x18) != 0;

    if (dlc == 0 || !dlcShown) {
        int tab2 = self->f_58;
        if (tab2 == 0) {
            Layout_drawBG(layout);
            unsigned int tab = HangarList_getCurrentTab(self->f_14);
            Array<void *> *items = (Array<void *> *)HangarList_getCurrentTabItems(self->f_14);
            if (items != 0) {
                float startPos = HangarWindow_getRelativeScrollStartPos(self);
                float visH = (float)self->f_d8;
                float scrollH = HangarWindow_getRelativeScrollHeight(self);
                int scrollPx = (int)(scrollH * visH);
                int startPx = (int)(startPos * visH);

                int topY = *g_hw_screenWidth;
                if (scrollPx > 0)
                    topY = (topY - G<int>(layout, 0x48)) - G<int>(layout, 0x2c);

                int rowGap;
                if (tab == 0)
                    rowGap = G<int>(layout, 0x4c) << 1;
                else if (tab == 4 || tab == 1)
                    rowGap = G<int>(layout, 0x50);
                else
                    rowGap = 0;

                topY += (G<int>(layout, 0x28) + self->f_f4) * -2;
                int baseY = G<int>(layout, 0x2cc);
                int colW = G<int>(layout, 0x4c);

                // Black-market scrolling hint strip.
                if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
                    int iw = PaintCanvas_GetImage2DWidth(canvas);
                    int ih = PaintCanvas_GetImage2DHeight(canvas);
                    int rows = __aeabi_idiv(*g_hw_screenHeight, 1);
                    int y = 0;
                    for (int r = 0; r <= rows; r++) {
                        PaintCanvas_DrawImage2D(canvas, self->f_f0,
                            (G<int>(layout, 0x28) - iw) + self->f_f4, y, 1);
                        int off = (scrollPx < 1) ? 0 : (G<int>(layout, 0x48) + G<int>(layout, 0x2c));
                        PaintCanvas_DrawImage2D(canvas, self->f_f0,
                            self->f_f4 + G<int>(layout, 0x28) + topY + off, y, 0);
                        y += ih;
                    }
                }

                int contentBase = colW + baseY + rowGap;

                // Hide all action buttons before re-laying them out per row.
                Array<void *> *btnArr = (Array<void *> *)self->f_24;
                for (int i = 0; i != 0x18; i++) {
                    if (self->f_d0 == 0) {
                        void *btn = G<void *>(G<void *>(self->f_24, 4), i * 4);
                        if (btn != 0)
                            TouchButton_setVisible(btn, false);
                    }
                }

                int boxW = rowGap - 2;

                for (unsigned int i = 0; i < items->length; i++) {
                    int y = (G<int>(layout, 0x70) + self->f_10c) * (int)i +
                            self->f_b4 + G<int>(layout, 0x20) + G<int>(layout, 0xc);
                    if (y < 0 || y > *g_hw_screenHeight)
                        continue;

                    void *li = items->data[i];
                    if (ListItem_isSelectable(li) == 0)
                        continue;

                    // Row background box (selected vs unselected, by tab).
                    if (self->f_68 == li && ListItem_isTextButton(li) == 0) {
                        String12 boxText;
                        if (tab == 0 && G<int>(li, 0x3c) >= 0) {
                            AEString_ctor(&boxText, hw_rnd_a, false);
                            Layout_drawBox(layout, 10, self->f_f4 + G<int>(layout, 0x28), y,
                                           topY, G<int>(layout, 0x70), &boxText);
                        } else {
                            AEString_ctor(&boxText, hw_rnd_b, false);
                            Layout_drawBox(layout, 4, self->f_f4 + G<int>(layout, 0x28), y,
                                           topY, G<int>(layout, 0x70), &boxText);
                        }
                        AEString_dtor(&boxText);
                    } else if (tab != 0 || G<int>(li, 0x3c) < 0) {
                        String12 boxText;
                        AEString_ctor(&boxText, hw_rnd_c, false);
                        Layout_drawBox(layout, 3, self->f_f4 + G<int>(layout, 0x28), y, topY,
                                       G<int>(layout, 0x70), &boxText);
                        AEString_dtor(&boxText);
                    } else {
                        String12 boxText;
                        AEString_ctor(&boxText, hw_rnd_d, false);
                        Layout_drawBox(layout, 9, self->f_f4 + G<int>(layout, 0x28), y, topY,
                                       G<int>(layout, 0x70), &boxText);
                        AEString_dtor(&boxText);
                    }

                    PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                    String12 label;
                    AEString_ctor(&label, hw_rnd_empty, false);

                    if (ListItem_isItem(li) == 0) {
                        if (ListItem_isShip(li) != 0) {
                            // Ship row: name + price + ship icon.
                            Ship_getIndex(G<void *>(li, 0xc));
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_shipNameBase));
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            String12 price;
                            Layout_formatCredits(&price, Ship_getPrice(G<void *>(li, 0xc)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + G<int>(layout, 0x28) + self->f_f4, 1);
                            AEString_dtor(&price);
                            ImageFactory_drawShip(*g_hw_globals, Ship_getIndex(G<void *>(li, 0xc)),
                                self->f_f4 + G<int>(layout, 0x28) + rowGap,
                                self->f_118 + y);
                            PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        } else if (ListItem_isSlot(li) != 0) {
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            if (tab == 4 && i == items->length - 1) {
                                TouchButton_setPosition(
                                    G<void *>(G<void *>(self->f_24, 4), 0x5c),
                                    self->f_f4 + G<int>(layout, 0x28) + topY / 2,
                                    G<int>(layout, 0x114) + y, 0x14);
                                TouchButton_setVisible(
                                    G<void *>(G<void *>(self->f_24, 4), 0x5c), true);
                                TouchButton_draw(G<void *>(G<void *>(self->f_24, 4), 0x5c));
                                String12 tmp;
                                AEString_ctor(&tmp, hw_rnd_x, false);
                                AEString_assign(&label, &tmp);
                                AEString_dtor(&tmp);
                            }
                        } else if (ListItem_isBluePrint(li) != 0) {
                            BluePrint_getIndex(G<void *>(li, 0x8));
                            AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            float rate = BluePrint_getCompletionRate(G<void *>(li, 0x8));
                            if (rate > 0.0f) {
                                PaintCanvas_DrawImage2D(canvas, self->f_78,
                                    G<int>(layout, 0x28) + contentBase + 2 + self->f_f4, 0, 0);
                                float dcw = (float)self->f_dc;
                                PaintCanvas_DrawRegion2D(canvas, self->f_7c, 0, 0,
                                    (int)(rate * dcw), self->f_e0, (float)(int)(rate * dcw),
                                    0, 0, 0,
                                    G<int>(layout, 0x28) + contentBase + 3 + self->f_f4);
                                String12 pct, sfx, sum;
                                AEString_ctor_int(&pct, (int)(rate * startPos));
                                AEString_ctor(&sfx, hw_rnd_x, false);
                                AEString_add(&sum, &pct, &sfx);
                                PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&sum,
                                    contentBase + 2 + G<int>(layout, 0x28) + self->f_f4 +
                                        self->f_dc + G<int>(layout, 0x2c), 0);
                                AEString_dtor(&sum);
                                AEString_dtor(&sfx);
                                AEString_dtor(&pct);
                                PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                            }
                            int bpIdx = BluePrint_getIndex(G<void *>(li, 0x8));
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), bpIdx));
                            ImageFactory_drawItem(*g_hw_globals, bpIdx, type,
                                G<int>(layout, 0x28) + rowGap + self->f_f4,
                                self->f_118 + y);
                        } else if (ListItem_isPendingProduct(li) != 0) {
                            int amt = G<int>(G<void *>(li, 0x18), 0x10);
                            String12 head;
                            if (amt < 2) {
                                AEString_ctor(&head, hw_rnd_e, false);
                            } else {
                                String12 num, sfx;
                                AEString_ctor_int(&num, amt);
                                AEString_ctor(&sfx, hw_rnd_e, false);
                                AEString_add(&head, &num, &sfx);
                                AEString_dtor(&sfx);
                                AEString_dtor(&num);
                            }
                            String12 full;
                            AEString_add(&full, &head, (String12 *)GameText_getText(*g_hw_itemNameBase));
                            AEString_assign(&label, &full);
                            AEString_dtor(&full);
                            AEString_dtor(&head);
                            int pidx = G<int>(G<void *>(li, 0x18), 0x14);
                            int type = Item_getType(G<void *>(G<void *>(*g_hw_globals, 0x4), pidx));
                            ImageFactory_drawItem(*g_hw_globals, pidx, type,
                                rowGap + G<int>(layout, 0x28) + self->f_f4,
                                self->f_118 + y);
                        } else if (ListItem_isMoveToCargoButton(li) != 0) {
                            TouchButton_setPosition(
                                G<void *>(G<void *>(self->f_24, 4), 0x18),
                                self->f_f4 + G<int>(layout, 0x28), y, 0x11);
                            TouchButton_setVisible(
                                G<void *>(G<void *>(self->f_24, 4), 0x18), true);
                            TouchButton_draw(G<void *>(G<void *>(self->f_24, 4), 0x18));
                        } else if (ListItem_isSellButton(li) != 0) {
                            TouchButton_setPosition(
                                G<void *>(G<void *>(self->f_24, 4), 0x14),
                                self->f_f4 + G<int>(layout, 0x28), y, 0x11);
                            TouchButton_setVisible(
                                G<void *>(G<void *>(self->f_24, 4), 0x14), true);
                            TouchButton_draw(G<void *>(G<void *>(self->f_24, 4), 0x14));
                        } else {
                            String12 txt;
                            AEString_ctor_str(&txt, G<String12 *>(li, 0x1c), false);
                            Layout_drawBox(layout, 0, self->f_f4 + G<int>(layout, 0x28),
                                           (y + G<int>(layout, 0x70)) - G<int>(layout, 0x1c), topY,
                                           G<int>(layout, 0x1c), &txt);
                            AEString_dtor(&txt);
                        }
                    } else {
                        // Regular item row: name + amounts + price + icon.
                        Item_getIndex(G<void *>(li, 0x10));
                        AEString_assign(&label, (String12 *)GameText_getText(*g_hw_itemNameBase));
                        PaintCanvas_SetColor((unsigned int)(uintptr_t)canvas);
                        if (F<uint8_t>(self, 0x11d) == 0) {
                            String12 price;
                            Item_getSinglePrice(G<void *>(li, 0x10));
                            Layout_formatCredits(&price, Item_getSinglePrice(G<void *>(li, 0x10)));
                            PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&price,
                                contentBase + G<int>(layout, 0x28) + self->f_f4, 1);
                            AEString_dtor(&price);
                        }
                        int iidx = Item_getIndex(G<void *>(li, 0x10));
                        int itype = Item_getType(G<void *>(li, 0x10));
                        ImageFactory_drawItem(*g_hw_globals, iidx, itype,
                            G<int>(layout, 0x28) + rowGap + self->f_f4,
                            self->f_118 + y);
                    }

                    PaintCanvas_DrawString(canvas, *g_hw_font, (int)(uintptr_t)&label,
                        self->f_f4 + G<int>(layout, 0x28) + contentBase, 0);

                    AEString_dtor(&label);
                }

                // Scroll bar when the content overflows.
                if (scrollPx > 0 || startPx > 0) {
                    Layout_drawScrollBar(layout,
                        ((*g_hw_screenHeight - G<int>(layout, 0x48)) - G<int>(layout, 0x28)) -
                            self->f_f4,
                        G<int>(layout, 0x20) + G<int>(layout, 0xc), self->f_d8, startPx,
                        scrollPx);
                }
            }

            String12 header;
            AEString_ctor(&header, (const char *)GameText_getText(*g_hw_headerTextId), false);
            Layout_drawHeader(layout, &header);
            AEString_dtor(&header);

            Array<void *> *tabs = (Array<void *> *)self->f_4;
            for (unsigned int i = 0; i < tabs->length; i++)
                TouchButton_draw(tabs->data[i]);
        }

        if (self->f_58 == 1) {
            self->f_58 = 0;
            HangarWindow_render(self);
            self->f_58 = 1;
            ListItemWindow_draw(self->f_18);
        }
    }

    // --- Footer + credits button (always drawn). ---
    layout = *g_hw_layout;
    Layout_drawFooter(layout);
    void *btns = self->f_24;
    TouchButton_setVisible(G<void *>(G<void *>(btns, 4), 0x2c), true);
    TouchButton_setAlwaysPressed(G<void *>(G<void *>(btns, 4), 0x2c), g_hw_optionFlags[0x4e] == 0);
    {
        String12 credits;
        Layout_formatCredits(&credits, Status_getCredits());
        TouchButton_setText(G<void *>(G<void *>(btns, 4), 0x2c), &credits);
        AEString_dtor(&credits);
    }
    TouchButton_draw(G<void *>(G<void *>(btns, 4), 0x2c));

    if (self->f_3c == 0)
        return;

    ChoiceWindow_draw(self->f_20);

    if (F<uint8_t>(self, 0xae) == 0) {
        if (self->f_b0 != 0) {
            // Free-credits action button column.
            for (unsigned int i = 0; i < 5; i++) {
                void *b = G<void *>(G<void *>(btns, 4), i * 4 + 0x48);
                bool vis;
                if (i == 0 || (i == 1 && g_hw_optionFlags[0x49] != 0) ||
                    (i == 2 && g_hw_optionFlags[0x4a] != 0) ||
                    (i == 3 && g_hw_optionFlags[0x4b] != 0))
                    vis = false;
                else
                    vis = (i != 4 || g_hw_optionFlags[0x4c] == 0);
                TouchButton_setVisible(b, vis);
                TouchButton_setPosition2(b, G<int>(layout, 0x28) + G<int>(self->f_20, 0),
                                         G<int>(layout, 0x8));
                TouchButton_draw(b);
            }
        }
    } else {
        // Buy-credits / IAP grid.
        void *appData = AppManager_GetApplicationData();
        (void)appData;
        if (F<uint8_t>(self, 0x11f) == 0) {
            // Five fixed-label buttons.
            for (int slot = 0x30; slot <= 0x40; slot += 4) {
                void *b = G<void *>(G<void *>(btns, 4), slot);
                TouchButton_setVisible(b, true);
                String12 t;
                AEString_ctor(&t, hw_rnd_x, false);
                TouchButton_setText(b, &t);
                AEString_dtor(&t);
                TouchButton_setPosition(b, *g_hw_screenWidth / 2, 0, 0x14);
                TouchButton_draw(b);
            }
        } else {
            // List-mode: per-index labelled buttons with icons.
            for (unsigned int i = 0; i <= 4; i++) {
                void *b = G<void *>(G<void *>(btns, 4), i * 4 + 0x30);
                TouchButton_setVisible(b, true);
                String12 label, split;
                switch (i) {
                case 0: AEString_ctor(&label, hw_rnd_a, false); AEString_ctor(&split, hw_rnd_b, false); break;
                case 1: AEString_ctor(&label, hw_rnd_b, false); AEString_ctor(&split, hw_rnd_c, false); break;
                case 2: AEString_ctor(&label, hw_rnd_c, false); AEString_ctor(&split, hw_rnd_d, false); break;
                case 3: AEString_ctor(&label, hw_rnd_d, false); AEString_ctor(&split, hw_rnd_e, false); break;
                default: AEString_ctor(&label, hw_rnd_e, false); AEString_ctor(&split, hw_rnd_x, false); break;
                }
                unsigned int rowOff = __aeabi_uidiv(i & 0xff, 3);
                int x = (self->f_128 + self->f_120) *
                            ((i + rowOff * -3) & 0xff) +
                        ((*g_hw_screenHeight / 2 - self->f_120) - self->f_128);
                int yy = (int)((float)((G<int>(layout, 0x20) * -3) +
                            (*g_hw_screenWidth / 2 - self->f_124 / 2)) +
                            (float)self->f_12c * -0.5f +
                            (float)((int)rowOff * (self->f_12c + self->f_124)));
                TouchButton_setPosition(b, x, yy, 'D');
                TouchButton_replaceTextKeepSize(b, &label);
                TouchButton_setSplitText(b, &split);
                TouchButton_draw(b);
                PaintCanvas_DrawImage2D(canvas, G<int>(self->f_30, i * 4), x,
                                        yy - G<int>(layout, 0x2c), 0x11);
                AEString_dtor(&split);
                AEString_dtor(&label);
            }
        }

        if (F<uint8_t>(self, 0x11f) == 0) {
            int h17 = G<int>(layout, 0x30);
            int h34 = G<int>(layout, 0x34);
            int th = PaintCanvas_GetTextHeight(canvas);
            ChoiceWindow_setHeight(self->f_20, (h34 + h17) * 6 + th * 2);
        }
    }
}
