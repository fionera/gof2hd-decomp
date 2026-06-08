#include "class.h"

// AEGeometry::render(AEGeometry*) -- resolved global render entry for the beam mesh.
extern "C" void AEGeometry_render(AEGeometry *geo);

// TractorBeam::render()
//   Only draws while the beam is active; forwards to the geometry's render.
void TractorBeam_render(TractorBeam *self)
{
    if (u8(self, 0x10) == 0)
        return;
    AEGeometry_render(F<AEGeometry *>(self, 0x14));
}
