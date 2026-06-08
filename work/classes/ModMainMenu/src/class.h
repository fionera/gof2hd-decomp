#ifndef WORK_CLASSES_MODMAINMENU_SRC_CLASS_H
#define WORK_CLASSES_MODMAINMENU_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct ModMainMenu {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    unsigned char _pad_c[12];
    void* f_18; // 0x18
    void* f_1c; // 0x1c
};
struct CutScene;
struct MenuTouchWindow;
struct GameRecord;
struct RecordHandler;
struct Station;
struct String;

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

#endif
