#include "class.h"

// ListItemWindow::shows3DShip() -- raw uint8 getter at +0x54.
struct ListItemWindow { uint8_t shows3DShip(); };
uint8_t ListItemWindow::shows3DShip()
{
    return u8(this, 0x54);
}
