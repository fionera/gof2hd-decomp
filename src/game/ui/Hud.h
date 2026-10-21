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
    bool field_0x0;
    unsigned char visible;
    unsigned char field_0x2;
    unsigned char field_0x3;
    unsigned int image_0x4;
    unsigned int image_0x8;
    unsigned short field_0xc;
    unsigned short field_0xe;
    int steerOption;
    int fireOption;
    Array<TouchButton *> * menuButtons;
    String eventLines[20];
    int eventLineX;
    int eventLineY;
    uint8_t _pad_0x114[72];
    int field_0x15c;
    int field_0x160;
    int field_0x164;
    uint8_t _pad_0x168[92];
    unsigned int factionLogoImage;
    uint8_t _pad_0x1c8[8];
    int field_0x1d0;
    int eventScrollTick;
    int eventScrolls;
    uint8_t field_0x1dc[4];
    String field_0x1e0;
    unsigned char letterbox;
    uint8_t _pad_0x1ed[3];
    int field_0x1f0;
    String field_0x1f4;
    String field_0x200;
    uint8_t _pad_0x20c[12];
    int field_0x218;
    uint8_t _pad_0x21c[1];
    unsigned char hasCloak;
    bool hasBoostButton;
    unsigned char hasShieldBar;
    unsigned char hasArmorRegen;
    unsigned char hasAutofireUI;
    uint8_t _pad_0x222[6];
    String field_0x228;
    unsigned char field_0x234;
    bool cargoFullFlag;
    unsigned char field_0x236;
    uint8_t _pad_0x237[1];
    int menuType;
    uint8_t _pad_0x23c[8];
    unsigned char shieldHitFlash;
    uint8_t _pad_0x245[19];
    Item * currentSecondaryWeapon;
    Array<Item *> * equipmentArray;
    int field_0x260;
    Array<ListItem *> * eventQueue;
    int eventQueueTimer;
    unsigned char eventQueueDirty;
    uint8_t _pad_0x26d[3];
    int eventQueuePaused;
    bool jumpMapSelectedFlag;
    unsigned char field_0x275;
    unsigned char field_0x276;
    unsigned char field_0x277;
    unsigned char weaponSelectState;
    unsigned char field_0x279;
    unsigned char field_0x27a;
    unsigned char field_0x27b;
    int fuelGaugeValue;
    unsigned char field_0x280;
    unsigned char field_0x281;
    bool quickMenuOpen;
    bool quickMenuEmpty;
    unsigned int touchFlags;
    int field_0x288;
    Array<void *> * keyArray; // lint: void_ptr (opaque touch-handle element type; matches exported void* touchId);
    int * elementBits;
    unsigned char autoTurretFlags;
    uint8_t _pad_0x295[3];
    unsigned int quickMenuTopImage;
    unsigned int quickMenuBottomImage;
    unsigned int quickMenuMiddleImage;
    unsigned int shieldFrameImage;
    unsigned int shieldFrameHitImage;
    unsigned int shieldBarBgImage;
    unsigned int shieldBarFillImage;
    unsigned int armorFrameImage;
    unsigned int armorFrameLowImage;
    unsigned int armorBarBgImage;
    unsigned int armorRegenFillImage;
    unsigned int armorBarFillImage;
    unsigned int barDividerImage;
    unsigned int image_0x2cc;
    unsigned int image_0x2d0;
    unsigned int lockBracketImage;
    unsigned int lockBracketLockedImage;
    unsigned int orbitMarkerActiveImage;
    unsigned int orbitMarkerIdleImage;
    unsigned int autoTurretOnImage;
    unsigned int autoTurretOffImage;
    unsigned int reticleImage;
    unsigned int missionBannerImage;
    unsigned int pauseButtonPressedImage;
    unsigned int pauseButtonImage;
    unsigned int fuelGaugeIconImage;
    unsigned int fuelGaugeBarImage;
    unsigned int image_0x304;
    unsigned int image_0x308;
    unsigned int image_0x30c;
    unsigned int image_0x310;
    unsigned int image_0x314;
    unsigned int image_0x318;
    unsigned int image_0x31c;
    unsigned int image_0x320;
    unsigned int image_0x324;
    unsigned int image_0x328;
    unsigned int image_0x32c;
    unsigned int image_0x330;
    unsigned int image_0x334;
    unsigned int image_0x338;
    unsigned int image_0x33c;
    unsigned int image_0x340;
    unsigned int image_0x344;
    unsigned int image_0x348;
    unsigned int image_0x34c;
    unsigned int image_0x350;
    unsigned int image_0x354;
    unsigned int image_0x358;
    unsigned int image_0x35c;
    unsigned int image_0x360;
    unsigned int image_0x364;
    unsigned int image_0x368;
    unsigned int image_0x36c;
    unsigned int image_0x370;
    unsigned int image_0x374;
    unsigned int image_0x378;
    unsigned int image_0x37c;
    unsigned int image_0x380;
    unsigned int image_0x384;
    unsigned int image_0x388;
    unsigned int image_0x38c;
    unsigned int image_0x390;
    unsigned int image_0x394;
    unsigned int image_0x398;
    unsigned int image_0x39c;
    unsigned int image_0x3a0;
    unsigned int image_0x3a4;
    unsigned int image_0x3a8;
    unsigned int image_0x3ac;
    unsigned int image_0x3b0;
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
    unsigned short field_0x3e8;
    unsigned short field_0x3ea;
    unsigned short field_0x3ec;
    unsigned short field_0x3ee;
    unsigned short field_0x3f0;
    unsigned short field_0x3f2;
    unsigned short field_0x3f4;
    unsigned short field_0x3f6;
    unsigned short field_0x3f8;
    unsigned short field_0x3fa;
    unsigned short field_0x3fc;
    unsigned short field_0x3fe;
    unsigned short field_0x400;
    unsigned short field_0x402;
    unsigned short field_0x404;
    unsigned short field_0x406;
    unsigned short field_0x408;
    unsigned short field_0x40a;
    unsigned short field_0x40c;
    unsigned short field_0x40e;
    unsigned short field_0x410;
    unsigned short field_0x412;
    unsigned short field_0x414;
    unsigned short field_0x416;
    unsigned short field_0x418;
    unsigned short field_0x41a;
    unsigned short field_0x41c;
    unsigned short lockBracketX;
    unsigned short lockBracketY;
    unsigned short field_0x422;
    unsigned short reticleX;
    unsigned short reticleY;
    unsigned short field_0x428;
    unsigned short field_0x42a;
    unsigned short field_0x42c;
    unsigned short field_0x42e;
    unsigned short field_0x430;
    unsigned short field_0x432;
    unsigned short field_0x434;
    unsigned short field_0x436;
    unsigned short field_0x438;
    unsigned short field_0x43a;
    unsigned short field_0x43c;
    unsigned short field_0x43e;
    unsigned short field_0x440;
    unsigned short field_0x442;
    unsigned short field_0x444;
    unsigned short field_0x446;
    unsigned short field_0x448;
    unsigned short field_0x44a;
    unsigned short field_0x44c;
    unsigned short field_0x44e;
    int field_0x450;
    unsigned short field_0x454;
    unsigned short field_0x456;
    unsigned short field_0x458;
    unsigned short field_0x45a;
    unsigned short field_0x45c;
    unsigned short field_0x45e;
    unsigned short field_0x460;
    unsigned short field_0x462;
    int field_0x464;
    int field_0x468;
    int field_0x46c;
    int field_0x470;
    unsigned char field_0x474;
    unsigned char field_0x475;
    unsigned char field_0x476;
    unsigned char field_0x477;
    int field_0x478;
    int field_0x47c;
    unsigned char messageActive;
    uint8_t _pad_0x481[3];
    int field_0x484;
    int field_0x488;
    int field_0x48c;
    int field_0x490;
    int field_0x494;
    int field_0x498;
    int field_0x49c;
    unsigned char autofireEnabled;
    unsigned char field_0x4a1;
    unsigned char field_0x4a2;
    unsigned char field_0x4a3;
    unsigned char field_0x4a4;
    unsigned char fireForTutorial;
    uint8_t _pad_0x4a6[6];
    int field_0x4ac;
    int field_0x4b0;
    int field_0x4b4;
    int field_0x4b8;
    int timeExtenderTimer;
    int timeExtenderDuration;
    int field_0x4c4;
    unsigned char field_0x4c8;
    uint8_t _pad_0x4c9[3];
    int field_0x4cc;
    int field_0x4d0;
    int field_0x4d4;
    int field_0x4d8;
    int field_0x4dc;
    int analogStickRadius;
    int field_0x4e4;
    int field_0x4e8;
    int field_0x4ec;
    int field_0x4f0;
    unsigned int fireButtonImages[4];        // 0x4f4, indexed by fire mode
    unsigned int fireButtonPressedImages[4]; // 0x504
    int secondaryLabelTimerSeed;
    int secondaryLabelTimer;
    String field_0x51c;
    bool hackingGameActive;
    uint8_t _pad_0x529[3];
    int cargoAggregateCount;
    Array<unsigned int> * uintArray;
    Sprite * digitSprite;
    unsigned int multiplierIconImage;

    Hud();

    ~Hud();

    void addToEventQueue(ListItem *item);

    bool cargoFull();

    void catchCargo(int itemId, int count, bool cargoFull, bool missionDelivery, bool extender, bool slotMode,
                    bool aggregate);

    void checkIfQuickMenuIsEmpty();

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

    void init();

    void initHudMenu(int menuType, Level *lvl);

    bool isHackingGameActive();

    bool jumpMapSelected();

    void playerHit();

    void releaseAllKeys();

    void resetAnalogStick();

    bool sameHudEventAsBefore(String str);

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
static_assert(__builtin_offsetof(Hud, eventScrolls) == 0x1d8, "Hud::eventScrolls offset");
static_assert(__builtin_offsetof(Hud, currentSecondaryWeapon) == 0x258, "Hud::currentSecondaryWeapon offset");
static_assert(__builtin_offsetof(Hud, jumpMapSelectedFlag) == 0x274, "Hud::jumpMapSelectedFlag offset");
static_assert(__builtin_offsetof(Hud, field_0x275) == 0x275, "Hud::field_0x275 offset");
static_assert(__builtin_offsetof(Hud, field_0x1dc) == 0x1dc, "Hud::field_0x1dc offset");
static_assert(__builtin_offsetof(Hud, letterbox) == 0x1ec, "Hud::letterbox offset");
static_assert(__builtin_offsetof(Hud, field_0x1f0) == 0x1f0, "Hud::field_0x1f0 offset");
static_assert(__builtin_offsetof(Hud, field_0x45c) == 0x45c, "Hud::field_0x45c offset");
static_assert(__builtin_offsetof(Hud, field_0x464) == 0x464, "Hud::field_0x464 offset");
static_assert(__builtin_offsetof(Hud, field_0x46c) == 0x46c, "Hud::field_0x46c offset");
#endif
