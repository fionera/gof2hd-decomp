#include "class.h"

struct Arr { uint32_t size; void **data; uint32_t size2; };

extern "C" void Layout_OnTouchBegin(void *layout, int x);
extern "C" void TouchButton_OnTouchBegin(void *btn, int x, int y);
extern "C" void *Achievements_getMedals(void *ach);
extern "C" int Achievements_isEliteMedal(void *ach, int idx);
// *g_layout -> Layout pointer (double load). *g_btnFlag -> byte flag. g_ach -> Achievements**.
extern "C" __attribute__((visibility("hidden"))) void **g_StatusWindow_layout;
extern "C" __attribute__((visibility("hidden"))) unsigned char *g_StatusWindow_btnFlag;
extern "C" __attribute__((visibility("hidden"))) void **g_StatusWindow_ach;

// StatusWindow::OnTouchBegin(int, int)
extern "C" int StatusWindow_OnTouchBegin(StatusWindow *self, int param_1, int param_2)
{
    void **lh = g_StatusWindow_layout;
    i32(self, 0x50) = param_2;
    i32(self, 0x3c) = param_2;
    i32(self, 0x44) = 0;
    F<uint8_t>(self, 0x54) = 1;
    Layout_OnTouchBegin(*lh, param_1);
    if (*g_StatusWindow_btnFlag == 0) {
        for (unsigned i = 0; i < F<Arr *>(self, 0x4)->size; ++i)
            TouchButton_OnTouchBegin(F<Arr *>(self, 0x4)->data[i], param_1, param_2);
    }
    if (i32(self, 0x30) == 1) {
        void **holder = g_StatusWindow_ach;
        int *medals = (int *)Achievements_getMedals(*holder);
        for (int i = 0; i < i32(self, 0x0); ++i) {
            if (medals[i] != 0 || Achievements_isEliteMedal(*holder, i) != 0)
                TouchButton_OnTouchBegin(F<Arr *>(self, 0x8)->data[i], param_1, param_2);
        }
    }
    return 0;
}
