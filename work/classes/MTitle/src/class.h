// MTitle — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.
#ifndef WORK_CLASSES_MTITLE_SRC_CLASS_H
#define WORK_CLASSES_MTITLE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct MTitle {
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8

    MTitle();
    ~MTitle();

    int OnInitialize();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnTouchEnd(int x, int y);
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
