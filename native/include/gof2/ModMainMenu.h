#ifndef GOF2_MODMAINMENU_H
#define GOF2_MODMAINMENU_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct ModMainMenu;
struct CutScene;
struct MenuTouchWindow;
struct GameRecord;
struct RecordHandler;
struct Station;

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

// Explicit layout so the named members coexist with raw byte-offset macro access.
struct ModMainMenu {
    void*    field_0x00;   // vtable
    int      field_0x04;   // paint canvas handle
    void*    f_8;          // 0x08 ApplicationManager*
    int      field_0x0c;   // state
    byte     field_0x10[4];
    int      field_0x14;
    void*    f_18;         // 0x18 MenuTouchWindow*
    void*    f_1c;         // 0x1c CutScene*
    int      field_0x20;
    int      field_0x24;
    byte     field_0x28;
    byte     field_0x29;
};
#endif
