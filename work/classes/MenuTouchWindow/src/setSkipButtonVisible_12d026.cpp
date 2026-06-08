#include "class.h"

extern "C" void _mtw_TouchButton_setVisible(void *btn, bool v);

// MenuTouchWindow::setSkipButtonVisible(bool). r0=this, r1=visible.
// Shows/hides all type-0x12 buttons (id 0).
struct MenuTouchWindow { void setSkipButtonVisible(bool visible); };
void MenuTouchWindow::setSkipButtonVisible(bool visible)
{
    void **arr = (void **)pp(this, 0x4);
    if (arr != 0) {
        for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
            void *btn = ((void **)arr[1])[i];
            if (btn != 0 && i32(btn, 0) == 0x12 && i32(btn, 4) == 0) {
                _mtw_TouchButton_setVisible(btn, visible);
                arr = (void **)pp(this, 0x4);
            }
        }
    }
}
