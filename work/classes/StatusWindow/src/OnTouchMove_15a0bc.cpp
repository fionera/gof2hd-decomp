#include "class.h"

struct Arr { uint32_t size; void **data; uint32_t size2; };

extern "C" void Layout_OnTouchMove(void *layout, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *btn, int x, int y);
extern "C" void TouchButton_setAlwaysPressed(void *btn, int v);
extern "C" void *Achievements_getMedals(void *ach);
extern "C" int Achievements_isEliteMedal(void *ach, int idx);
extern "C" __attribute__((visibility("hidden"))) void **g_SWm_layout;
extern "C" __attribute__((visibility("hidden"))) int *g_SWm_height;
extern "C" __attribute__((visibility("hidden"))) int *g_SWm_force;
extern "C" __attribute__((visibility("hidden"))) unsigned char *g_SWm_btnFlag;
extern "C" __attribute__((visibility("hidden"))) void **g_SWm_ach;

// StatusWindow::OnTouchMove(int, int)
extern "C" int StatusWindow_OnTouchMove(StatusWindow *self, int param_1, int param_2)
{
    void **lh = g_SWm_layout;
    void *layout = *lh;
    if ((i32(layout, 0xc) < param_2 && param_2 < *g_SWm_height - i32(layout, 0x10)) || *g_SWm_force != 0) {
        int d = param_2 - i32(self, 0x3c);
        i32(self, 0x44) = d;
        u32(self, 0x48) = 0x3f800000u;
        i32(self, 0x38) += d;
        i32(self, 0x3c) = param_2;
        if (i32(self, 0x34) >= 0) {
            int e = i32(self, 0x50) - param_2;
            if (e < 0) e = -e;
            if (e > 3) {
                TouchButton_setAlwaysPressed(F<Arr *>(self, 0x8)->data[i32(self, 0x34)], 0);
                i32(self, 0x34) = -1;
                layout = *lh;
            }
        }
    }
    Layout_OnTouchMove(layout, param_1, param_2);
    if (*g_SWm_btnFlag == 0) {
        for (unsigned i = 0; i < F<Arr *>(self, 0x4)->size; ++i)
            TouchButton_OnTouchMove(F<Arr *>(self, 0x4)->data[i], param_1, param_2);
    }
    if (i32(self, 0x30) == 1) {
        void **holder = g_SWm_ach;
        int *medals = (int *)Achievements_getMedals(*holder);
        for (int i = 0; i < i32(self, 0x0); ++i) {
            if (medals[i] != 0 || Achievements_isEliteMedal(*holder, i) != 0)
                TouchButton_OnTouchMove(F<Arr *>(self, 0x8)->data[i], param_1, param_2);
        }
    }
    return 0;
}
