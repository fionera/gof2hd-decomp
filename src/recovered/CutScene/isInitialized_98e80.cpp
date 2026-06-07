#include "class.h"

uint8_t CutScene::isInitialized()
{
    return u8(this, 0x5c);
}
