#include "class.h"

// FModSound::FModSound()
extern "C" void *AEFile_GetAppRootDir();

struct FModSound { FModSound(); };
FModSound::FModSound()
{
    u32(this, OFF_SYSTEM) = 0;
    u32(this, 0x2400) = 0;
    u32(this, 0x2404) = 0;
    pp(this, 0xc) = AEFile_GetAppRootDir();
    u8(this, 0x10) = 1;
    for (int i = 0x11; i != 0x15; i++)
        u8(this, i) = 1;
    for (int i = 0; i != 0x23d4; i += 4)
        u32(this, i + OFF_EVENTS) = 0;
    u32(this, 0x2438) = 0;
    u32(this, 0x2434) = 0;
    u32(this, 0x2424) = 0;
    u32(this, 0x2428) = 0;
    u32(this, 0x242c) = 0;
    u32(this, 0x2430) = 0;
}
