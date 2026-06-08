#include "class.h"

// AEGeometry::~AEGeometry() @ 0x00071fc8.
extern "C" void AEGeometry_dtor(AEGeometry *self);
extern "C" void operator_delete(void *p);

// TractorBeam::~TractorBeam() (D2). Returns `this`.
//   Destroys and frees the beam geometry, then clears the slot.
extern "C" TractorBeam *_ZN11TractorBeamD2Ev(TractorBeam *self)
{
    AEGeometry *geo = F<AEGeometry *>(self, 0x14);
    if (geo != 0) {
        AEGeometry_dtor(geo);
        operator_delete(geo);
    }
    F<AEGeometry *>(self, 0x14) = 0;
    return self;
}
