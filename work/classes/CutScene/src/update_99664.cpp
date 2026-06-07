#include "class.h"

// update(): single unconditional tail-call thunk (b.w trampoline).
extern "C" void CutScene_update_tail(CutScene *self);

void CutScene::update()
{
    return CutScene_update_tail(this);
}
