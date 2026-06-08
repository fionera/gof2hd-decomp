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
    if (self->f_c == 0)
        return;
    self->f_8 = delta;

    if (self->f_58 == 1) {
        ListItemWindow_update(self->f_18, delta);
        return;
    }

    unsigned int tab = HangarList_getCurrentTab(self->f_14);
    Array<void *> *buttons = F<Array<void *> *>(self, 4);
    for (unsigned int i = 0; i < buttons->length; i++) {
        bool pressed = true;
        if (i != tab && !(tab == 3 && i == 0))
            pressed = (i == 2 && tab == 4);
        TouchButton_setAlwaysPressed(buttons->data[i], pressed);
    }

    if (self->f_d0 == 0) {
        float v = self->f_c4 * self->f_c8;
        self->f_c8 = v;
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            float pos = VectorSignedToFloat(self->f_b4, 0);
            self->f_b4 = (int)(v + pos);
        }
    }

    if (self->f_b4 > 0) {
        float f = VectorSignedToFloat(-self->f_b4, 0);
        self->f_c4 = 1.0f;
        self->f_c8 = f * 0.5f;
    }

    if (HangarList_getCurrentTabItems(self->f_14) != 0) {
        int diff = self->f_d8 - self->f_d4;
        if (diff < 0) {
            if (self->f_b4 < diff) {
                float f = VectorSignedToFloat(diff - self->f_b4, 0);
                self->f_c4 = 1.0f;
                self->f_c8 = f * 0.5f;
            }
        } else {
            self->f_c8 = 0;
            self->f_b4 = 0;
        }
    }

    if (self->f_88 != 0) {
        void *btnUp = G<void *>(G<void *>(self->f_24, 4), 0x20);
        void *btnDown = G<void *>(G<void *>(self->f_24, 4), 0x24);
        if (TouchButton_isTouched(btnUp) != 0 || TouchButton_isTouched(btnDown) != 0) {
            int t6c = self->f_6c + delta;
            int t70 = self->f_70 + delta;
            self->f_6c = t6c;
            self->f_70 = t70;
            int threshold = (t6c > 0x5dc) ? 0x1e : 200;
            if (t70 > threshold && (self->f_70 = 0,
                                    self->f_88 != 0 || F<uint8_t>(self, 0x89) != 0)) {
                if (TouchButton_isTouched(btnDown) != 0 && TouchButton_isVisible(btnDown) != 0) {
                    int n = (self->f_6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, true);
                } else if (TouchButton_isTouched(btnUp) != 0 &&
                           TouchButton_isVisible(btnUp) != 0) {
                    int n = (self->f_6c > 4000) ? 5 : 1;
                    for (; n != 0; n--)
                        HangarWindow_transaction(self, false);
                }
            }
        }
    }
}
