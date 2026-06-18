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
class PlayerEgo;
class Level;
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
    int eventLineX;          // +0x160 event-line X origin
    int eventLineY;          // +0x164 event-line Y origin
    int factionLogoImage;    // +0x1c4 faction-badge image id
    int field_0x1d0;
    int eventScrollTick;     // +0x1d8 medal/event scroll timer
    unsigned char eventScrolls;  // +0x1de event line scrolls
    String field_0x1e0;  // current HUD-event line
    unsigned char letterbox;  // cinematic letterbox flag
    String field_0x1f4;  // cargo/event scratch line
    String field_0x200;
    unsigned char hasBoostButton;   // +0x21e boost-button present
    unsigned char hasShieldBar;     // +0x21f shield bar present
    unsigned char hasArmorRegen;    // +0x220 armor-regen overlay present
    unsigned char hasAutofireUI;    // +0x221 autofire UI present
    String field_0x228;
    unsigned char cargoFullFlag;
    void *menuLevel;  // active Level* / menu type
    unsigned char shieldHitFlash;   // +0x244 shield-hit flash
    Item *currentSecondaryWeapon;
    Array<Item *> *equipmentArray;  // secondary-weapon equipment
    Array<ListItem *> *eventQueue;  // event-banner queue
    int eventQueueTimer;
    unsigned char eventQueueDirty;
    int eventQueuePaused;
    unsigned char jumpMapSelectedFlag;
    int field_0x278;
    unsigned char field_0x27a;
    int fuelGaugeValue;      // +0x27c gauge numeric value
    unsigned char field_0x280;
    unsigned char field_0x281;
    unsigned char quickMenuOpen;
    unsigned char quickMenuEmpty;
    unsigned int touchFlags;
    unsigned char autoTurretFlags;  // +0x287 auto-turret manual-toggle bits
    int field_0x288;
    Array<void *> *keyArray;  // per-touch key ids (0x19) — touch ids are opaque void* handles
    int *elementBits;  // per-touch action bits (0x19)
    int quickMenuTopImage;       // +0x298 radial menu top cap
    int quickMenuBottomImage;    // +0x29c radial menu bottom cap
    int quickMenuMiddleImage;    // +0x2a0 radial menu middle slice
    int shieldFrameImage;        // +0x2a4 shield bar frame
    int shieldFrameHitImage;     // +0x2a8 shield bar frame (hit)
    int shieldBarBgImage;        // +0x2ac shield bar background
    int shieldBarFillImage;      // +0x2b0 shield bar fill
    int armorFrameImage;         // +0x2b4 armor bar frame
    int armorFrameLowImage;      // +0x2b8 armor bar frame (low)
    int armorBarBgImage;         // +0x2bc armor bar background
    int armorRegenFillImage;     // +0x2c0 armor regen overlay fill
    int armorBarFillImage;       // +0x2c4 armor bar fill
    int barDividerImage;         // +0x2d4 status-bar divider
    int pauseButtonPressedImage; // +0x2f4 pause button (pressed)
    int pauseButtonImage;        // +0x2f8 pause button (idle)
    int lockBracketImage;        // +0x304 lock bracket
    int lockBracketLockedImage;  // +0x308 lock bracket (locked)
    int orbitMarkerActiveImage;  // +0x30c orbit marker (active)
    int orbitMarkerIdleImage;    // +0x310 orbit marker (idle)
    int autoTurretOnImage;       // +0x314 auto-turret icon (on)
    int autoTurretOffImage;      // +0x318 auto-turret icon (off)
    int reticleImage;            // +0x31c aiming reticle
    int missionBannerImage;      // +0x324 mission/cargo banner bg
    int eventBannerImage;        // +0x354 event-banner bg
    int quickMenuHeaderImage;    // +0x35c radial menu header glyph
    int fuelGaugeIconImage;      // +0x370 fuel gauge icon
    int fuelGaugeBarImage;       // +0x374 fuel gauge bar
    String field_0x3b4;  // secondary-weapon label
    int secondaryLabelX;     // +0x3c0 secondary-weapon label X (centered)
    int field_0x3c4;
    int menuOriginY;         // +0x3c8 radial menu Y origin
    int menuRowHeight;       // +0x3cc radial menu row height
    int field_0x3d0;
    int field_0x3d4;
    int menuBaseY;           // +0x3d8 radial menu base Y
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
    int hitFlashTimer;       // +0x46c shield/armor hit flash timer
    int field_0x470;
    unsigned char autofireEnabled;
    unsigned char fireForTutorial;
    int timeExtenderTimer;   // +0x4bc time-extender countdown
    int timeExtenderDuration;// +0x4c0 time-extender total duration
    unsigned char messageActive;
    int menuOriginX;         // +0x4cc radial menu X origin
    int menuOriginYBase;     // +0x4d0 radial menu Y base origin
    int touchHalfExtent;     // +0x4d8 default touch-rect half extent
    int touchHalfExtentSmall;// +0x4dc smaller touch-rect half extent
    int analogStickRadius;   // +0x4e0 analog-stick max radius
    int eventLineMargin;     // +0x4e8 event-line inner margin
    int eventLineMarginAlt;  // +0x4f0 event-line letterbox margin
    int secondaryLabelTimerSeed; // +0x514 secondary-weapon label seed (-1)
    int secondaryLabelTimer; // +0x518 secondary-weapon label timer
    String field_0x51c;  // full-screen HUD message
    unsigned char hackingGameActive;
    int cargoAggregateCount; // +0x52c aggregated cargo "+N" count
    Array<unsigned int> *uintArray;
    void *digitSprite;  // challenge-mode digit sprite
    int multiplierIconImage; // +0x538 challenge-mode multiplier icon

    Hud();
    ~Hud();

    void addToEventQueue(ListItem *item);
    uint8_t cargoFull();
    void catchCargo(int amount, int cargoVal, bool a, bool docked, bool mission, bool p6, bool p7, int aggregateKey);
    Hud * checkIfQuickMenuIsEmpty();
    void clearQueue();
    void closeHudMenu();
    void draw(long long t0, long long t1, PlayerEgo *ego, bool letterbox, unsigned int x, unsigned int y);
    void drawChallengeModeScore();
    void drawEventQueue();
    void drawEventString(String text, bool rightAlign);
    void drawMenu();
    void drawOrbitInformation();
    void drawPauseButton();
    void enableFireForTutorial(bool value);
    unsigned int firePressed();
    float getAnalogX();
    float getAnalogY();
    void hudEvent(int eventId, PlayerEgo *ego, int arg);
    void hudEventMedal(int medalId, int percent);
    int init();
    void initHudMenu(int menuType, Level *lvl);
    uint8_t isHackingGameActive();
    uint8_t jumpMapSelected();
    void playerHit();
    void releaseAllKeys();
    void resetAnalogStick();
    unsigned int sameHudEventAsBefore(String str);
    int sameHudEventAsBeforeAggregate(String str);
    void setAutofireEnabled(bool value);
    void setCurrentSecondaryWeapon(Item *item);
    void setHackingGameActive(bool value);
    void setJumpMapSelected(bool value);
    void setTimeExtender(bool p1, bool p2, bool p3, bool p4);
    unsigned int touchBegin(unsigned int a, unsigned int b, void *key);
    unsigned int touchEnd(unsigned int a, unsigned int b, void *key);
    unsigned int touchMove(unsigned int a, unsigned int b, void *key);
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
