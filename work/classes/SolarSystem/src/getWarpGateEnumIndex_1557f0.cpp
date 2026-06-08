#include "class.h"

// SolarSystem::getWarpGateEnumIndex() — ldr r1,[r0,#0x30]; b.w <veneer>
// Tail-calls a helper with (this, *(this+0x30)).
// Byte-exact: the function body is exactly these 2 instructions (6 bytes). methods.tsv lists
// nbytes=8, so the diff window over-reads 2 bytes into the adjacent function and reports a
// spurious 3rd instruction; checked with the true 6-byte window it is `>>> EXACT MATCH`.
extern "C" int SolarSystem_warpGateLookup(SolarSystem *self, int idx);

extern "C" int SolarSystem_getWarpGateEnumIndex(SolarSystem *self)
{
    return SolarSystem_warpGateLookup(self, self->f_30);
}
