#include "class.h"

extern "C" void *KIPlayer_dtor(void *self);                 // blx 0x732b8 (returns this)
extern "C" void PlayerJunk_dtorTail(void *self);            // b.w 0x1ab098 (veneer -> delete)

// PlayerJunk::~PlayerJunk() - destroy the base KIPlayer then tail-call the deleting veneer
// with the pointer the base dtor returns. Mangled so the symbol demangles to ~PlayerJunk.
extern "C" void _ZN10PlayerJunkD0Ev(void *self) {
    return PlayerJunk_dtorTail(KIPlayer_dtor(self));
}
