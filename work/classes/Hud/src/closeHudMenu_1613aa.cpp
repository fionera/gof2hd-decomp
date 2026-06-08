#include "class.h"

extern "C" void ArrayReleaseClasses_TouchButton(void *arr);   // ArrayReleaseClasses<TouchButton*>
extern "C" void *Array_TouchButton_dtor(void *arr);           // Array<TouchButton*>::~Array
extern "C" void operator_delete(void *p);

extern "C" void Hud_closeHudMenu(Hud *self)
{
    if (self->f_18 != 0) {
        ArrayReleaseClasses_TouchButton(self->f_18);
        if (self->f_18 != 0) {
            void *p = Array_TouchButton_dtor(self->f_18);
            operator_delete(p);
        }
        self->f_18 = 0;
    }
    UC(self, 0x282) = 0;
}
