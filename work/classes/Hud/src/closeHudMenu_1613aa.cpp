#include "class.h"

extern "C" void ArrayReleaseClasses_TouchButton(void *arr);   // ArrayReleaseClasses<TouchButton*>
extern "C" void *Array_TouchButton_dtor(void *arr);           // Array<TouchButton*>::~Array
extern "C" void operator_delete(void *p);

extern "C" void Hud_closeHudMenu(Hud *self)
{
    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_TouchButton(P(self, 0x18));
        if (P(self, 0x18) != 0) {
            void *p = Array_TouchButton_dtor(P(self, 0x18));
            operator_delete(p);
        }
        P(self, 0x18) = 0;
    }
    UC(self, 0x282) = 0;
}
