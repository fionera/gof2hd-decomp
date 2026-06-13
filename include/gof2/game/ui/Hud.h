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
// I() is also defined identically in Explosion.h; only define it here when that
// header (which has the canonical definition for TUs that include both) is absent.
#ifndef GOF2_EXPLOSION_H
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
#endif
#ifndef GOF2_LEVELSCRIPT_H
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }
#endif

class Hud {
public:
    void* _opaque;  // no offset accesses observed

    // ---- methods (converted from free functions) ----
    Hud * ctor();
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

    // ---- methods recovered from inlined fragments / tail thunks ----
    // The decompiler emitted these as standalone Hud_* helpers, but they are
    // really pieces of draw()/init()/hudEvent()/catchCargo()/initHudMenu()/
    // touchMove()/checkIfQuickMenuIsEmpty() (inlined or tail-called).
    void drawReticleAndBrackets(void *ego, unsigned int x, unsigned int y);
    void drawRadar();
    void drawBars(void *ego);
    void drawSecondaryWeaponPanel();
    void drawMissionBanner();
    void drawMessage();
    unsigned int touchMoveFallback(unsigned int a, void *b);
    void secondaryWeaponChanged();
    void refreshQuickMenu();
    void catchCargoFinish(ListItem *item);
    void eventQueueFinish(void *canvas, unsigned int color);
    void loadImages();
    void hudEventBuild(int eventId, void *ego, int arg);
    void buildQuickMenu(int menuType);
    void subObjectDtor(void *p);

    // drawTitleImage(visible): in this build the title-image draw was compiled down
    // to an identity stub that just yields back its `visible` argument.
    bool drawTitleImage(bool visible);
    // enterCinematic(arg, flag): forward into the engine cinematic-mode entry point
    // (reached through a GOT/veneer slot in the binary) to toggle the HUD overlay.
    void enterCinematic(int arg, int flag);
};
#endif
