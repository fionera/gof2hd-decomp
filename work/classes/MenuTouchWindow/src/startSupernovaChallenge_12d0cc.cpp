#include "class.h"

// MenuTouchWindow::startSupernovaChallenge()
// The target's 16 bytes disassemble as bad/ARM-misdecoded data: this is a thin tail
// thunk that forwards to the supernova-challenge start handler. Faithful recoverable
// translation via an extern helper.
extern "C" void _mtw_startSupernovaChallenge_impl(void *self);

struct MenuTouchWindow { void startSupernovaChallenge(); };
void MenuTouchWindow::startSupernovaChallenge()
{
    _mtw_startSupernovaChallenge_impl(this);
}
