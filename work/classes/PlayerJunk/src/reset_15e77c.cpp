#include "class.h"

extern "C" void KIPlayer_reset(void *self);                 // blx 0x74518
extern "C" void PlayerJunk_resetTail(void *self, int one);  // b.w 0x1abe08 (veneer)

// PlayerJunk::reset() - reset base KIPlayer, clear state, tail-call the show/visible setter.
extern "C" void _ZN10PlayerJunk5resetEv(void *self) {
    KIPlayer_reset(self);
    F<int>(self, 0x88) = 0;
    return PlayerJunk_resetTail(self, 1);
}
