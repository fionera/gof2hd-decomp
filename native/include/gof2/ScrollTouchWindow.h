#ifndef GOF2_SCROLLTOUCHWINDOW_H
#define GOF2_SCROLLTOUCHWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// ScrollTouchWindow is a top-level class. Keep the object opaque and access
// fields only through offsets from the work-items.


namespace AbyssEngine {



}



static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline unsigned char &UC(void *p, int off) { return *(unsigned char *)((char *)p + off); }
static inline unsigned short &US(void *p, int off) { return *(unsigned short *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

struct ScrollTouchWindow { void* _opaque; };  // no offset accesses observed
#endif
