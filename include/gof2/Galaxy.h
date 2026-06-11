#ifndef GOF2_GALAXY_H
#define GOF2_GALAXY_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy — top-level class (NO namespace). Byte-exact decomp scaffold.
// Field offsets taken from the work-items:
//   +0x00  uint8_t* stations  (heap array of 0x87 visited-flags)
//   +0x04  AEArray* systems   (Array<SolarSystem*>)




// ---- tiny offset-cast helpers -------------------------------------------------
static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

// Galaxy — owns the per-station visited-flag array (at +0x0) and the SolarSystem* Array (+0x4).
// Field access is via the B/I/U/P offset-cast helpers above; storage covers offsets 0x0..0x8.
class Galaxy {
public:
    Galaxy();

    void reset();
    int distancePercent(int x1, int y1, int x2, int y2);
    void visitStation(int index);
    int invDistancePercent(int x1, int y1, int x2, int y2);
    void setVisited(bool *src, int count);
    int getSystem(int index);
    void *getPlasmaProbabilities(void *station);
    void *getAsteroidProbabilities(void *station);
    int getStation(int index);

    char field_storage[0x8];
};
#endif
