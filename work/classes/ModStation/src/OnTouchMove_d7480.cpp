#include "class.h"

struct HangarWindow;
struct StarMap;
struct SpaceLounge;
struct StatusWindow;
struct MissionsWindow;
struct MenuTouchWindow;
struct ChoiceWindow;
struct DialogueWindow;
struct NewsTicker;

__attribute__((visibility("hidden"))) extern void **g_ModStation_tm_layout;  // g -> P, *P -> layout obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_tm_screenH; // **g -> screen height

extern "C" void HangarWindow_OnTouchMove(void *w, int x, int y);
extern "C" void StarMap_OnTouchMove(void *w, int x, int y);
extern "C" void SpaceLounge_OnTouchMove(void *w, int x, int y);
extern "C" void StatusWindow_OnTouchMove(void *w, int x, int y);
extern "C" void MissionsWindow_OnTouchMove(void *w, int x, int y);
extern "C" void MenuTouchWindow_OnTouchMove(void *w, int x, int y);
extern "C" void ChoiceWindow_OnTouchMove(void *w, int x, int y);
extern "C" void DialogueWindow_OnTouchMove(void *w, int x, int y);
extern "C" void Layout_OnTouchMove(void *w, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *w, int x, int y);
extern "C" int NewsTicker_OnTouchMove(void *w, int x, int y);

// ModStation::OnTouchMove(int, int, void*)
extern "C" void ModStation_OnTouchMove(ModStation *self, int x, int y, void *touch)
{
    if (P(self, 0x128) != touch)
        return;
    I(self, 0x110) = x;
    I(self, 0x114) = y;
    if (C(self, 0x60) != 0)
        return;

    void **layoutHolder = g_ModStation_tm_layout;
    void *layoutObj = *layoutHolder;
    if (*(char *)layoutObj != 0) {
        Layout_OnTouchMove(layoutObj, x, y);
        return;
    }
    if (C(self, 0x69) != 0) {
        DialogueWindow_OnTouchMove(P(self, 0x84), x, y);
        return;
    }
    if (C(self, 0x63) != 0) {
        ChoiceWindow_OnTouchMove(P(self, 0x70), x, y);
        return;
    }
    if (C(self, 0x6a) != 0) {
        ChoiceWindow_OnTouchMove(P(self, 0x88), x, y);
        return;
    }
    if (C(self, 0x66) != 0) {
        HangarWindow_OnTouchMove(P(self, 0x78), x, y);
        return;
    }
    if (C(self, 0x67) != 0) {
        StarMap_OnTouchMove(P(self, 0x10), x, y);
        return;
    }
    if (C(self, 0x65) != 0) {
        SpaceLounge_OnTouchMove(P(self, 0x74), x, y);
        return;
    }
    if (C(self, 0x68) != 0) {
        StatusWindow_OnTouchMove(P(self, 0x7c), x, y);
        return;
    }
    if (C(self, 0x64) != 0) {
        MissionsWindow_OnTouchMove(P(self, 0x80), x, y);
        return;
    }
    if (C(self, 0x62) != 0) {
        MenuTouchWindow_OnTouchMove(P(self, 0x50), x, y);
        return;
    }
    if (C(self, 0x61) == 0)
        return;
    TouchButton_OnTouchMove(P(self, 0x90), x, y);
    TouchButton_OnTouchMove(P(self, 0x94), x, y);
    for (int i = 0; i != 5; i++)
        TouchButton_OnTouchMove(*(void **)(I(P(self, 0x8c), 4) + i * 4), x, y);
    Layout_OnTouchMove(layoutObj, x, y);
    if (NewsTicker_OnTouchMove(P(self, 0x1c), x, y) != 0)
        return;
    int *layout = (int *)layoutObj;
    if (y <= layout[0xc / 4])
        return;
    if (*(int *)*g_ModStation_tm_screenH - layout[0x10 / 4] <= y)
        return;
    if (x <= I(self, 0xa8))
        return;
    int delta = x - I(self, 0xe4);
    I(self, 0xf0) = delta;
    I(self, 0xf4) = 0x3f800000;
    I(self, 0xe8) = I(self, 0xe8) + delta;
    I(self, 0xe4) = x;
}
