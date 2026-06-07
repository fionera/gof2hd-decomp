#include "class.h"

// MenuTouchWindow::hideMessage()
struct MenuTouchWindow { void hideMessage(); };
void MenuTouchWindow::hideMessage()
{
    u8(this, 0x170) = 0;
}
