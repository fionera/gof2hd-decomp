#include "class.h"

// MissionsWindow::hangarNeedsUpdate() -- raw uint8 getter at +0x23.
struct MissionsWindow { uint8_t hangarNeedsUpdate(); };
uint8_t MissionsWindow::hangarNeedsUpdate()
{
    return u8(this, 0x23);
}
