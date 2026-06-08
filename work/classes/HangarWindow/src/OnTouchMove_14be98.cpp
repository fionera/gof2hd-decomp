#include "class.h"

extern "C" {
void Layout_OnTouchMove(void *layout, int touch);
void ListItemWindow_OnTouchMove(void *win, int touch);
void TouchButton_OnTouchMove(void *btn, int touch);
void TouchButton_resetTouch(void *btn);
int TouchButton_isTouched(void *btn);
void ChoiceWindow_OnTouchMove(void *win, int touch);
void HangarWindow_setSellMode(HangarWindow *self);
}

__attribute__((visibility("hidden"))) extern void **g_hw_layout;
__attribute__((visibility("hidden"))) extern int *g_hw_screenWidth;

extern "C" unsigned int HangarWindow_OnTouchMove(HangarWindow *self, int touch, int coord)
{
    void *layout = *g_hw_layout;
    Layout_OnTouchMove(layout, touch);

    if (F<uint8_t>(self, 0x3c) != 0) {
        if (F<uint8_t>(self, 0xae) != 0) {
            for (int i = 0xc; i != 0x11; i++)
                TouchButton_OnTouchMove(G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4), touch);
            TouchButton_OnTouchMove(G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x44), touch);
        } else if (F<uint8_t>(self, 0xb0) != 0) {
            for (int i = 0x12; i != 0x17; i++)
                TouchButton_OnTouchMove(G<void *>(G<void *>(F<void *>(self, 0x24), 4), i * 4), touch);
        }
        ChoiceWindow_OnTouchMove(F<void *>(self, 0x20), touch);
        return 0;
    }

    if (F<int>(self, 0x58) == 1) {
        ListItemWindow_OnTouchMove(F<void *>(self, 0x18), touch);
        return 0;
    }

    if (G<int>(layout, 0xc) < coord && coord < *g_hw_screenWidth - G<int>(layout, 0x10)) {
        int dy = coord - F<int>(self, 0xb8);
        F<int>(self, 0xc0) = dy;
        F<float>(self, 0xc4) = 1.0f;
        F<int>(self, 0xb4) = F<int>(self, 0xb4) + dy;
        F<int>(self, 0xb8) = coord;

        void *btnUp = G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x20);
        void *btnDown = G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x24);
        int touched = TouchButton_isTouched(btnUp);
        if (touched != 0)
            touched = 1;
        else
            touched = TouchButton_isTouched(btnDown);

        int adist = coord - F<int>(self, 0xcc);
        if (adist < 0)
            adist = -adist;

        if (touched == 0 && adist > 5) {
            F<int>(self, 0x6c) = 0;
            F<int>(self, 0x70) = 0;
            Array<void *> *buttons = F<Array<void *> *>(self, 0x24);
            for (unsigned int i = 0; i < buttons->length; i++)
                TouchButton_OnTouchMove(buttons->data[i], touch);
            HangarWindow_setSellMode(self);
            F<uint8_t>(self, 0xd2) = 0;
            F<void *>(self, 0x68) = 0;
            TouchButton_resetTouch(btnUp);
            TouchButton_resetTouch(btnDown);
        }
    }

    Array<void *> *tabs = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < tabs->length; i++)
        TouchButton_OnTouchMove(tabs->data[i], touch);
    return 0;
}
