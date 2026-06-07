#include "class.h"

extern "C" void Array_Wanted_ctor(void *arr);
extern "C" void ArrayAdd_Wanted(void *wanted, void *arr);
extern "C" void *Status_getWanted(void *status);
extern "C" void *Status_getSystem(void *status);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" int SolarSystem_getRace(void *system);
extern "C" int Wanted_getBoard(void *wanted);
extern "C" int Wanted_isActive(void *wanted);
extern "C" void WantedWindow_selectWanted(WantedWindow *self, int idx);
extern "C" void Array_TouchButton_ctor(void *arr);
extern "C" void ArraySetLength_TouchButton(int length, void *arr);
extern "C" String *GameText_getText(void *text, int id);
extern "C" int Layout_getHelpButtonOffset(void *layout);
extern "C" void TouchButton_ctor6(void *button, String *text, int type, int x, int y, int icon);
extern "C" int TouchButton_getWidth(void *button);
extern "C" void TouchButton_setAlwaysPressed(void *button, bool pressed);
extern "C" void Layout_setWindowDimensions(void *layout, int x, int y, int w, int h);
extern "C" void *TouchButton_dtor(void *button);
extern "C" void TouchButton_ctor8(void *button, String *text, int type, int x, int y, int width, int icon, int mode);

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_status;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_layout;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_fullscreen;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_tablet;
__attribute__((visibility("hidden"))) extern uint8_t *g_WantedWindow_init_small;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_screen_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_window_w;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_init_window_h;
__attribute__((visibility("hidden"))) extern void **g_WantedWindow_init_text;

extern "C" int WantedWindow_init(WantedWindow *self)
{
    typedef int v4si __attribute__((vector_size(16)));
    v4si zero = {0, 0, 0, 0};
    *(v4si *)((char *)self + 0x91) = zero;
    *(v4si *)((char *)self + 0x84) = zero;

    void *wantedList = operator_new(0x0c);
    Array_Wanted_ctor(wantedList);
    F<void *>(self, 0x38) = wantedList;

    void *status = *g_WantedWindow_init_status;
    void *allWanted = Status_getWanted(status);
    void **layoutHolder = g_WantedWindow_init_layout;
    void *layout = *layoutHolder;
    int wBase = F<int>(layout, 0x28);
    int wLimit = F<int>(layout, 0xcc);
    int wExtra = F<int>(layout, 0x4c);
    int activeWidth = wLimit - wBase - wExtra;

    for (uint32_t i = 0; i < F<uint32_t>(allWanted, 0x0); ++i) {
        int race = SolarSystem_getRace(Status_getSystem(status));
        void *wanted = ArrayItem(allWanted, i);
        if (race == Wanted_getBoard(wanted)) {
            race = SolarSystem_getRace(Status_getSystem(status));
            if (race != 0 || Status_getCurrentCampaignMission(status) < 0x80) {
                race = SolarSystem_getRace(Status_getSystem(status));
                if (race == 0 && Status_getCurrentCampaignMission(status) >= 0xa2) {
                    ArrayAdd_Wanted(wanted, F<void *>(self, 0x38));
                }
            } else {
                ArrayAdd_Wanted(wanted, F<void *>(self, 0x38));
            }
        }
    }

    uint32_t h;
    uint32_t w;
    uint32_t halfW = 0;
    if (*g_WantedWindow_init_fullscreen == 0) {
        h = *g_WantedWindow_init_window_h;
        w = *g_WantedWindow_init_window_w;
        F<int>(self, 0x1c) = 0;
        F<int>(self, 0x20) = 0;
    } else {
        if (*g_WantedWindow_init_tablet == 0) {
            h = 1000;
            w = 0x514;
            if (*g_WantedWindow_init_small == 0) {
                h = 500;
                w = 0x28a;
            }
            halfW = w >> 1;
        } else {
            h = 0x2bf;
            halfW = 0x1c9;
            w = 0x392;
        }
        F<uint32_t>(self, 0x1c) = (*g_WantedWindow_init_screen_w >> 1) - halfW;
        F<uint32_t>(self, 0x20) = (*g_WantedWindow_init_screen_h >> 1) - (h >> 1);
    }

    F<uint32_t>(self, 0x24) = w;
    F<uint32_t>(self, 0x28) = h;
    F<uint32_t>(self, 0x30) = 0;

    uint32_t selected = 0;
    for (;;) {
        void *arr = F<void *>(self, 0x38);
        uint32_t count = F<uint32_t>(arr, 0x0);
        if (selected >= count) {
            selected = F<uint32_t>(self, 0x30);
            if (selected == count - 1) {
                if (Wanted_isActive(ArrayItem(arr, selected)) == 0) {
                    selected = 0;
                    F<uint32_t>(self, 0x30) = 0;
                } else {
                    selected = F<uint32_t>(self, 0x30);
                }
            }
            break;
        }
        if (Wanted_isActive(ArrayItem(arr, selected)) != 0) {
            F<uint32_t>(self, 0x30) = selected;
            arr = F<void *>(self, 0x38);
            count = F<uint32_t>(arr, 0x0);
            if (selected == count - 1) {
                if (Wanted_isActive(ArrayItem(arr, selected)) == 0) {
                    selected = 0;
                    F<uint32_t>(self, 0x30) = 0;
                } else {
                    selected = F<uint32_t>(self, 0x30);
                }
            }
            break;
        }
        ++selected;
    }

    F<uint32_t>(self, 0x34) = selected;
    WantedWindow_selectWanted(self, selected);

    void *buttons = operator_new(0x0c);
    Array_TouchButton_ctor(buttons);
    F<void *>(self, 0x0c) = buttons;
    ArraySetLength_TouchButton(2, buttons);

    void **textHolder = g_WantedWindow_init_text;
    void *button = operator_new(0xc8);
    String *text = GameText_getText(*textHolder, 0xc93);
    int x = F<int>(self, 0x1c);
    int bw = F<int>(self, 0x24);
    int help = Layout_getHelpButtonOffset(layout);
    TouchButton_ctor6(button, text, 3, x + bw - help, F<int>(self, 0x20), 0x12);
    *(void **)((char *)F<void *>(buttons, 0x4) + 4) = button;

    button = operator_new(0xc8);
    text = GameText_getText(*textHolder, 0x81);
    x = F<int>(self, 0x1c);
    bw = F<int>(self, 0x24);
    help = Layout_getHelpButtonOffset(layout);
    int width = TouchButton_getWidth(*(void **)((char *)F<void *>(buttons, 0x4) + 4));
    TouchButton_ctor6(button, text, 3, x + bw - help - width + F<int>(layout, 0x38),
                      F<int>(self, 0x20), 0x12);
    *(void **)F<void *>(buttons, 0x4) = button;

    TouchButton_setAlwaysPressed(*(void **)((char *)F<void *>(buttons, 0x4) + 4), true);
    Layout_setWindowDimensions(layout, F<int>(self, 0x1c), F<int>(self, 0x20),
                               F<int>(self, 0x24), F<int>(self, 0x28));

    layout = *layoutHolder;
    F<int>(self, 0xa4) = (F<int>(layout, 0x34) + F<int>(layout, 0x70)) *
                         F<int>(F<void *>(self, 0x38), 0x0);
    F<int>(self, 0xa8) =
        (((((F<int>(self, 0x28) - F<int>(layout, 0x10)) - F<int>(layout, 0x0c)) -
           F<int>(layout, 0x20)) - F<int>(layout, 0x24)) - F<int>(layout, 0x5c)) +
        F<int>(layout, 0x2c);

    if (F<void *>(self, 0x18) != 0) {
        operator_delete(TouchButton_dtor(F<void *>(self, 0x18)));
    }
    F<void *>(self, 0x4) = 0;
    F<void *>(self, 0x18) = 0;
    F<uint16_t>(self, 0x14) = 0;

    button = operator_new(0xc8);
    text = GameText_getText(*textHolder, 0x1a8);
    layout = *layoutHolder;
    TouchButton_ctor8(button, text, 0,
                      F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1) + F<int>(layout, 0x2c),
                      (((F<int>(self, 0x20) - F<int>(layout, 0x2c)) + F<int>(self, 0x28)) -
                       F<int>(layout, 0x10)) - F<int>(layout, 0x24),
                      activeWidth, 0x21, 4);
    F<uint8_t>(self, 0x15) = 0;
    F<void *>(self, 0x18) = button;
    F<int>(self, 0x0) = 1;
    return 1;
}
