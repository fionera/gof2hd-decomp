#include "class.h"

// Hud::~Hud() — releases the equipment array, the event-queue array, the touch-button array,
// the uint array at +0x530, six engine objects (via a shared dtor thunk), and the block of
// String members from +0x10 up to +0x100. Returns this.
extern "C" void operator_delete(void *p);
extern "C" void *Array_Item_dtor(void *p);
extern "C" void *Array_ListItem_dtor(void *p);
extern "C" void ArrayReleaseClasses_TouchButton(void *arr);
extern "C" void *Array_TouchButton_dtor(void *arr);
extern "C" void *Array_uint_dtor(void *p);
extern "C" void Hud_subObjectDtor(void *p);   // DAT_0017134c shared sub-object dtor thunk
extern "C" void String_dtor(void *p);

extern "C" Hud *Hud_dtor(Hud *self)
{
    if (self->f_25c != 0) operator_delete(Array_Item_dtor(self->f_25c));
    self->f_25c = 0;

    if (self->f_264 != 0) operator_delete(Array_ListItem_dtor(self->f_264));
    self->f_264 = 0;

    if (self->f_18 != 0) {
        ArrayReleaseClasses_TouchButton(self->f_18);
        if (self->f_18 != 0) operator_delete(Array_TouchButton_dtor(self->f_18));
    }
    self->f_18 = 0;

    if (self->f_530 != 0) operator_delete(Array_uint_dtor(self->f_530));
    self->f_530 = 0;

    Hud_subObjectDtor(B(self, 0x51c));
    Hud_subObjectDtor(B(self, 0x3b4));
    Hud_subObjectDtor(B(self, 0x228));
    Hud_subObjectDtor(B(self, 0x200));
    Hud_subObjectDtor(B(self, 0x1f4));
    Hud_subObjectDtor(B(self, 0x1e0));

    for (int off = 0x100; off != 0x10; off -= 0xc)
        String_dtor(B(self, off));

    return self;
}
