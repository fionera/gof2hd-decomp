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

#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
#endif

// Explicit layout so the named members coexist with raw byte-offset macro access.
struct ModMainMenu {
    void*    field_0x00;   // vtable
    int      paintCanvas;   // paint canvas handle
    void*    f_8;          // 0x08 ApplicationManager*
    int      state;   // state
    byte     field_0x10[4];
    int      frameTime;
    void*    f_18;         // 0x18 MenuTouchWindow*
    void*    f_1c;         // 0x1c CutScene*
    int      logoImage;
    int      fadeTimer;
    byte     logoActive;
    byte     hasSavedGame;
};
#endif
