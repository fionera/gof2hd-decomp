#include "class.h"

// MenuTouchWindow::isMakingScreenshot() -> (unsigned)field < 0x80000000, i.e. signed >= 0.
struct MenuTouchWindow { bool isMakingScreenshot(); };
bool MenuTouchWindow::isMakingScreenshot()
{
    return u32(this, 0x184) < 0x80000000u;
}
