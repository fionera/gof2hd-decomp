#include "gof2/engine/render/ResourceTransform.h"




// ---- _ResourceTransform_77ee4.cpp ----
// AbyssEngine::ResourceTransform::~ResourceTransform()
// Releases the two owned heap arrays (at +0x40 and +0x48) and nulls the slots.
// Returns `this` (the ABI quirk for the base/complete destructor variant).

void *_ZN11AbyssEngine17ResourceTransformD2Ev(ResourceTransform *self)
{
    void *p40 = self->field_0x40;
    if (p40 != 0)
        operator delete[](p40);
    self->field_0x40 = 0;

    void *p48 = self->field_0x48;
    if (p48 != 0)
        operator delete[](p48);
    self->field_0x48 = 0;

    return self;
}
