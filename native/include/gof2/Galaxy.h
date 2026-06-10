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
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline uint16_t &US(void *p, int off) { return *(uint16_t *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

struct Galaxy { void* _opaque; };  // no offset accesses observed
#endif
