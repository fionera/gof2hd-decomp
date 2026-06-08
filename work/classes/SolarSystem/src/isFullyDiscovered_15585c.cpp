#include "class.h"

struct Galaxy;
// Galaxy::getVisited() -> char* visited-flags table.
extern "C" char *Galaxy_getVisited(Galaxy *g);

// Galaxy singleton: global variable holds a Galaxy*; one register caches that pointer,
// and the Galaxy's first member is passed to getVisited each iteration.
extern Galaxy *gGalaxyDiscover __attribute__((visibility("hidden")));

// SolarSystem::isFullyDiscovered() — every station in array +0x38 must be visited.
extern "C" int SolarSystem_isFullyDiscovered(SolarSystem *self)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
    uint32_t i = 0;
    Galaxy *gal = gGalaxyDiscover;
    while (true) {
        if (i >= arr[0])
            return 1;
        char *visited = Galaxy_getVisited(*(Galaxy **)gal);
        arr = F<uint32_t *>(self, 0x38);
        uint32_t flagIdx = ((int *)arr[1])[i];
        i++;
        if (visited[flagIdx] == 0)
            return 0;
    }
}
