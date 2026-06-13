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

// NOTE: the B/I/P offset-cast helpers are also defined (unconditionally) by other
// class headers such as gof2/Galaxy.h. They are only declared here when no other
// header has already provided them (the including TU may #define GOF2_BIP_HELPERS
// after including such a header to avoid a redefinition).
#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
#endif

// Explicit layout so the named members coexist with raw byte-offset macro access.
class ModMainMenu {
public:
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

    // Tail-call fragments peeled out of the OnXxx() handlers: the decompiler split each
    // handler's basic block at its final tail-branch. Each helper completes the operation
    // by handing off to the engine/runtime object the original code branched into; they
    // are static because (apart from the deleting-destructor tail) they act on a foreign
    // object rather than on the menu itself.
    static void releaseTail(void *sound);                    // OnRelease(): free all FMOD events
    static void resumeTail(int sound, int channel, int arg); // OnResume(): restore BG-music volume
    static void renderBackdropEnd(void *canvas);             // OnRender3D(): end the 3D pass
    static void deleteTail(ModMainMenu *self);               // deleting dtor: release storage
    static void suspendTail(int recordHandler);              // OnSuspend(): persist options
    static void touchEndTail(void *starSystem);              // OnTouchEnd(): (re)init star lighting
    static void renderOverlayEnd(int canvas);                // OnRender2D(): end the 2D pass
};
#endif
