#ifndef WORK_CLASSES_SPACELOUNGE_SRC_CLASS_H
#define WORK_CLASSES_SPACELOUNGE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct SpaceLounge {
    // @portable-fields
    unsigned char _pad_0[4];
    void* f_4; // 0x4
    void* f_8; // 0x8
    void* f_c; // 0xc
    unsigned char _pad_10[20];
    void* f_24; // 0x24
    void* f_28; // 0x28
    void* f_2c; // 0x2c
    unsigned char _pad_30[8];
    void* f_38; // 0x38
    void* f_3c; // 0x3c
    void* f_40; // 0x40
    void* f_44; // 0x44
    void* f_48; // 0x48
    unsigned char _pad_4c[12];
    void* f_58; // 0x58
    void* f_5c; // 0x5c
    void* f_60; // 0x60
    unsigned char _pad_64[92];
    void* f_c0; // 0xc0
};
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
