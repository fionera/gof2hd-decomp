#ifndef WORK_CLASSES_SPACELOUNGE_SRC_CLASS_H
#define WORK_CLASSES_SPACELOUNGE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct SpaceLounge;
struct Agent;

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline unsigned int &U(void *p, int off) { return *(unsigned int *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline unsigned char &UC(void *p, int off) { return *(unsigned char *)((char *)p + off); }
static inline bool &BL(void *p, int off) { return *(bool *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
static inline void *PAt(void *p, int off) { return *(void **)((char *)p + off); }
static inline void *&PIndex(void *p, unsigned idx) { return *(void **)((char *)p + idx * 4); }

#endif
