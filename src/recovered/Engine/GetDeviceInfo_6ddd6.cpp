#include "class.h"

extern "C" uint32_t NFC_isPad();
extern "C" uint32_t NFC_getWidth();
extern "C" uint32_t NFC_getHeight();

extern "C" void Engine_GetDeviceInfo(Engine *self)
{
    *(uint8_t *)((char *)self + 0x8) = NFC_isPad();
    *(uint32_t *)((char *)self + 0x0) = NFC_getWidth();
    *(uint32_t *)((char *)self + 0x4) = NFC_getHeight();
}
