#include "gof2/GameData.h"


extern "C" void _ZN11AbyssEngine6StringD1Ev(AbyssEngine::String *self);
extern "C" void _ZN11AbyssEngine6StringC1Ev(AbyssEngine::String *self);

// ---- _GameData_d06f4.cpp ----
// GameData::~GameData() -- destroys 8 embedded AbyssEngine::String members in reverse order.

GameData::~GameData()
{
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0xb8));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0xa8));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x94));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x88));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x7c));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x30));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x24));
    _ZN11AbyssEngine6StringD1Ev((AbyssEngine::String *)((char *)this + 0x18));
}

// ---- GameData_d0c6c.cpp ----
// GameData::GameData() -- default-constructs 8 embedded AbyssEngine::String members.

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
