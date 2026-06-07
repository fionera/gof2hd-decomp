#include "class.h"

extern "C" void GameText_release(GameText *self);
extern "C" void operator_delete_arr(void *p);
extern "C" void AEString_dtor(void *s);
// Tail veneer to the base/Array<int> destructor; takes and returns this.
extern "C" GameText *GameText_dtor_tail(GameText *self);

// GameText::~GameText() -> releases owned text table + string, tail-calls base dtor.
extern "C" GameText *_ZN8GameTextD2Ev(GameText *self)
{
    GameText_release(self);
    void *p = F<void *>(self, 0xc);
    if (p != 0) operator_delete_arr(p);
    F<void *>(self, 0xc) = 0;
    AEString_dtor((char *)self + 0x10);
    return GameText_dtor_tail(self);
}
