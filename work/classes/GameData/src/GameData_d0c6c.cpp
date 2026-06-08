#include "class.h"

// GameData::GameData() -- default-constructs 8 embedded AbyssEngine::String members.
extern "C" void _ZN11AbyssEngine6StringC1Ev(AbyssEngine::String *self);

GameData::GameData()
{
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x18));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x24));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x30));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x7c));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x88));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0x94));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0xa8));
    _ZN11AbyssEngine6StringC1Ev((AbyssEngine::String *)((char *)this + 0xb8));
}
