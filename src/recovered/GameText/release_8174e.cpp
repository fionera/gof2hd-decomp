#include "class.h"

// GameText::release() -- destroys each owned text-table entry via its vtable[1], then nulls it.
extern "C" void GameText_release(GameText *self)
{
    void **data = F<void **>(self, 0xc);
    if (data == 0) return;
    int i = 0;
    for (int byteoff = 0; i < i32(self, 0x1c); byteoff += 4) {
        void *obj = *(void **)((char *)F<void **>(self, 0xc) + byteoff);
        void **slot;
        if (obj == 0) {
            slot = (void **)((char *)F<void **>(self, 0xc) + i * 4);
        } else {
            void (**vt)(void *) = *(void (***)(void *))obj;
            vt[1](obj);
            slot = (void **)((char *)F<void **>(self, 0xc) + byteoff);
        }
        *slot = 0;
        ++i;
    }
}
