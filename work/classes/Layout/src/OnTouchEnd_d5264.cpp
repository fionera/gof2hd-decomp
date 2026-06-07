#include "class.h"

extern "C" uint8_t TouchButton_OnTouchEnd(void *btn, int x, int y);   // 0x74788
extern "C" int ChoiceWindow_OnTouchEnd(void *cw, int x, int y);       // 0x74770
extern "C" int TouchButton_isVisible(void *btn);                       // 0x74ec0
extern "C" int Layout_dispatchTouchEnd(void *btn, int x, int y);       // tail 0x1ac0e8

// Layout::OnTouchEnd(int x, int y)
extern "C" int Layout_OnTouchEnd(Layout *self, int x, int y) {
    if (F<uint8_t>(self, 0x0) == 0 && F<uint8_t>(self, 0x3cc) != 0)
        F<uint8_t>(self, 0x3c0) = TouchButton_OnTouchEnd(F<void *>(self, 0x3bc), x, y);
    if (F<void *>(self, 0x3c4) != 0 && F<uint8_t>(self, 0x0) != 0)
        return ChoiceWindow_OnTouchEnd(F<void *>(self, 0x3c4), x, y) == 0;
    void *btn;
    if (TouchButton_isVisible(F<void *>(self, 0x3b4)) == 0)
        btn = F<void *>(self, 0x3b8);
    else
        btn = F<void *>(self, 0x3b4);
    return Layout_dispatchTouchEnd(btn, x, y);
}
