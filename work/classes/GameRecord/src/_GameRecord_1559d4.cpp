#include "class.h"

// GameRecord::~GameRecord()
GameRecord::~GameRecord()
{
    AEString_dtor((char *)this + 0x194);
    AEString_dtor((char *)this + 0x188);
}
