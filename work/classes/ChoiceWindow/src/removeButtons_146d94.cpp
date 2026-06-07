#include "class.h"

void ChoiceWindow::removeButtons()
{
    F<uint8_t>(this, 0x59) = 0;
}
