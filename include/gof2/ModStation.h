#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// ModStation — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.


struct ModStation;     // opaque; we only ever take a ModStation* and offset-cast.

// Other game types referenced by pointer only — keep opaque.
// (String comes from common.h via `using AbyssEngine::String;` — do not redeclare.)
struct ChoiceWindow;
struct RecordHandler;

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char*           B (void* p, int off) { return (char*)p + off; 
    // ---- methods (converted from free functions) ----
    void ModStation();
    void OnInitialize();
    void OnKeyPress(long long key);
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnSuspend();
    void OnTouchBegin(int x, int y, void *touch);
    void OnTouchEnd(int param_1, int param_2, void *param_3);
    void OnTouchMove(int x, int y, void *touch);
    void OnUpdate();
    void addAchievement(int param_1, int param_2);
    void autosave();
    void checkHints();
    void checkMedals();
    void checkPendingProducts();
    void dtor();
    ModStation * dtor_inner();
    void leaveStation();
    void resetIdleCamForHangar();
    void setGameLoaded();
    void showCBSMessage();
    void showDlcMenu();
    void showMapWindow();
}
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline unsigned int&   U (void* p, int off) { return *(unsigned int*)((char*)p + off); }
static inline float&          F (void* p, int off) { return *(float*)((char*)p + off); }
static inline double&         D (void* p, int off) { return *(double*)((char*)p + off); }
static inline char&           C (void* p, int off) { return *(char*)((char*)p + off); }
static inline unsigned char&  UC(void* p, int off) { return *(unsigned char*)((char*)p + off); }
static inline bool&           BL(void* p, int off) { return *(bool*)((char*)p + off); }
static inline short&          S (void* p, int off) { return *(short*)((char*)p + off); }
static inline unsigned short& US(void* p, int off) { return *(unsigned short*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }

struct ModStation { void* _opaque; };  // no offset accesses observed
#endif
