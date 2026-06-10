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
struct String;

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }

struct ModMainMenu { void* _opaque; };  // no offset accesses observed
#endif
