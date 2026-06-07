#include "class.h"

// MenuTouchWindow::isShowingMessage()
struct MenuTouchWindow { uint8_t isShowingMessage(); };
uint8_t MenuTouchWindow::isShowingMessage()
{
    return u8(this, 0x170);
}
