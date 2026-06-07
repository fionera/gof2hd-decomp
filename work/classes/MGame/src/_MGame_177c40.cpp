#include "class.h"

// MGame::~MGame() deleting destructor: run the complete dtor (which returns `this`)
// then tail-call operator delete with that pointer — so `this` is never saved.
extern "C" MGame *MGame_dtor(MGame *self);
extern "C" void MGame_opdelete(void *p);

extern "C" void MGame_deleting_dtor(MGame *self) {
    MGame_opdelete(MGame_dtor(self));
}
