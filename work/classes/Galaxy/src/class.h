// Galaxy — top-level class (NO namespace). Byte-exact decomp scaffold.
// Field offsets taken from the work-items:
//   +0x00  uint8_t* stations  (heap array of 0x87 visited-flags)
//   +0x04  AEArray* systems   (Array<SolarSystem*>)
#ifndef WORK_CLASSES_GALAXY_SRC_CLASS_H
#define WORK_CLASSES_GALAXY_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Galaxy {
    // @portable-fields
    uint8_t* stations; // 0x0
    AEArray* systems; // 0x4

    Galaxy();
    ~Galaxy();

    void reset();
    int getSystem(int index);
    int getStation(int index);
    void setVisited(bool *src, int count);
    void visitStation(int index);
    int distancePercent(int x1, int y1, int x2, int y2);
    int invDistancePercent(int x1, int y1, int x2, int y2);
    void *getPlasmaProbabilities(void *station);
    void *getAsteroidProbabilities(void *station);
};

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline uint16_t &US(void *p, int off) { return *(uint16_t *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

#endif
