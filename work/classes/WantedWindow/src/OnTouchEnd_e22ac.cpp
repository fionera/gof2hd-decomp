#include "class.h"

extern "C" void StarMap_OnTouchEnd_tail(void);
extern "C" int StarMap_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);
extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" int WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y);
extern "C" void WantedWindow_selectWanted(WantedWindow *self, int idx);
extern "C" int Wanted_isActive(void *wanted);
extern "C" int Wanted_getLastSeen(void *wanted);
extern "C" int Wanted_getTravelsTo(void *wanted);
extern "C" void *ApplicationManager_GetApplicationModule(void *app, int id);
extern "C" void *Galaxy_getStation(void *galaxy, int id);
extern "C" void *Mission_ctor(void *mission, int a, int b, int dest);
extern "C" void *StarMap_ctor(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" void StarMap_init(void *map, bool jumpMapMode, void *mission, bool flag, int idx);
extern "C" int Station_getSystem(void *station);
extern "C" void StarMap_setStart(void *map, int system, int lastSeen);
extern "C" void *Station_dtor(void *station);
extern "C" int Layout_OnTouchEnd(void *layout, int x, int y);
extern "C" void Layout_resetWindowDimensions(void *layout);
extern "C" int Layout_helpPressed(void *layout);
extern "C" String *GameText_getText(void *text, int id);
extern "C" void String_copy_ctor(String *dst, String *src, bool copy);
extern "C" void Layout_initHelpWindow(void *layout, String *text);
extern "C" void String_dtor(String *s);

__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_fullscreen;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_tablet;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_end_small;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_screen_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_window_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_end_window_w;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_app;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_galaxy;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_layout_a;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_layout_b;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_end_text;

extern "C" void WantedWindow_OnTouchEnd(WantedWindow *self, int x, int y)
{
    String help;

    if (F<uint8_t>(self, 0x14) != 0) {
        if (StarMap_OnTouchEnd(F<void *>(self, 0x4), x, y) != 0) {
            uint32_t h;
            uint32_t w;
            uint32_t halfW = 0;
            if (*g_WantedWindow_end_fullscreen == 0) {
                h = *g_WantedWindow_end_window_h;
                w = *g_WantedWindow_end_window_w;
                F<int>(self, 0x1c) = 0;
                F<int>(self, 0x20) = 0;
            } else {
                if (*g_WantedWindow_end_tablet == 0) {
                    h = 1000;
                    w = 0x514;
                    if (*g_WantedWindow_end_small == 0) {
                        h = 500;
                        w = 0x28a;
                    }
                    halfW = w >> 1;
                } else {
                    h = 0x2bf;
                    halfW = 0x1c9;
                    w = 0x392;
                }
                F<uint32_t>(self, 0x1c) = (*g_WantedWindow_end_screen_w >> 1) - halfW;
                F<uint32_t>(self, 0x20) = (*g_WantedWindow_end_screen_h >> 1) - (h >> 1);
            }
            F<uint32_t>(self, 0x24) = w;
            F<uint32_t>(self, 0x28) = h;
            F<uint8_t>(self, 0x14) = 0;
        }
    } else {
        int delta = F<int>(self, 0x90);
        int pos = F<int>(self, 0x84) + delta;
        float velocity = 0.0f;
        int mag = delta;
        if (mag < 0) {
            mag = -mag;
        }
        if (mag > 3) {
            velocity = (float)delta;
        }
        F<float>(self, 0x94) = 0.95f;
        F<uint8_t>(self, 0xa0) = 0;
        F<int>(self, 0x84) = pos;
        F<int>(self, 0x8c) = pos;
        F<float>(self, 0x98) = velocity;

        ScrollTouchWindow_OnTouchEnd(F<void *>(self, 0x2c), x, y);
        for (uint32_t i = 0; i < F<uint32_t>(F<void *>(self, 0x0c), 0x0); ++i) {
            if (TouchButton_OnTouchEnd(ArrayItem(F<void *>(self, 0x0c), i), x, y) != 0) {
                F<uint32_t>(self, 0x0) = i;
            }
        }

        if (F<int>(self, 0x34) >= 0) {
            int idx = WantedWindow_getWantedAtPosition(self, x, y);
            F<int>(self, 0x30) = idx;
            F<int>(self, 0x34) = idx;
            WantedWindow_selectWanted(self, idx);
        }

        bool openMap = false;
        if (F<void *>(self, 0x18) != 0) {
            void *wanted = ArrayItem(F<void *>(self, 0x38), F<int>(self, 0x30));
            if (Wanted_isActive(wanted) != 0 &&
                TouchButton_OnTouchEnd(F<void *>(self, 0x18), x, y) != 0) {
                openMap = true;
            }
        }

        if (openMap) {
            void **appHolder = g_WantedWindow_end_app;
            void *module = ApplicationManager_GetApplicationModule(*appHolder, 5);
            F<void *>(self, 0x4) = F<void *>(module, 0x10);
            void *wanted = ArrayItem(F<void *>(self, 0x38), F<int>(self, 0x30));
            int lastSeen = Wanted_getLastSeen(wanted);
            void *station = Galaxy_getStation(*g_WantedWindow_end_galaxy, lastSeen);
            void *oldMission = F<void *>(self, 0xb0);
            if (oldMission != 0) {
                (*(void (**)(void *))(*(int *)oldMission + 4))(oldMission);
            }
            F<void *>(self, 0xb0) = 0;

            void *mission = operator_new(0x78);
            Mission_ctor(mission, 0, 0, Wanted_getTravelsTo(wanted));
            F<void *>(self, 0xb0) = mission;

            void *map = F<void *>(self, 0x4);
            if (map == 0) {
                map = operator_new(0x1e8);
                StarMap_ctor(map, true, mission, false, -1);
                ApplicationManager_GetApplicationModule(*appHolder, 5);
                F<void *>(ApplicationManager_GetApplicationModule(*appHolder, 5), 0x10) = map;
                map = F<void *>(ApplicationManager_GetApplicationModule(*appHolder, 5), 0x10);
                F<void *>(self, 0x4) = map;
            } else {
                StarMap_init(map, true, mission, false, -1);
            }

            int system = Station_getSystem(station);
            lastSeen = Wanted_getLastSeen(wanted);
            StarMap_setStart(map, system, lastSeen);
            if (station != 0) {
                operator_delete(Station_dtor(station));
            }
            F<uint8_t>(self, 0x14) = 1;
            Layout_resetWindowDimensions(*g_WantedWindow_end_layout_a);
        } else {
            void **layoutHolder = g_WantedWindow_end_layout_b;
            void *layout = *layoutHolder;
            if (Layout_OnTouchEnd(layout, x, y) != 0) {
                Layout_resetWindowDimensions(layout);
            } else if (Layout_helpPressed(layout) != 0) {
                String_copy_ctor(&help, GameText_getText(*g_WantedWindow_end_text, 0x27b), false);
                Layout_initHelpWindow(layout, &help);
                String_dtor(&help);
            }
        }
    }

}
