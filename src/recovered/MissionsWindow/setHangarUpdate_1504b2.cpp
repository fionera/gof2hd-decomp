#include "class.h"

// MissionsWindow::setHangarUpdate(bool) -- store the flag byte at +0x23.
struct MissionsWindow { void setHangarUpdate(bool); };
void MissionsWindow::setHangarUpdate(bool v)
{
    u8(this, 0x23) = v;
}
