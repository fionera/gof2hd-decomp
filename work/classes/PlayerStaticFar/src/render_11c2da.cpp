#include "class.h"

// render(): a single unconditional tail-call thunk (b.w into the PLT-ish
// trampoline). The body is one branch to the resolved render routine.
extern "C" void PlayerStaticFar_render_tail(PlayerStaticFar *self);

void PlayerStaticFar::render()
{
    return PlayerStaticFar_render_tail(this);
}
