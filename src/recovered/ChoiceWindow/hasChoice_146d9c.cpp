#include "class.h"

uint8_t ChoiceWindow::hasChoice()
{
    return F<uint8_t>(this, 0x58);
}
