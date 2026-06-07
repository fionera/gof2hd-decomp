#include "class.h"

struct SpaceLounge;
struct StarMap;
struct HangarWindow;
struct StatusWindow;
struct MissionsWindow;
struct MenuTouchWindow;
struct ChoiceWindow;
struct DialogueWindow;
struct NewsTicker;

// Globals.
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_layout;  // g -> P, *P -> layout obj
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_screenH; // **g -> screen height
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_campaign;// *g -> obj for nextCampaignMission
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_appmod;  // *g -> app module value
__attribute__((visibility("hidden"))) extern void **g_ModStation_tb_clear;   // *g -> slot zeroed

extern "C" int Radio_lastMessageShown(void *radio);
extern "C" void Status_nextCampaignMission(int flag);
extern "C" void ApplicationManager_SetCurrentApplicationModule(unsigned int mod);

extern "C" void HangarWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void SpaceLounge_OnTouchBegin(void *w, int x, int y);
extern "C" void StarMap_OnTouchBegin(void *w, int x, int y);
extern "C" void StatusWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void MissionsWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void MenuTouchWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void ChoiceWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void DialogueWindow_OnTouchBegin(void *w, int x, int y);
extern "C" void Layout_OnTouchBegin(void *w, int x, int y);
extern "C" void TouchButton_OnTouchBegin(void *w, int x, int y);
extern "C" int NewsTicker_OnTouchBegin(void *w, int x, int y);

// ModStation::OnTouchBegin(int, int, void*)
extern "C" void ModStation_OnTouchBegin(ModStation *self, int x, int y, void *touch)
{
    if (P(self, 0x128) != 0)
        return;
    UC(self, 0x118) = 1;
    P(self, 0x128) = touch;
    int flag = I(self, 0x60);
    I(self, 0x110) = x;
    I(self, 0x114) = y;
    if (flag != 0) {
        if (Radio_lastMessageShown(P(self, 0x54)) != 0) {
            UC(self, 0x24) = 0;
            Status_nextCampaignMission((char)*(int *)*g_ModStation_tb_campaign);
            unsigned int mod = *(unsigned int *)*g_ModStation_tb_appmod;
            *(int *)*g_ModStation_tb_clear = 0;
            ApplicationManager_SetCurrentApplicationModule(mod);
            UC(self, 0x60) = 0;
        }
        return;
    }
    void **layoutHolder = g_ModStation_tb_layout;
    void *layoutObj = *layoutHolder;
    if (*(char *)layoutObj != 0) {
        Layout_OnTouchBegin(layoutObj, x, y);
        return;
    }
    if (C(self, 0x69) != 0) {
        DialogueWindow_OnTouchBegin(P(self, 0x84), x, y);
        return;
    }
    if (C(self, 0x63) != 0) {
        ChoiceWindow_OnTouchBegin(P(self, 0x70), x, y);
        return;
    }
    if (C(self, 0x6a) != 0) {
        ChoiceWindow_OnTouchBegin(P(self, 0x88), x, y);
        return;
    }
    if (C(self, 0x66) != 0) {
        HangarWindow_OnTouchBegin(P(self, 0x78), x, y);
        return;
    }
    if (C(self, 0x65) != 0) {
        SpaceLounge_OnTouchBegin(P(self, 0x74), x, y);
        return;
    }
    if (C(self, 0x67) != 0) {
        StarMap_OnTouchBegin(P(self, 0x10), x, y);
        return;
    }
    if (C(self, 0x68) != 0) {
        StatusWindow_OnTouchBegin(P(self, 0x7c), x, y);
        return;
    }
    if (C(self, 0x64) != 0) {
        MissionsWindow_OnTouchBegin(P(self, 0x80), x, y);
        return;
    }
    if (C(self, 0x62) != 0) {
        MenuTouchWindow_OnTouchBegin(P(self, 0x50), x, y);
        return;
    }
    if (C(self, 0x61) == 0)
        return;
    TouchButton_OnTouchBegin(P(self, 0x90), x, y);
    TouchButton_OnTouchBegin(P(self, 0x94), x, y);
    for (int i = 0; i != 5; i++)
        TouchButton_OnTouchBegin(*(void **)(I(P(self, 0x8c), 4) + i * 4), x, y);
    Layout_OnTouchBegin(layoutObj, x, y);
    if (NewsTicker_OnTouchBegin(P(self, 0x1c), x, y) != 0)
        return;
    int *layout = (int *)layoutObj;
    if (y <= layout[0xc / 4])
        return;
    if (*(int *)*g_ModStation_tb_screenH - layout[0x10 / 4] <= y)
        return;
    if (x <= I(self, 0xa8))
        return;
    I(self, 0xfc) = x;
    I(self, 0xe8) = x;
    I(self, 0xf0) = 0;
    UC(self, 0x100) = 1;
}
