#include "class.h"
typedef unsigned int uintptr_t;

struct String12 { uint32_t a, b, c; };

extern "C" {
void *Status_getStation();
int Station_getIndex(void *station);
void *Station_getItems(void *station);
void *Station_getShips(void *station);
void Status_calcCargoPrices(void *status);
void *Status_getShip();
void *Status_getSystem();
void *Status_getBluePrints(void *status);
int Status_getCredits();
int Status_getCurrentCampaignMission();
int Status_inBlackMarketSystem();
int SolarSystem_getIndex(void *sys);
void *Ship_getCargo(void *ship);
void Ship_adjustPrice(void *ship);
int Ship_getCurrentLoad(void *ship);
void *Ship_getEquipment(void *ship);
void *Item_mixItems(void *a, void *b);
int Item_getSinglePrice(void *item);
int Item_getIndex(void *item);
void *operator_new(unsigned int n);
void *operator_new__(unsigned int n);
int __aeabi_idiv(int a, int b);

void HangarList_ctor(void *list);
void HangarList_init(void *list, void *ship, void *items, void *ships, void *bps);
void HangarList_setCurrentTab(void *list, bool flag);

void Array_TouchButtonPtr_ctor(void *arr);
void ArraySetLength_TouchButtonPtr(int len, void *arr);

void *GameText_getText(int id);
int Layout_getHelpButtonOffset();
int Layout_getFooterTransitionWidth();
void Layout_formatCredits(String12 *out, int credits);

// TouchButton constructor overloads (distinct argument arities in the target).
void TouchButton_ctor_text(void *btn, void *text, int a, int b, int c, char k);
void TouchButton_ctor_text2(void *btn, void *text, int a, int b, int c, int d, char k);
void TouchButton_ctor_img(void *btn, void *img, int a, int b, int c, int d, char k, char m);
void TouchButton_ctor_img2(void *btn, void *imgA, void *imgB, int a, int b, int c, char k);
int TouchButton_getWidth(void *btn);
int TouchButton_getHeight(void *btn);
void TouchButton_getPosition(void *btn, float *x, float *y);
void TouchButton_setVisible(void *btn, bool vis);

void PaintCanvas_Image2DCreate(void *canvas, int id, unsigned int *out);
int PaintCanvas_GetImage2DWidth(void *canvas);
int PaintCanvas_GetImage2DHeight(void *canvas);

void ListItemWindow_ctor(void *win);
void ChoiceWindow_ctor(void *win);
void ChoiceWindow_set(void *win, void *text);

void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);

void RecordHandler_saveOptions(void *rh);

void HangarWindow_refreshCurrentContentHeight(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_globals;          // *piVar27 (Status singleton)
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;           // *puVar30 (PaintCanvas)
__attribute__((visibility("hidden"))) extern void **g_hw_layout;           // *piVar22 (layout config)
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;        // *piVar11
__attribute__((visibility("hidden"))) extern int *g_hw_screenHeight;       // *piVar15
__attribute__((visibility("hidden"))) extern int *g_hw_helpTextId;         // *piVar9
__attribute__((visibility("hidden"))) extern void **g_hw_posXArray;        // DAT for X positions
__attribute__((visibility("hidden"))) extern void **g_hw_posYArray;        // DAT for Y positions
__attribute__((visibility("hidden"))) extern void **g_hw_imageCountSlot;   // mirror of tab count
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_specialModeFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_blackMarketHintFlag;
__attribute__((visibility("hidden"))) extern void **g_hw_recordHandler;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_listModeFlag;
__attribute__((visibility("hidden"))) extern uint8_t *g_hw_introHintFlag;
__attribute__((visibility("hidden"))) extern float g_hw_posScale;
extern "C" extern const char hw_init_buy[], hw_init_sell[], hw_init_lbl[],
    hw_init_more[], hw_init_back[], hw_init_help[];

extern "C" void HangarWindow_initialize(HangarWindow *self)
{
    void *status = *g_hw_globals;

    // Special "shipyard upgrade" mode at station 0x6c with campaign stage 3.
    uint8_t special = 0;
    if (Station_getIndex(Status_getStation()) == 0x6c)
        special = (G<int>(status, 0x114) == 3);
    F<uint8_t>(self, 0x11d) = special;
    Status_calcCargoPrices(status);

    // Build the hangar item list.
    void *list = operator_new(0xc);
    HangarList_ctor(list);
    F<void *>(self, 0x14) = list;
    void *mixed = Item_mixItems(Ship_getCargo(Status_getShip()),
                                Station_getItems(Status_getStation()));
    F<void *>(self, 0x10) = mixed;
    HangarList_init(list, Status_getShip(), 0, Station_getShips(Status_getStation()),
                    Status_getBluePrints(*g_hw_globals));

    // Tab bar: 3 help/info buttons across the top.
    void *tabs = operator_new(0xc);
    Array_TouchButtonPtr_ctor(tabs);
    F<void *>(self, 0x4) = tabs;
    ArraySetLength_TouchButtonPtr(3, tabs);

    int scrW = *g_hw_screenWidth;
    void *layout = *g_hw_layout;

    void *b0 = operator_new(200);
    TouchButton_ctor_text(b0, GameText_getText(*g_hw_helpTextId), 3,
                          scrW - Layout_getHelpButtonOffset(), 0, 0x12);
    G<void *>(G<void *>(F<void *>(self, 0x4), 4), 8) = b0;

    void *b1 = operator_new(200);
    int w0 = TouchButton_getWidth(b0);
    TouchButton_ctor_text(b1, GameText_getText(*g_hw_helpTextId), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0) + G<int>(layout, 0x38), 0, 0x12);
    G<void *>(G<void *>(F<void *>(self, 0x4), 4), 4) = b1;

    void *b2 = operator_new(200);
    int w0b = TouchButton_getWidth(b0);
    int w1b = TouchButton_getWidth(b1);
    TouchButton_ctor_text(b2, GameText_getText(*g_hw_helpTextId), 3,
                          (scrW - Layout_getHelpButtonOffset() - w0b - w1b) + G<int>(layout, 0x38) * 2,
                          0, 0x12);
    G<void *>(G<void *>(F<void *>(self, 0x4), 4), 0) = b2;
    F<uint8_t>(self, 0x11f) = *g_hw_listModeFlag;

    // Six tab icons.
    void *icons = operator_new__(0x18);
    F<void *>(self, 0x30) = icons;
    for (int i = 0; i != 6; i++)
        PaintCanvas_Image2DCreate(*g_hw_canvas, i + 0x232a, (unsigned int *)((char *)icons + i * 4));

    int *posX = (int *)*g_hw_posXArray;
    int *posY = (int *)*g_hw_posYArray;
    Array<void *> *tabArr = (Array<void *> *)F<void *>(self, 0x4);
    for (unsigned int i = 0; i < tabArr->length; i++) {
        if (i < 10) {
            float x = 0, y = 0;
            TouchButton_getPosition(tabArr->data[i], &x, &y);
            posX[i] = (int)x;
            TouchButton_getPosition(tabArr->data[i], &x, &y);
            posY[i] = (int)y;
        }
    }

    *(unsigned int *)*g_hw_imageCountSlot = tabArr->length;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x52e, (unsigned int *)((char *)self + 0xe8));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x544, (unsigned int *)((char *)self + 0xec));

    // Action button bank (24 entries).
    void *btns = operator_new(0xc);
    Array_TouchButtonPtr_ctor(btns);
    F<void *>(self, 0x24) = btns;
    ArraySetLength_TouchButtonPtr(0x18, btns);

    unsigned int img;
    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x470, &img);
    void *e0 = operator_new(200);
    TouchButton_ctor_img((void *)e0, (void *)(uintptr_t)img, 7, 0, 0, G<int>(layout, 0x60), 0x11, 4);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0) = e0;

    if (Status_getCurrentCampaignMission() == 0x4d)
        Station_getIndex(Status_getStation());

    void *e1 = operator_new(200);
    TouchButton_ctor_text(e1, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 4) = e1;
    void *e2 = operator_new(200);
    TouchButton_ctor_text(e2, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 8) = e2;

    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x533, &img);
    void *e3 = operator_new(200);
    TouchButton_ctor_img((void *)e3, (void *)(uintptr_t)img, 7, 0, 0, G<int>(layout, 0x64), 0x11, 4);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0xc) = e3;

    img = 0xffffffff;
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x532, &img);
    void *e4 = operator_new(200);
    TouchButton_ctor_img((void *)e4, (void *)(uintptr_t)img, 7, 0, 0, G<int>(layout, 0x64), 0x11, 4);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x10) = e4;

    void *e5 = operator_new(200);
    TouchButton_ctor_text2(e5, GameText_getText(*g_hw_helpTextId), 7, 0, 0, F<int>(self, 0x110), 0x11);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x14) = e5;
    void *e6 = operator_new(200);
    TouchButton_ctor_text2(e6, GameText_getText(*g_hw_helpTextId), 7, 0, 0, F<int>(self, 0x110), 0x11);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x18) = e6;
    void *e7 = operator_new(200);
    TouchButton_ctor_text(e7, GameText_getText(*g_hw_helpTextId), 7, 0, 0, 0x11);
    G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x1c) = e7;

    {
        String12 lbl;
        void *e8 = operator_new(200);
        AEString_ctor(&lbl, hw_init_buy, false);
        TouchButton_ctor_img((void *)e8, &lbl, 8, 0, 0, G<int>(layout, 0x50), 0x11, 4);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x20) = e8;
        AEString_dtor(&lbl);
    }
    {
        String12 lbl;
        void *e9 = operator_new(200);
        AEString_ctor(&lbl, hw_init_sell, false);
        TouchButton_ctor_img((void *)e9, &lbl, 9, 0, 0, G<int>(layout, 0x50), 0x11, 4);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x24) = e9;
        AEString_dtor(&lbl);
    }
    {
        void *e10 = operator_new(200);
        TouchButton_ctor_img((void *)e10, GameText_getText(*g_hw_helpTextId), 7, 0, 0,
                             G<int>(layout, 0x50), 0x11, 4);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x28) = e10;
    }
    {
        String12 credits;
        void *e11 = operator_new(200);
        Layout_formatCredits(&credits, Status_getCredits());
        TouchButton_ctor_img((void *)e11, &credits, 0xb, *g_hw_screenWidth, *g_hw_screenHeight,
                             Layout_getFooterTransitionWidth(), 0x22, 4);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x2c) = e11;
        AEString_dtor(&credits);
    }

    // Five collapsible "more" rows (indices 0x0c..0x10 in slot terms).
    uint8_t listMode = F<uint8_t>(self, 0x11f);
    int row = 0;
    for (int slot = 0xc; (unsigned int)(slot - 0xc) < 5; slot++) {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_more, false);
        int visIdx;
        if (listMode == 0) {
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, G<int>(layout, 0x264), 0x11, 1);
            visIdx = row + 0xc;
        } else {
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
            visIdx = slot;
        }
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), slot * 4) = btn;
        AEString_dtor(&lbl);
        TouchButton_setVisible(G<void *>(G<void *>(F<void *>(self, 0x24), 4), visIdx * 4), false);
        row++;
    }
    {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_back, false);
        if (listMode == 0)
            TouchButton_ctor_img(btn, &lbl, 0, 0, 0, G<int>(layout, 0x264), 0x11, 1);
        else
            TouchButton_ctor_text(btn, &lbl, 10, 0, 0, 1);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x44) = btn;
        AEString_dtor(&lbl);
        TouchButton_setVisible(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x44), false);
    }

    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233e, (unsigned int *)((char *)self + 0x34));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233f, (unsigned int *)((char *)self + 0x38));
    {
        String12 lbl;
        void *btn = operator_new(200);
        AEString_ctor(&lbl, hw_init_help, false);
        TouchButton_ctor_text(btn, &lbl, 7, 0, 0, 0x11);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x5c) = btn;
        AEString_dtor(&lbl);
    }

    // Tab selector icons (5 entries with paired up/down images).
    unsigned int imgA, imgB;
    for (int i = 0x12; (unsigned int)(i - 0x12) < 5; i++) {
        imgB = 0xffffffff;
        if (i == 0x12) {
            PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233c, &imgA);
            PaintCanvas_Image2DCreate(*g_hw_canvas, 0x233d, &imgB);
        } else {
            short s = (short)(i - 0x12);
            PaintCanvas_Image2DCreate(*g_hw_canvas, s * 2 + 0x2330, &imgA);
            PaintCanvas_Image2DCreate(*g_hw_canvas, s * 2 + 0x2331, &imgB);
        }
        void *btn = operator_new(200);
        TouchButton_ctor_img2(btn, (void *)(uintptr_t)imgA, (void *)(uintptr_t)imgB, 0x13, 0, 0, 1);
        G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4) = btn;
        TouchButton_setVisible(G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4), false);
    }

    F<int>(self, 0x120) = TouchButton_getWidth(0);
    int h = TouchButton_getHeight(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x30));
    F<int>(self, 0x124) = h;
    F<int>(self, 0x128) = (int)((float)(-F<int>(self, 0x120)) * g_hw_posScale);
    F<int>(self, 0x12c) = (int)((float)(-h) * g_hw_posScale);

    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x475, (unsigned int *)((char *)self + 0x78));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x476, (unsigned int *)((char *)self + 0x7c));
    PaintCanvas_Image2DCreate(*g_hw_canvas, 0x477, (unsigned int *)((char *)self + 0x74));
    F<int>(self, 0xdc) = PaintCanvas_GetImage2DWidth(*g_hw_canvas);
    F<int>(self, 0xe0) = PaintCanvas_GetImage2DHeight(*g_hw_canvas);

    // Recompute best cargo prices from known equipment when not in a black market.
    if (F<void *>(self, 0x10) != 0 && Status_inBlackMarketSystem() == 0 &&
        F<uint8_t>(self, 0x11d) == 0) {
        unsigned int *equip = (unsigned int *)Ship_getEquipment(Status_getShip());
        Array<void *> *cargo = (Array<void *> *)F<void *>(self, 0x10);
        unsigned int n = cargo->length + (equip ? equip[0] : 0);
        for (unsigned int i = 0; i < n; i++) {
            void *itemPtr;
            if (i < cargo->length)
                itemPtr = cargo->data[i];
            else
                itemPtr = ((void **)equip[1])[i - cargo->length];
            if (itemPtr != 0) {
                int price = Item_getSinglePrice(itemPtr);
                int idx = Item_getIndex(itemPtr);
                void *globals = *g_hw_globals;
                int *buyTbl = (int *)G<int>(G<void *>(globals, 0x40), 4);
                if (buyTbl[idx] < price || buyTbl[idx] == 0) {
                    buyTbl[idx] = price;
                    int sysIdx = SolarSystem_getIndex(Status_getSystem());
                    *(int *)(G<int>(G<void *>(globals, 0x48), 4) + idx * 4) = sysIdx;
                }
                int *sellTbl = (int *)G<int>(G<void *>(globals, 0x3c), 4);
                if (price < sellTbl[idx] || sellTbl[idx] == 0) {
                    sellTbl[idx] = price;
                    int sysIdx = SolarSystem_getIndex(Status_getSystem());
                    *(int *)(G<int>(G<void *>(*g_hw_globals, 0x44), 4) + idx * 4) = sysIdx;
                }
            }
        }
    }

    Ship_adjustPrice(Status_getShip());

    void *liw = operator_new(0x13c);
    ListItemWindow_ctor(liw);
    F<void *>(self, 0x18) = liw;
    void *cw1 = operator_new(0x5c);
    ChoiceWindow_ctor(cw1);
    F<int>(self, 0x58) = 0;
    F<void *>(self, 0x1c) = cw1;
    void *cw2 = operator_new(0x5c);
    ChoiceWindow_ctor(cw2);
    F<uint8_t>(self, 0x3c) = 0;
    F<void *>(self, 0x20) = cw2;

    F<uint8_t>(self, 0x11c) = 0;
    F<int>(self, 0x6c) = 0;
    F<int>(self, 0x70) = 0;
    F<void *>(self, 0x28) = 0;
    F<void *>(self, 0x2c) = 0;
    F<uint8_t>(self, 0x130) = 0;
    F<uint16_t>(self, 0xd1) = 0;
    F<uint8_t>(self, 0xb1) = 0;
    F<int>(self, 0xad) = 0;

    int scrH = *g_hw_screenHeight;
    int contentW = *g_hw_screenWidth - 10;
    F<int>(self, 0x40) = 0x10;
    F<int>(self, 0x44) = 5;
    F<int>(self, 0x48) = 5;
    F<int>(self, 0x4c) = contentW;
    F<int>(self, 0x50) = scrH - 10;

    int *cols = (int *)operator_new__(0xc);
    F<void *>(self, 0x54) = cols;
    int third = __aeabi_idiv(contentW, 3) - 2;
    cols[0] = third;
    cols[1] = third;
    cols[2] = (*g_hw_screenWidth - 0xe) + third * -2;

    HangarList_setCurrentTab(F<void *>(self, 0x14), *g_hw_specialModeFlag != 0);
    HangarWindow_refreshCurrentContentHeight(self);

    F<int>(self, 0xa8) = Ship_getCurrentLoad(Status_getShip());
    void *lay2 = *g_hw_layout;
    F<int>(self, 0xd8) = ((*g_hw_screenHeight - G<int>(lay2, 0x10) - G<int>(lay2, 0xc)) -
                          G<int>(lay2, 0x20)) - G<int>(lay2, 0x24);

    int extra = 0;
    if (*g_hw_blackMarketHintFlag != 0 && *g_hw_introHintFlag == 0) {
        PaintCanvas_Image2DCreate(*g_hw_canvas, 0x6a4, (unsigned int *)((char *)self + 0xf0));
        extra = (int)((float)(*g_hw_screenWidth) * g_hw_posScale);
    }
    F<void *>(self, 0x68) = 0;
    F<int>(self, 0xf4) = extra;
    F<uint16_t>(self, 0x88) = 0;
    F<uint8_t>(self, 0xac) = 0;
    F<uint16_t>(self, 0x90) = 0;
    F<uint8_t>(self, 0x92) = 0;

    // Zero the 16-byte block at 0xc1 and the touch-drag block at 0xb4.
    *(int *)((char *)self + 0xc1) = 0;
    *(int *)((char *)self + 0xc5) = 0;
    *(int *)((char *)self + 0xc9) = 0;
    *(int *)((char *)self + 0xcd) = 0;
    F<int>(self, 0xe4) = 0;
    F<uint8_t>(self, 0x0) = 0;
    F<uint8_t>(self, 0xc) = 1;
    F<uint8_t>(self, 0xf8) = 0;
    F<int>(self, 0xfc) = 0xffffffff;
    F<int>(self, 0xb4) = 0;
    F<int>(self, 0xb8) = 0;
    F<int>(self, 0xbc) = 0;
    F<int>(self, 0xc0) = 0;

    // First-visit hint at later campaign stages.
    if (Status_getCurrentCampaignMission() > 0xd) {
        uint8_t *introFlag = g_hw_introHintFlag;
        if (introFlag[0x4e] == 0) {
            GameText_getText(*g_hw_helpTextId);
            ChoiceWindow_set(F<void *>(self, 0x20), 0);
            introFlag[0x4e] = 1;
            RecordHandler_saveOptions(*g_hw_recordHandler);
            F<uint8_t>(self, 0x3c) = 1;
        }
    }
}
