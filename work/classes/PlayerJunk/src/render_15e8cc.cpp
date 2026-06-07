#include "class.h"

extern "C" void AEGeometry_render(void *geom);   // blx 0x72238
extern "C" void PlayerJunk_renderTail(void *self);  // b.w 0x1ac3a8 (veneer)

// PlayerJunk::render() - render the geometry if present, then (unless the state is 3 or 4)
// tail-call the base render veneer.
extern "C" void _ZN10PlayerJunk6renderEv(void *self) {
    void *geom = F<void *>(self, 0x78);
    if (geom != 0)
        AEGeometry_render(geom);
    if ((uint32_t)(F<int>(self, 0x88) - 3) > 1)
        return PlayerJunk_renderTail(self);
}
