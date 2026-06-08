#include "class.h"

struct String12 { uint32_t a, b, c; };

extern "C" {
void *ApplicationManager_GetApplicationData();
void AEString_ctor(String12 *self, const char *text, bool copy);
void AEString_dtor(String12 *self);
void *GameText_getText(int id);
void ChoiceWindow_set(void *win, String12 *title, String12 *t2, bool flag,
                      String12 *yes, String12 *no, void *body, int a, int b);
void ChoiceWindow_setWidth(void *win, int w);
void ChoiceWindow_setHeight(void *win, int h);
float VectorSignedToFloat(int v, int mode);
}

__attribute__((visibility("hidden"))) extern int *g_hw_buyTextId;
__attribute__((visibility("hidden"))) extern int *g_hw_buyTextId2;
__attribute__((visibility("hidden"))) extern char *g_hw_buyFlag;
__attribute__((visibility("hidden"))) extern int *g_hw_buyWidth;
__attribute__((visibility("hidden"))) extern int *g_hw_buyHeight;
extern "C" {
extern const char hw_buy_yes[], hw_buy_no[], hw_buy_icon[];
extern const char hw_buy2_yes[], hw_buy2_no[], hw_buy2_icon[];
extern float hw_buy_heightScale;
}

extern "C" void HangarWindow_showCreditsBuyWindow(HangarWindow *self)
{
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = F<void *>(self, 0x20);
    String12 a, b, yes, no;

    if (F<uint8_t>(self, 0x11f) == 0) {
        AEString_ctor(&a, hw_buy_yes, false);
        AEString_ctor(&b, hw_buy_no, false);
        AEString_ctor(&yes, hw_buy_icon, false);
        AEString_ctor(&no, hw_buy2_yes, false);
        void *body = GameText_getText(*g_hw_buyTextId);
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);
        AEString_dtor(&no);
        AEString_dtor(&yes);
        AEString_dtor(&b);
        AEString_dtor(&a);
    } else {
        AEString_ctor(&a, hw_buy2_no, false);
        AEString_ctor(&b, hw_buy2_no, false);
        AEString_ctor(&yes, hw_buy2_icon, false);
        AEString_ctor(&no, hw_buy_yes, false);
        void *body = GameText_getText(*g_hw_buyTextId2);
        ChoiceWindow_set(win, &a, &b, false, &yes, &no, body, -1, -1);
        AEString_dtor(&no);
        AEString_dtor(&yes);
        AEString_dtor(&b);
        AEString_dtor(&a);

        int h;
        void *win2;
        if (*g_hw_buyFlag == 0) {
            ChoiceWindow_setWidth(F<void *>(self, 0x20), *g_hw_buyWidth);
            h = *g_hw_buyHeight;
            win2 = F<void *>(self, 0x20);
        } else {
            ChoiceWindow_setWidth(F<void *>(self, 0x20), F<int>(self, 0x120) * 3);
            float v = VectorSignedToFloat(F<int>(self, 0x124), 0);
            win2 = F<void *>(self, 0x20);
            h = (int)(v * hw_buy_heightScale);
        }
        ChoiceWindow_setHeight(win2, h);
    }

    F<uint8_t>(self, 0x3c) = 1;
    F<uint8_t>(self, 0xae) = 1;
    F<uint8_t>(self, 0xaf) = 0;
}
