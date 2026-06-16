#ifndef GOF2_HUD_H
#define GOF2_HUD_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
// Galaxy on Fire 2 -- Hud: the in-game heads-up display. Owns the radial quick-menu
// buttons, the event-banner queue, the per-touch key state, and the full set of HUD
// image atlases and layout coordinates.

// Other game types referenced by pointer only.
class Item;
class ListItem;
class TouchButton;
// String comes from common.h (AbyssEngine::String, aliased into global scope).

class Hud {
public:
    unsigned char field_0x0;
    Array<TouchButton *> *menuButtons;  // radial quick-menu buttons
    // Scratch block of message/label String members.
    String field_0x1c;
    String field_0x28;
    String field_0x34;
    String field_0x40;
    String field_0x4c;
    String field_0x58;
    String field_0x64;
    String field_0x70;
    String field_0x7c;
    String field_0x88;
    String field_0x94;
    String field_0xa0;
    String field_0xac;
    String field_0xb8;
    String field_0xc4;
    String field_0xd0;
    String field_0xdc;
    String field_0xe8;
    String field_0xf4;
    String field_0x100;
    int field_0x160;
    int field_0x164;
    int field_0x1c4;  // faction-badge image id
    int field_0x1d0;
    int field_0x1d8;
    unsigned char field_0x1de;
    String field_0x1e0;  // current HUD-event line
    unsigned char letterbox;  // cinematic letterbox flag
    String field_0x1f4;  // cargo/event scratch line
    String field_0x200;
    unsigned char field_0x21e;  // boost-button present
    unsigned char field_0x21f;  // shield bar present
    unsigned char field_0x220;  // armor-regen overlay present
    unsigned char field_0x221;  // autofire UI present
    String field_0x228;
    unsigned char cargoFullFlag;
    void *menuLevel;  // active Level* / menu type
    unsigned char field_0x244;  // shield-hit flash
    Item *currentSecondaryWeapon;
    Array<Item *> *equipmentArray;  // secondary-weapon equipment
    Array<ListItem *> *eventQueue;  // event-banner queue
    int eventQueueTimer;
    unsigned char eventQueueDirty;
    int eventQueuePaused;
    unsigned char jumpMapSelectedFlag;
    int field_0x278;
    unsigned char field_0x27a;
    int field_0x27c;
    unsigned char field_0x280;
    unsigned char field_0x281;
    unsigned char quickMenuOpen;
    unsigned char quickMenuEmpty;
    unsigned int touchFlags;
    unsigned char field_0x287;
    int field_0x288;
    Array<int> *keyArray;  // per-touch key ids (0x19)
    int *elementBits;  // per-touch action bits (0x19)
    int field_0x298;
    int field_0x29c;
    int field_0x2a0;
    int field_0x2a4;
    int field_0x2a8;
    int field_0x2ac;
    int field_0x2b0;
    int field_0x2b4;
    int field_0x2b8;
    int field_0x2bc;
    int field_0x2c0;
    int field_0x2c4;
    int field_0x2d4;
    int field_0x2f4;
    int field_0x2f8;
    int field_0x304;
    int field_0x308;
    int field_0x30c;
    int field_0x310;
    int field_0x314;
    int field_0x318;
    int field_0x31c;
    int field_0x324;
    int field_0x354;
    int field_0x35c;
    int field_0x370;
    int field_0x374;
    String field_0x3b4;  // secondary-weapon label
    int field_0x3c0;
    int field_0x3c4;
    int field_0x3c8;
    int field_0x3cc;
    int field_0x3d0;
    int field_0x3d4;
    int field_0x3d8;
    int field_0x3dc;
    unsigned short field_0x3e0;
    unsigned short field_0x3e2;
    unsigned short field_0x3e4;
    unsigned short field_0x3e6;
    unsigned short field_0x3ec;
    unsigned short field_0x3ee;
    unsigned short field_0x3f2;
    unsigned short field_0x3f4;
    unsigned short field_0x3f8;
    unsigned short field_0x3fa;
    unsigned short field_0x3fe;
    unsigned short field_0x400;
    unsigned short field_0x404;
    unsigned short field_0x406;
    unsigned short field_0x40a;
    unsigned short field_0x40c;
    unsigned short field_0x410;
    unsigned short field_0x412;
    unsigned short field_0x416;
    unsigned short field_0x418;
    unsigned short field_0x41a;
    unsigned short field_0x41c;
    unsigned short lockBracketX;
    unsigned short lockBracketY;
    unsigned short reticleX;
    unsigned short reticleY;
    unsigned short field_0x42c;
    unsigned short field_0x438;
    unsigned short field_0x43c;
    unsigned short field_0x43e;
    unsigned short field_0x440;
    unsigned short field_0x442;
    unsigned short field_0x444;
    unsigned short field_0x446;
    unsigned short field_0x448;
    unsigned short field_0x44a;
    unsigned short field_0x44c;
    unsigned short field_0x454;
    unsigned short field_0x456;
    unsigned short field_0x458;
    unsigned short field_0x45a;
    unsigned short field_0x45e;
    unsigned short field_0x460;
    int field_0x468;
    int field_0x46c;
    int field_0x470;
    unsigned char autofireEnabled;
    unsigned char fireForTutorial;
    int field_0x4bc;
    int field_0x4c0;
    unsigned char messageActive;
    int field_0x4cc;
    int field_0x4d0;
    int field_0x4d8;
    int field_0x4dc;
    int field_0x4e0;
    int field_0x4e8;
    int field_0x4f0;
    int field_0x514;
    int field_0x518;
    String field_0x51c;  // full-screen HUD message
    unsigned char hackingGameActive;
    int field_0x52c;
    Array<unsigned int> *uintArray;
    void *digitSprite;  // challenge-mode digit sprite
    int field_0x538;

    Hud();
    ~Hud();

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

    // Per-panel renderers and helpers driven by the methods above.
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

    bool drawTitleImage(bool visible);
    // Toggle the HUD's cinematic overlay.
    void enterCinematic(int arg, int flag);
};
#endif
