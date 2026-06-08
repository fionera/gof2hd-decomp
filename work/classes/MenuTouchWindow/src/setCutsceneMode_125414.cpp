#include "class.h"

extern "C" void _mtw_TouchButton_setVisible(void *btn, bool v);

// MenuTouchWindow::setCutsceneMode(bool). Ghidra mislabels: r0=this, r1=mode.
// Sets this->0x238 = mode, then hides/shows all type-0x13 buttons (id 0) to !mode.
struct MenuTouchWindow { void setCutsceneMode(bool mode); };
void MenuTouchWindow::setCutsceneMode(bool mode)
{
    u8(this, 0x238) = (uint8_t)mode;
    void **arr = (void **)pp(this, 0x4);
    for (uint32_t i = 0; i < *(uint32_t *)arr; i++) {
        void *btn = ((void **)arr[1])[i];
        if (i32(btn, 0) == 0x13 && i32(btn, 4) == 0) {
            _mtw_TouchButton_setVisible(btn, (bool)((uint8_t)mode ^ 1));
        }
        arr = (void **)pp(this, 0x4);
    }
}
