#ifndef GOF2_GALAXY_H
#define GOF2_GALAXY_H
#include "gof2/common.h"

class SolarSystem;
class Station;

// Galaxy — the star-map model: owns the per-station visited-flag array and the
// table of SolarSystem objects loaded from the game's binary data files.
class Galaxy {
public:
    Galaxy();
    ~Galaxy();

    // Reset every per-station visited flag to "not visited".
    void reset();

    // Percentage-style proximity metric between two map points (planar distance,
    // scaled to roughly 0..100). invDistancePercent() returns its 100-complement.
    int distancePercent(int x1, int y1, int x2, int y2);
    int invDistancePercent(int x1, int y1, int x2, int y2);

    // Mark a station/system index as visited.
    void visitStation(int index);
    void setSystemVisited(int systemId);

    // Bulk-load the visited flags from a bool array, zero-filling the remainder.
    void setVisited(bool *src, int count);

    // The SolarSystem index stored in slot `index` (0 for negative indices).
    int getSystem(int index);

    // Probability tables (plasma / asteroid resource yields) for a given station.
    void *getPlasmaProbabilities(Station *station);
    void *getAsteroidProbabilities(Station *station);

    // Resolve a station index; negative resolves to the player's current station.
    int getStation(int index);

    // Euclidean distance between two systems' map positions (Z compressed by 1/10),
    // scaled by the global unit factor. Returns 0 when both refer to the same system.
    float distance(SolarSystem *a, SolarSystem *b);

    // Accessors for the two owned members.
    Array<SolarSystem *> *getSystems();
    uint8_t *getVisited();

    uint8_t *visited;                 // heap array of 0x87 per-station visited flags
    Array<SolarSystem *> *systems;    // table of loaded SolarSystem objects
};
extern Galaxy* gGalaxy;            // canonical Galaxy singleton (binary .bss 0x2281ac)

#endif
