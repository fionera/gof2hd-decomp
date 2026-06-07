#include "class.h"

__attribute__((visibility("hidden"))) extern void *volatile g_ModMainMenu_vtable;

extern "C" void _ZN11ModMainMenuC2Ev(ModMainMenu *self)
{
    *(volatile uint8_t *)B(self, 0x10) = 0;
    *(volatile uint8_t *)B(self, 0x29) = 0;
    *(volatile int *)B(self, 0x0c) = 100;
    void *vtable = g_ModMainMenu_vtable;
    I(self, 0x14) = 0;
    I(self, 0x18) = 0;
    I(self, 0x1c) = 0;
    P(self, 0) = (char *)vtable + 8;
}
