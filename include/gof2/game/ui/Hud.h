#ifndef GOF2_HUD_H
#define GOF2_HUD_H
#include "gof2/common.h"
// Galaxy on Fire 2 — Hud class (top-level, no namespace).
// Full named layout recovered from the per-method byte-offset accesses. Trailing
// comments are the original 32-bit field offsets, kept for cross-reference; the
// live layout is a natural 64-bit struct. Fields whose semantics are not evident
// from the binary keep the deterministic field_0xNN name (real members).

// Other game types referenced by pointer only — keep opaque.
struct Item;
struct ListItem;
struct TouchButton;
// String comes from common.h (AbyssEngine::String, aliased into global scope).

// ---- tiny offset-cast helpers for EXTERNAL opaque objects --------------------
// Hud's own fields are named members; these only address other objects (ListItem,
// the event-queue display holder) whose layouts are not modelled here. I() is also
// defined identically in Explosion.h; only define it when that header is absent.
#ifndef GOF2_EXPLOSION_H
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
#endif
#ifndef GOF2_LEVELSCRIPT_H
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }
#endif

class Hud {
public:
    unsigned char field_0x0;                     // +0x00
    Array<TouchButton *> *menuButtons;           // +0x18  radial quick-menu buttons
    // +0x1c..+0x108: block of 20 engine String members (message/label scratch)
    String field_0x1c;                           // +0x1c
    String field_0x28;                           // +0x28
    String field_0x34;                           // +0x34
    String field_0x40;                           // +0x40
    String field_0x4c;                           // +0x4c
    String field_0x58;                           // +0x58
    String field_0x64;                           // +0x64
    String field_0x70;                           // +0x70
    String field_0x7c;                           // +0x7c
    String field_0x88;                           // +0x88
    String field_0x94;                           // +0x94
    String field_0xa0;                           // +0xa0
    String field_0xac;                           // +0xac
    String field_0xb8;                           // +0xb8
    String field_0xc4;                           // +0xc4
    String field_0xd0;                           // +0xd0
    String field_0xdc;                           // +0xdc
    String field_0xe8;                           // +0xe8
    String field_0xf4;                           // +0xf4
    String field_0x100;                          // +0x100
    int field_0x160;                             // +0x160
    int field_0x164;                             // +0x164
    int field_0x1c4;                             // +0x1c4  faction-badge image id
    int field_0x1d0;                             // +0x1d0
    int field_0x1d8;                             // +0x1d8
    unsigned char field_0x1de;                   // +0x1de
    String field_0x1e0;                          // +0x1e0  current HUD-event line
    unsigned char letterbox;                     // +0x1ec  cinematic letterbox flag
    String field_0x1f4;                          // +0x1f4  cargo/event scratch line
    String field_0x200;                          // +0x200
    unsigned char field_0x21e;                   // +0x21e  boost-button present
    unsigned char field_0x21f;                   // +0x21f  shield bar present
    unsigned char field_0x220;                   // +0x220  armor-regen overlay present
    unsigned char field_0x221;                   // +0x221  autofire UI present
    String field_0x228;                          // +0x228
    unsigned char cargoFullFlag;                 // +0x235
    void *menuLevel;                             // +0x238  active Level* / menu type
    unsigned char field_0x244;                   // +0x244  shield-hit flash
    Item *currentSecondaryWeapon;                // +0x258
    Array<Item *> *equipmentArray;               // +0x25c  secondary-weapon equipment
    Array<ListItem *> *eventQueue;               // +0x264  event-banner queue
    int eventQueueTimer;                         // +0x268
    unsigned char eventQueueDirty;               // +0x26c
    int eventQueuePaused;                         // +0x270
    unsigned char jumpMapSelectedFlag;           // +0x274
    int field_0x278;                             // +0x278
    unsigned char field_0x27a;                   // +0x27a
    int field_0x27c;                             // +0x27c
    unsigned char field_0x280;                   // +0x280
    unsigned char field_0x281;                   // +0x281
    unsigned char quickMenuOpen;                 // +0x282
    unsigned char quickMenuEmpty;                // +0x283
    unsigned int touchFlags;                     // +0x284
    unsigned char field_0x287;                   // +0x287
    int field_0x288;                             // +0x288
    Array<int> *keyArray;                        // +0x28c  per-touch key ids (0x19)
    int *elementBits;                            // +0x290  per-touch action bits (0x19)
    int field_0x298;                             // +0x298
    int field_0x29c;                             // +0x29c
    int field_0x2a0;                             // +0x2a0
    int field_0x2a4;                             // +0x2a4
    int field_0x2a8;                             // +0x2a8
    int field_0x2ac;                             // +0x2ac
    int field_0x2b0;                             // +0x2b0
    int field_0x2b4;                             // +0x2b4
    int field_0x2b8;                             // +0x2b8
    int field_0x2bc;                             // +0x2bc
    int field_0x2c0;                             // +0x2c0
    int field_0x2c4;                             // +0x2c4
    int field_0x2d4;                             // +0x2d4
    int field_0x2f4;                             // +0x2f4
    int field_0x2f8;                             // +0x2f8
    int field_0x304;                             // +0x304
    int field_0x308;                             // +0x308
    int field_0x30c;                             // +0x30c
    int field_0x310;                             // +0x310
    int field_0x314;                             // +0x314
    int field_0x318;                             // +0x318
    int field_0x31c;                             // +0x31c
    int field_0x324;                             // +0x324
    int field_0x354;                             // +0x354
    int field_0x35c;                             // +0x35c
    int field_0x370;                             // +0x370
    int field_0x374;                             // +0x374
    String field_0x3b4;                          // +0x3b4  secondary-weapon label
    int field_0x3c0;                             // +0x3c0
    int field_0x3c4;                             // +0x3c4
    int field_0x3c8;                             // +0x3c8
    int field_0x3cc;                             // +0x3cc
    int field_0x3d0;                             // +0x3d0
    int field_0x3d4;                             // +0x3d4
    int field_0x3d8;                             // +0x3d8
    int field_0x3dc;                             // +0x3dc
    unsigned short field_0x3e0;                  // +0x3e0
    unsigned short field_0x3e2;                  // +0x3e2
    unsigned short field_0x3e4;                  // +0x3e4
    unsigned short field_0x3e6;                  // +0x3e6
    unsigned short field_0x3ec;                  // +0x3ec
    unsigned short field_0x3ee;                  // +0x3ee
    unsigned short field_0x3f2;                  // +0x3f2
    unsigned short field_0x3f4;                  // +0x3f4
    unsigned short field_0x3f8;                  // +0x3f8
    unsigned short field_0x3fa;                  // +0x3fa
    unsigned short field_0x3fe;                  // +0x3fe
    unsigned short field_0x400;                  // +0x400
    unsigned short field_0x404;                  // +0x404
    unsigned short field_0x406;                  // +0x406
    unsigned short field_0x40a;                  // +0x40a
    unsigned short field_0x40c;                  // +0x40c
    unsigned short field_0x410;                  // +0x410
    unsigned short field_0x412;                  // +0x412
    unsigned short field_0x416;                  // +0x416
    unsigned short field_0x418;                  // +0x418
    unsigned short field_0x41a;                  // +0x41a
    unsigned short field_0x41c;                  // +0x41c
    unsigned short lockBracketX;                 // +0x41e
    unsigned short lockBracketY;                 // +0x420
    unsigned short reticleX;                     // +0x424
    unsigned short reticleY;                     // +0x426
    unsigned short field_0x42c;                  // +0x42c
    unsigned short field_0x438;                  // +0x438
    unsigned short field_0x43c;                  // +0x43c
    unsigned short field_0x43e;                  // +0x43e
    unsigned short field_0x440;                  // +0x440
    unsigned short field_0x442;                  // +0x442
    unsigned short field_0x444;                  // +0x444
    unsigned short field_0x446;                  // +0x446
    unsigned short field_0x448;                  // +0x448
    unsigned short field_0x44a;                  // +0x44a
    unsigned short field_0x44c;                  // +0x44c
    unsigned short field_0x454;                  // +0x454
    unsigned short field_0x456;                  // +0x456
    unsigned short field_0x458;                  // +0x458
    unsigned short field_0x45a;                  // +0x45a
    unsigned short field_0x45e;                  // +0x45e
    unsigned short field_0x460;                  // +0x460
    int field_0x468;                             // +0x468
    int field_0x46c;                             // +0x46c
    int field_0x470;                             // +0x470
    unsigned char autofireEnabled;               // +0x4a0
    unsigned char fireForTutorial;               // +0x4a5
    int field_0x4bc;                             // +0x4bc
    int field_0x4c0;                             // +0x4c0
    unsigned char messageActive;                 // +0x4c8
    int field_0x4cc;                             // +0x4cc
    int field_0x4d0;                             // +0x4d0
    int field_0x4d8;                             // +0x4d8
    int field_0x4dc;                             // +0x4dc
    int field_0x4e0;                             // +0x4e0
    int field_0x4e8;                             // +0x4e8
    int field_0x4f0;                             // +0x4f0
    int field_0x514;                             // +0x514
    int field_0x518;                             // +0x518
    String field_0x51c;                          // +0x51c  full-screen HUD message
    unsigned char hackingGameActive;             // +0x528
    int field_0x52c;                             // +0x52c
    Array<unsigned int> *uintArray;              // +0x530
    void *digitSprite;                           // +0x534  challenge-mode digit sprite
    int field_0x538;                             // +0x538

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
