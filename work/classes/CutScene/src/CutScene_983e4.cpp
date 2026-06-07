#include "class.h"

CutScene::CutScene(int param)
{
    i32(this, 0x18) = 0;
    i32(this, 0x1c) = 0;
    i32(this, 0x88) = param;
    i32(this, 0x0) = 0;
    i32(this, 0x64) = 0;
    i32(this, 0x68) = 0;
    i32(this, 0x6c) = -1;
    i32(this, 0x70) = -1;
    u8(this, 0x5c) = 0;
    i32(this, 0x38) = 0;
    __builtin_memset((char *)this + 0x8, 0, 16);
    __builtin_memset((char *)this + 0x28, 0, 16);
    u32(this, 0x24) = 0x3851b717u;
}
