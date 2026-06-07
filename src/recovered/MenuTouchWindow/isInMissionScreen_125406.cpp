#include "class.h"

// MenuTouchWindow::isInMissionScreen()
struct MenuTouchWindow { bool isInMissionScreen(); };
bool MenuTouchWindow::isInMissionScreen()
{
    return i32(this, 0x16c) == 9;
}
