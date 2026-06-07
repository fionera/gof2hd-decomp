#include "class.h"

// MenuTouchWindow::inCinematicMode()
struct MenuTouchWindow { bool inCinematicMode(); };
bool MenuTouchWindow::inCinematicMode()
{
    return i32(this, 0x16c) == 0xd;
}
