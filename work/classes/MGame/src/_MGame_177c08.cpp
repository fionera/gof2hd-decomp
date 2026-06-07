#include "class.h"

// PIC globals: the MGame vtable base (hidden -> direct pc-relative single-deref of the
// value) and the String destructor.
__attribute__((visibility("hidden"))) extern void *MGame_vtable;
extern "C" void MGame_OnRelease(MGame *self);
extern "C" void String_dtor(void *s);

// MGame::~MGame() complete destructor: install the MGame vtable, release game state,
// destroy the embedded String at 0x64, and return this.
extern "C" MGame *MGame_dtor(MGame *self) {
    *(void **)self = (char *)MGame_vtable + 8;
    MGame_OnRelease(self);
    String_dtor((char *)self + 0x64);
    return self;
}
