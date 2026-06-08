#include "class.h"
typedef unsigned int uintptr_t;

// AbyssEngine::String - 12-byte value type.
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
int TouchButton_getHeight(void *btn);
int TouchButton_getWidth(void *btn);
int PaintCanvas_GetTextWidth(unsigned int canvas, void *str);
}

__attribute__((visibility("hidden"))) extern int *g_hw_freeCreditsTextId;
__attribute__((visibility("hidden"))) extern void **g_hw_canvas;
__attribute__((visibility("hidden"))) extern void **g_hw_layout;
extern "C" extern const char hw_freeCredits_yes[];
extern "C" extern const char hw_freeCredits_no[];
extern "C" extern const char hw_freeCredits_icon[];

extern "C" void HangarWindow_showFreeCreditsWindow(HangarWindow *self)
{
    void *appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x4c) = 0;
    appData = ApplicationManager_GetApplicationData();
    *((uint8_t *)appData + 0x3d) = 1;

    void *win = self->f_20;
    String12 title, title2, yes, no;
    AEString_ctor(&title, hw_freeCredits_yes, false);
    AEString_ctor(&title2, hw_freeCredits_yes, false);
    AEString_ctor(&yes, hw_freeCredits_no, false);
    AEString_ctor(&no, hw_freeCredits_icon, false);
    void *body = GameText_getText(*g_hw_freeCreditsTextId);
    ChoiceWindow_set(win, &title, &title2, false, &yes, &no, body, -1, -1);
    AEString_dtor(&no);
    AEString_dtor(&yes);
    AEString_dtor(&title2);
    AEString_dtor(&title);

    int rowH = TouchButton_getHeight(G<void *>(G<void *>(self->f_24, 4), 0x48));
    ChoiceWindow_setHeight(self->f_20, rowH * 5);

    int maxW = 0;
    for (int i = 5; i != 0; i--) {
        GameText_getText(*g_hw_freeCreditsTextId);
        int w = PaintCanvas_GetTextWidth((unsigned int)(uintptr_t)*g_hw_canvas, 0);
        if (maxW < w)
            maxW = w;
    }
    int btnW = TouchButton_getWidth(0);
    void *layout = *g_hw_layout;
    ChoiceWindow_setWidth(self->f_20,
                          G<int>(layout, 0x2c) + btnW + maxW + G<int>(layout, 0x28) * 4);

    self->f_b0 = 1;
    self->f_3c = 1;
    F<uint8_t>(self, 0xae) = 0;
    F<uint8_t>(self, 0xaf) = 0;
}
