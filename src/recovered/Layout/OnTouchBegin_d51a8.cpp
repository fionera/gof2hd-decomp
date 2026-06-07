#include "class.h"

extern "C" void TouchButton_OnTouchBegin(void *btn, int x, int y);    // 0x74734
extern "C" void ChoiceWindow_OnTouchBegin(void *cw, int x, int y);    // 0x7471c
extern "C" int TouchButton_isVisible(void *btn);                       // 0x74ec0
extern "C" int Layout_dispatchTouchBegin(void *btn, int x, int y);     // tail 0x1ac0c8

// Layout::OnTouchBegin(int x, int y)
extern "C" int Layout_OnTouchBegin(Layout *self, int x, int y) {
    if (F<uint8_t>(self, 0x3cc) != 0)
        TouchButton_OnTouchBegin(F<void *>(self, 0x3bc), x, y);
    if (F<void *>(self, 0x3c4) != 0 && F<uint8_t>(self, 0x0) != 0) {
        ChoiceWindow_OnTouchBegin(F<void *>(self, 0x3c4), x, y);
        return 0;
    }
    void *btn;
    if (TouchButton_isVisible(F<void *>(self, 0x3b4)) == 0)
        btn = F<void *>(self, 0x3b8);
    else
        btn = F<void *>(self, 0x3b4);
    return Layout_dispatchTouchBegin(btn, x, y);
}
