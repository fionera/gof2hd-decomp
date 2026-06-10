#ifndef GOF2_HUD_H
#define GOF2_HUD_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Hud — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.


struct Hud;            // opaque; we only ever take a Hud* and offset-cast.

// Other game types referenced by pointer only — keep opaque.
struct Item;
struct ListItem;
struct TouchButton;
// String comes from common.h (AbyssEngine::String, aliased into global scope).

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char*           B (void* p, int off) { return (char*)p + off; 
    // ---- methods (converted from free functions) ----
    Hud * Hud();
    void addToEventQueue(ListItem *item);
    uint8_t cargoFull();
    void catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6, bool p7, int aggregateKey);
    Hud * checkIfQuickMenuIsEmpty();
    void clearQueue();
    void closeHudMenu();
    void draw(long long t0, long long t1, void *ego, bool letterbox, unsigned int x, unsigned int y);
    void drawChallengeModeScore();
    void drawEventQueue();
    void drawEventString(void *text, int rightAlign);
    void drawMenu();
    void drawOrbitInformation();
    void drawPauseButton();
    Hud * dtor();
    void enableFireForTutorial(bool value);
    unsigned int firePressed();
    float getAnalogX();
    float getAnalogY();
    void hudEvent(int eventId, void *ego, int arg);
    void hudEventMedal(int medalId, int percent);
    int init();
    void initHudMenu(int menuType, void *lvl);
    uint8_t isHackingGameActive();
    uint8_t jumpMapSelected();
    void playerHit();
    void releaseAllKeys();
    void resetAnalogStick();
    unsigned int sameHudEventAsBefore(String *str);
    int sameHudEventAsBeforeAggregate(String *str);
    void setAutofireEnabled(bool value);
    void setCurrentSecondaryWeapon(Item *item);
    void setHackingGameActive(bool value);
    void setJumpMapSelected(bool value);
    void setTimeExtender(bool p1, bool p2, bool p3, bool p4);
    unsigned int touchBegin(unsigned int a, void *b, int key);
    unsigned int touchEnd(unsigned int a, void *b, int key);
    unsigned int touchMove(unsigned int a, void *b, int key);
    unsigned int touchedElement(unsigned int x, unsigned int y);
    void updateQueue(int dt);
    void updateSecondaryWeaponString();
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

struct Hud { void* _opaque; };  // no offset accesses observed
#endif
