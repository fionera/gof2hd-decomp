#include "class.h"

extern "C" {
unsigned int HangarList_getCurrentTab(void *list);
int HangarList_getCurrentTabItems(void *list);
void TouchButton_setAlwaysPressed(void *btn, bool pressed);
int TouchButton_isTouched(void *btn);
int TouchButton_isVisible(void *btn);
float VectorSignedToFloat(int v, int mode);
void HangarWindow_transaction(HangarWindow *self, bool buy);
void ListItemWindow_update(void *win, int delta);
}

extern "C" void HangarWindow_update(HangarWindow *self, int delta)
{
    if (F<uint8_t>(self, 0xc) == 0)
        return;
    F<int>(self, 8) = delta;

    if (F<int>(self, 0x58) == 1) {
        ListItemWindow_update(F<void *>(self, 0x18), delta);
        return;
    }

    unsigned int tab = HangarList_getCurrentTab(F<void *>(self, 0x14));
    Array<void *> *buttons = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < buttons->length; i++) {
        bool pressed = true;
        if (i != tab && !(tab == 3 && i == 0))
            pressed = (i == 2 && tab == 4);
        TouchButton_setAlwaysPressed(buttons->data[i], pressed);
    }

    if (F<uint8_t>(self, 0xd0) == 0) {
        float v = F<float>(self, 0xc4) * F<float>(self, 200);
        F<float>(self, 200) = v;
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            float pos = VectorSignedToFloat(F<int>(self, 0xb4), 0);
            F<int>(self, 0xb4) = (int)(v + pos);
        }
    }

    if (F<int>(self, 0xb4) > 0) {
        float f = VectorSignedToFloat(-F<int>(self, 0xb4), 0);
        F<float>(self, 0xc4) = 1.0f;
        F<float>(self, 200) = f * 0.5f;
    }

    if (HangarList_getCurrentTabItems(F<void *>(self, 0x14)) != 0) {
        int diff = F<int>(self, 0xd8) - F<int>(self, 0xd4);
        if (diff < 0) {
            if (F<int>(self, 0xb4) < diff) {
                float f = VectorSignedToFloat(diff - F<int>(self, 0xb4), 0);
                F<float>(self, 0xc4) = 1.0f;
                F<float>(self, 200) = f * 0.5f;
            }
        } else {
            F<int>(self, 200) = 0;
            F<int>(self, 0xb4) = 0;
        }
    }

    if (F<uint8_t>(self, 0x88) != 0) {
        void *btnUp = G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x20);
        void *btnDown = G<void *>(G<void *>(F<void *>(self, 0x24), 4), 0x24);
        if (TouchButton_isTouched(btnUp) != 0 || TouchButton_isTouched(btnDown) != 0) {
            int t6c = F<int>(self, 0x6c) + delta;
            int t70 = F<int>(self, 0x70) + delta;
            F<int>(self, 0x6c) = t6c;
            F<int>(self, 0x70) = t70;
            int threshold = (t6c > 0x5dc) ? 0x1e : 200;
            if (t70 > threshold && (F<int>(self, 0x70) = 0,
                                    F<uint8_t>(self, 0x88) != 0 || F<uint8_t>(self, 0x89) != 0)) {
                if (TouchButton_isTouched(btnDown) != 0 && TouchButton_isVisible(btnDown) != 0) {
                    int n = (F<int>(self, 0x6c) > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, true);
                } else if (TouchButton_isTouched(btnUp) != 0 &&
                           TouchButton_isVisible(btnUp) != 0) {
                    int n = (F<int>(self, 0x6c) > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, false);
                }
            }
        }
    }
}
