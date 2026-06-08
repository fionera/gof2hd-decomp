#include "class.h"

extern "C" void _mtw_Status_resetGame();
extern "C" float _mtw_FModSound_stop(void *snd);
extern "C" void _mtw_FModSound_play(void *snd, int id, void *pos, float v);

// FModSound singleton holder, a Status-state object holder, and a transition-thunk holder.
extern void *const gGof2Fmod __attribute__((visibility("hidden")));
extern void *const gGof2StatusObj __attribute__((visibility("hidden")));
extern void *const gGof2Transition __attribute__((visibility("hidden")));

typedef void (*TransitionFn)(void *app, int a, int b);

// MenuTouchWindow::startGOF2()
struct MenuTouchWindow { void startGOF2(); };
void MenuTouchWindow::startGOF2()
{
    _mtw_Status_resetGame();
    void *snd = *(void **)gGof2Fmod;
    // copy the saved fade value (+0x1a4) into the status object at +0x2c
    *(uint32_t *)((char *)*(void **)gGof2StatusObj + 0x2c) = u32(this, 0x1a4);
    float v = _mtw_FModSound_stop(snd);
    _mtw_FModSound_play(snd, 0x8f, 0, v);
    void *appHolder = *(void **)gGof2Transition;
    TransitionFn fn = *(TransitionFn *)((char *)appHolder); // thunk via DAT_001ab904
    fn(*(void **)appHolder, 2, 0);
}
