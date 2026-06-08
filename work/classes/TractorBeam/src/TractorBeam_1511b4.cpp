#include "class.h"

// AEGeometry::AEGeometry(mesh-id, canvas, visible) ctor @ 0x0007207c.
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t meshId, PaintCanvas *canvas, bool visible);
// Hidden PC-relative pointer to the active PaintCanvas root.
extern void *const gCanvasRoot __attribute__((visibility("hidden")));

// TractorBeam::TractorBeam(AEGeometry*, int)
//   Zeroes the embedded state then constructs the beam mesh geometry. The mesh
//   id is the base id 0x3798 offset by the (truncated) integer argument.
void TractorBeam_ctor(TractorBeam *self, AEGeometry * /*unused*/, int param2)
{
    f32(self, 0x00) = 0.0f;
    f32(self, 0x04) = 0.0f;
    f32(self, 0x08) = 0.0f;
    u32(self, 0x0c) = 0;
    *(uint16_t *)((char *)self + 0x10) = 0;

    AEGeometry *geo = (AEGeometry *)operator new(0xc0);
    uint16_t meshId = (uint16_t)((short)param2 + 0x3798);
    PaintCanvas *canvas = *(PaintCanvas **)(*(void **)gCanvasRoot);
    AEGeometry_ctor(geo, meshId, canvas, false);

    F<AEGeometry *>(self, 0x14) = geo;
    u32(self, 0x18) = 0;
}
