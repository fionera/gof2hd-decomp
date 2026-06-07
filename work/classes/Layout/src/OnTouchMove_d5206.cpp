#include "class.h"

extern "C" void TouchButton_OnTouchMove(void *btn, int x, int y);     // 0x74764
extern "C" void ChoiceWindow_OnTouchMove(void *cw, int x, int y);     // 0x7474c
extern "C" int TouchButton_isVisible(void *btn);                       // 0x74ec0
extern "C" int Layout_dispatchTouchMove(void *btn, int x, int y);      // tail 0x1ac0d8

// Layout::OnTouchMove(int x, int y)
extern "C" int Layout_OnTouchMove(Layout *self, int x, int y) {
    if (F<uint8_t>(self, 0x3cc) != 0)
        TouchButton_OnTouchMove(F<void *>(self, 0x3bc), x, y);
    if (F<void *>(self, 0x3c4) != 0 && F<uint8_t>(self, 0x0) != 0) {
        ChoiceWindow_OnTouchMove(F<void *>(self, 0x3c4), x, y);
        return 0;
    }
    void *btn;
    if (TouchButton_isVisible(F<void *>(self, 0x3b4)) == 0)
        btn = F<void *>(self, 0x3b8);
    else
        btn = F<void *>(self, 0x3b4);
    return Layout_dispatchTouchMove(btn, x, y);
}
