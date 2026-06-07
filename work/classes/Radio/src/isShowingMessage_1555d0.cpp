#include "class.h"

bool Radio::isShowingMessage()
{
    return F<void *>(this, 0x04) != 0;
}
