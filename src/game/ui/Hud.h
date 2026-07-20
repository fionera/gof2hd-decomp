#ifndef GOF2_HUD_H
#define GOF2_HUD_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "ListItem.h"
#include "TouchButton.h"
#include "game/mission/Item.h"
#include "game/ship/PlayerEgo.h"

#include "game/ui/HudEventDisplay.h"

#include "game/ui/CargoBay.h"
class Radar;

class Item;
class Level;
class ListItem;
class PlayerEgo;
class TouchButton;
class Sprite;

class Hud {
public:
    unsigned char field_0x0;
    unsigned char visible;
    unsigned char field_0x2;
    unsigned char field_0x3;
    // ASM (closeHudMenu, r0=this): menuButtons null-checked at [this,#24]=0x18, ours @4. 20 bytes precede;
    // this lands field_0x1c@0x1c (matching the Ghidra-named String run, 12B apart).
    uint8_t _pad_0x4[20];
    Array<TouchButton *> *menuButtons;

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
    int eventLineX;
    int eventLineY;
    int factionLogoImage;
    uint8_t _hudrealign_0x118[184]; // realign field_0x1d0 to 0x1d0
    int field_0x1d0;
    int eventScrollTick;
    unsigned char eventScrolls;
    uint8_t _hudrealign_0x1dc[4]; // realign field_0x1e0 to 0x1e0
    String field_0x1e0;
    unsigned char letterbox;
    uint8_t _hudrealign_0x1f0[4]; // realign field_0x1f4 to 0x1f4
    String field_0x1f4;
    String field_0x200;
    unsigned char hasBoostButton;
    unsigned char hasShieldBar;
    unsigned char hasArmorRegen;
    unsigned char hasAutofireUI;
    uint8_t _hudrealign_0x210[24]; // realign field_0x228 to 0x228
    String field_0x228;
    unsigned char cargoFullFlag;
    Level *menuLevel;
    Item *currentSecondaryWeapon;
    Array<Item *> *equipmentArray;
    unsigned char shieldHitFlash; // ASM (playerHit): shieldHitFlash@0x244, after equipmentArray
    Array<ListItem *> *eventQueue;
    int eventQueueTimer;
    unsigned char eventQueueDirty;
    int eventQueuePaused;
    unsigned char jumpMapSelectedFlag;
    uint8_t _hudrealign_0x259[28]; // realign field_0x275 to 0x275
    unsigned char field_0x275;
    unsigned short field_0x276;
    unsigned short weaponSelectState;
    unsigned char field_0x27a;
    unsigned char field_0x27b;
    int fuelGaugeValue;
    unsigned char field_0x280;
    unsigned char field_0x281;
    unsigned char quickMenuOpen;
    unsigned char quickMenuEmpty;
    unsigned int touchFlags;
    int field_0x288;
    Array<void *> *keyArray; // lint: void_ptr (opaque touch-handle element type; matches exported void* touchId)
    int *elementBits;
    unsigned char autoTurretFlags; // ASM (releaseAllKeys): field_0x288@0x288, so autoTurretFlags is after elementBits
    int quickMenuTopImage;
    int quickMenuBottomImage;
    int quickMenuMiddleImage;
    int shieldFrameImage;
    int shieldFrameHitImage;
    int shieldBarBgImage;
    int shieldBarFillImage;
    int armorFrameImage;
    int armorFrameLowImage;
    int armorBarBgImage;
    int armorRegenFillImage;
    int armorBarFillImage;
    int barDividerImage;
    int pauseButtonPressedImage;
    int pauseButtonImage;
    int lockBracketImage;
    int lockBracketLockedImage;
    int orbitMarkerActiveImage;
    int orbitMarkerIdleImage;
    int autoTurretOnImage;
    int autoTurretOffImage;
    int reticleImage;
    int missionBannerImage;
    int eventBannerImage;
    int quickMenuHeaderImage;
    int fuelGaugeIconImage;
    int fuelGaugeBarImage;
    uint8_t _hudrealign_0x304[176]; // realign field_0x3b4 to 0x3b4
    String field_0x3b4;
    int secondaryLabelX;
    int field_0x3c4;
    int menuOriginY;
    int menuRowHeight;
    int field_0x3d0;
    int field_0x3d4;
    int menuBaseY;
    int field_0x3dc;
    unsigned short field_0x3e0;
    unsigned short field_0x3e2;
    unsigned short field_0x3e4;
    unsigned short field_0x3e6;
    uint8_t _hudrealign_0x3e8[4]; // realign field_0x3ec to 0x3ec
    unsigned short field_0x3ec;
    unsigned short field_0x3ee;
    uint8_t _hudrealign_0x3f0[2]; // realign field_0x3f2 to 0x3f2
    unsigned short field_0x3f2;
    unsigned short field_0x3f4;
    uint8_t _hudrealign_0x3f6[2]; // realign field_0x3f8 to 0x3f8
    unsigned short field_0x3f8;
    unsigned short field_0x3fa;
    uint8_t _hudrealign_0x3fc[2]; // realign field_0x3fe to 0x3fe
    unsigned short field_0x3fe;
    unsigned short field_0x400;
    uint8_t _hudrealign_0x402[2]; // realign field_0x404 to 0x404
    unsigned short field_0x404;
    unsigned short field_0x406;
    uint8_t _hudrealign_0x408[2]; // realign field_0x40a to 0x40a
    unsigned short field_0x40a;
    unsigned short field_0x40c;
    uint8_t _hudrealign_0x40e[2]; // realign field_0x410 to 0x410
    unsigned short field_0x410;
    unsigned short field_0x412;
    uint8_t _hudrealign_0x414[2]; // realign field_0x416 to 0x416
    unsigned short field_0x416;
    unsigned short field_0x418;
    unsigned short field_0x41a;
    unsigned short field_0x41c;
    unsigned short lockBracketX;
    unsigned short lockBracketY;
    unsigned short reticleX;
    unsigned short reticleY;
    uint8_t _hudrealign_0x426[6]; // realign field_0x42c to 0x42c
    unsigned short field_0x42c;
    uint8_t _hudrealign_0x42e[10]; // realign field_0x438 to 0x438
    unsigned short field_0x438;
    uint8_t _hudrealign_0x43a[2]; // realign field_0x43c to 0x43c
    unsigned short field_0x43c;
    unsigned short field_0x43e;
    unsigned short field_0x440;
    unsigned short field_0x442;
    unsigned short field_0x444;
    unsigned short field_0x446;
    unsigned short field_0x448;
    unsigned short field_0x44a;
    unsigned short field_0x44c;
    uint8_t _hudrealign_0x44e[6]; // realign field_0x454 to 0x454
    unsigned short field_0x454;
    unsigned short field_0x456;
    unsigned short field_0x458;
    unsigned short field_0x45a;
    uint8_t _hudrealign_0x45c[2]; // realign field_0x45e to 0x45e
    unsigned short field_0x45e;
    unsigned short field_0x460;
    uint8_t _hudrealign_0x464[4]; // realign field_0x468 to 0x468
    int field_0x468;
    int hitFlashTimer;
    int field_0x470;
    unsigned char autofireEnabled;
    unsigned char fireForTutorial;
    int timeExtenderTimer;
    int timeExtenderDuration;
    unsigned char messageActive;
    int menuOriginX;
    int menuOriginYBase;
    int touchHalfExtent;
    int touchHalfExtentSmall;
    int analogStickRadius;
    int eventLineMargin;
    int eventLineMarginAlt;
    int secondaryLabelTimerSeed;
    int secondaryLabelTimer;
    uint8_t _hudrealign_0x4a8[116]; // realign field_0x51c to 0x51c
    String field_0x51c;
    unsigned char hackingGameActive;
    int cargoAggregateCount;
    Array<unsigned int> *uintArray;
    Sprite *digitSprite;
    int multiplierIconImage;

    Hud();

    ~Hud();

    void addToEventQueue(ListItem *item);

    uint8_t cargoFull();

    void catchCargo(int itemId, int count, bool single, bool missionDelivery, bool extender, bool slotMode,
                    bool aggregate);

    Hud *checkIfQuickMenuIsEmpty();

    void clearQueue();

    void closeHudMenu();

    void draw(long long t0, long long t1, PlayerEgo *ego, bool letterbox, unsigned int x, unsigned int y);

    void drawChallengeModeScore(int unused);

    void drawCredits();

    void drawBigNumber(int x, int y, int value, bool flag);

    void drawEventQueue();

    void drawEventString(String text, bool rightAlign);

    void drawMenu(int unused);

    void drawOrbitInformation();

    void drawPauseButton();

    void enableFireForTutorial(bool value);

    unsigned int firePressed();

    float getAnalogX();

    float getAnalogY();

    void hudEvent(int eventId, PlayerEgo *ego, int arg);

    void hudEventMedal(int medalId, int percent);

    int hudAction(int action, Level *lvl, Radar *radar);

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

    void setVisible(bool value);

    unsigned int touchBegin(unsigned int a, unsigned int b, void *key); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    unsigned int touchEnd(unsigned int a, unsigned int b, void *key); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    unsigned int touchMove(unsigned int a, unsigned int b, void *key); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    unsigned int touchedElement(unsigned int x, unsigned int y);

    void updateQueue(int dt);

    void updateSecondaryWeaponString();

    void drawTitleImage(bool visible);

    static int RADAR_WIDTH;
    static int RADAR_HEIGHT;
    static int wingmanCommand;
};

static_assert(__builtin_offsetof(HudEventDisplay, eventBannerDisplayScale) == 0x1e0,
              "HudEventDisplay::eventBannerDisplayScale must live at +0x1e0");
static_assert(__builtin_offsetof(HudEventDisplay, eventBannerDisplayBase) == 0x1e4,
              "HudEventDisplay::eventBannerDisplayBase must live at +0x1e4");
static_assert(__builtin_offsetof(CargoBay, cargoCurrent) == 0x54,
              "CargoBay::cargoCurrent must live at +0x54");
static_assert(__builtin_offsetof(CargoBay, cargoMax) == 0x58,
              "CargoBay::cargoMax must live at +0x58");
#endif
