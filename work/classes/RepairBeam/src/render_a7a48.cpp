#include "class.h"

// RepairBeam::render() — render each beam geometry whose target id slot is active (!= -1).
extern "C" void AEGeometry_render(void *geom);

extern "C" void RepairBeam_render(RepairBeam *self)
{
    int **targetIds = F<int **>(self, 0x14);
    for (unsigned i = 0; i < (unsigned)targetIds[0][0]; i = i + 1) {
        if (targetIds[1][i] != -1) {
            void ***geoms = F<void ***>(self, 0x10);
            AEGeometry_render(geoms[1][i]);
        }
    }
}
