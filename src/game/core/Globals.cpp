#include "game/core/Globals.h"
#include "engine/math/AEMath.h"
#include <arm_neon.h>
#include <cstdint>

namespace {
#pragma pack(push, 1)

    struct alignas(4) GameSettingsRecord {
        float colorR;
        float colorG;
        float colorB;

        // Bytes 0x0c..0x13: a single 8-byte region read under several overlapping
        // interpretations. Canonical storage is the byte array; the former named
        // members are accessed via reinterpret_cast at their byte offset within it:
        //   musicVolume  uint8  @0x0c -> _region[0]
        //   volumePair   int16  @0x0c -> reinterpret_cast<int16_t&>(_region[0])
        //   sfxVolume    uint8  @0x0d -> _region[1]
        //   ambientVolume uint8 @0x0e -> _region[2]
        //   flagAt0f     int16  @0x0f -> reinterpret_cast<int16_t&>(_region[3])
        //   flagAt10     int16  @0x10 -> reinterpret_cast<int16_t&>(_region[4])
        //   flagAt11     uint8  @0x11 -> _region[5]
        uint8_t _region[8];

        float glowR;
        float glowG;
        float tintR;
        float tintG;
        float tintB;
        float brightness;
        float contrast;
        uint8_t enableFlag30;
        int32_t intAt31;
        int32_t intAt35;
        uint8_t flagAt39;
        uint8_t _pad3a;
        int32_t intAt3b;
        uint8_t flagAt3f;
        int16_t shortAt40;
        uint8_t _pad42[2];
        float qualityLevel;
        int32_t intAt48;
        int16_t shortAt4c;
        uint8_t flagAt4e;
        uint8_t _pad4f;
        int32_t intAt50;
        int32_t resWidth;
        int32_t resHeight;
        uint8_t _pad5c[4];
        int16_t shortAt60;
    };

    struct InitZeroObject {
        // Region 0x00..0x3a. Canonical storage is the aligned int32 slot array
        // (slots[0..11] cover 0x00..0x2f); the misaligned tail ints overlap the
        // tail of that region and the trailing bytes, accessed via byte-offset
        // reinterpret_cast over slots:
        //   tailAt2b int32 @0x2b, tailAt2f @0x2f, tailAt33 @0x33, tailAt37 @0x37
        int32_t slots[12];
        uint8_t _tail_0x30[0x0b];
    };

    struct LineMetrics {
        int32_t field0;
        int32_t lineHeight;
    };

    struct CoordsObject {
        uint8_t _pad[0x54];
        int32_t steerValue;
        int32_t fireValue;
    };

    struct CtorSecondaryObject {
        int32_t slot0;
        int32_t slot1;
        uint8_t _pad08[0x0b];
        uint8_t flagAt13;
    };

    struct InitFlagByte {
        uint8_t flag;
    };

#pragma pack(pop)

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
    static_assert(offsetof(GameSettingsRecord, colorR) == 0x00, "colorR");
    static_assert(offsetof(GameSettingsRecord, colorG) == 0x04, "colorG");
    static_assert(offsetof(GameSettingsRecord, colorB) == 0x08, "colorB");
    static_assert(offsetof(GameSettingsRecord, _region) == 0x0c, "_region");
    static_assert(offsetof(GameSettingsRecord, glowR) == 0x14, "glowR");
    static_assert(offsetof(GameSettingsRecord, glowG) == 0x18, "glowG");
    static_assert(offsetof(GameSettingsRecord, tintR) == 0x1c, "tintR");
    static_assert(offsetof(GameSettingsRecord, tintG) == 0x20, "tintG");
    static_assert(offsetof(GameSettingsRecord, tintB) == 0x24, "tintB");
    static_assert(offsetof(GameSettingsRecord, brightness) == 0x28, "brightness");
    static_assert(offsetof(GameSettingsRecord, contrast) == 0x2c, "contrast");
    static_assert(offsetof(GameSettingsRecord, enableFlag30) == 0x30, "enableFlag30");
    static_assert(offsetof(GameSettingsRecord, intAt31) == 0x31, "intAt31");
    static_assert(offsetof(GameSettingsRecord, intAt35) == 0x35, "intAt35");
    static_assert(offsetof(GameSettingsRecord, flagAt39) == 0x39, "flagAt39");
    static_assert(offsetof(GameSettingsRecord, intAt3b) == 0x3b, "intAt3b");
    static_assert(offsetof(GameSettingsRecord, flagAt3f) == 0x3f, "flagAt3f");
    static_assert(offsetof(GameSettingsRecord, shortAt40) == 0x40, "shortAt40");
    static_assert(offsetof(GameSettingsRecord, qualityLevel) == 0x44, "qualityLevel");
    static_assert(offsetof(GameSettingsRecord, intAt48) == 0x48, "intAt48");
    static_assert(offsetof(GameSettingsRecord, shortAt4c) == 0x4c, "shortAt4c");
    static_assert(offsetof(GameSettingsRecord, flagAt4e) == 0x4e, "flagAt4e");
    static_assert(offsetof(GameSettingsRecord, intAt50) == 0x50, "intAt50");
    static_assert(offsetof(GameSettingsRecord, resWidth) == 0x54, "resWidth");
    static_assert(offsetof(GameSettingsRecord, resHeight) == 0x58, "resHeight");
    static_assert(offsetof(GameSettingsRecord, shortAt60) == 0x60, "shortAt60");

    static_assert(offsetof(InitZeroObject, slots) == 0x00, "slots");
    static_assert(offsetof(InitZeroObject, _tail_0x30) == 0x30, "_tail_0x30");
    static_assert(sizeof(InitZeroObject) == 0x3b, "InitZeroObject size");

    static_assert(offsetof(LineMetrics, lineHeight) == 0x04, "lineHeight");
    static_assert(offsetof(CoordsObject, steerValue) == 0x54, "steerValue");
    static_assert(offsetof(CoordsObject, fireValue) == 0x58, "fireValue");
    static_assert(offsetof(CtorSecondaryObject, slot1) == 0x04, "slot1");
    static_assert(offsetof(CtorSecondaryObject, flagAt13) == 0x13, "flagAt13");
#endif
}

int Globals::is_dialogue_window_visible = 0;
int Globals::is_choice_window_visible = 0;
int Globals::is_menu_visible = 0;
int Globals::is_hacking_visible = 0;
bool Globals::isStarMapVisible = false;
unsigned char Globals::isCinematicModeActive = 0;
int Globals::mouseCursorActivated = 0;
unsigned char Globals::showMouseDuringGameOver = 0;
unsigned char *Globals::keyBindings = nullptr;

int Globals::left_edge = 0;
int Globals::right_edge = 0;
int Globals::top_edge = 0;
int Globals::bottom_edge = 0;
int Globals::resetKeyboard = 0;
int Globals::rotateShipInStation = 0;
int Globals::translateStarMapInXDirection = 0;
int Globals::translateStarMapInYDirection = 0;
int Globals::smallButton_dim = 0;
int Globals::touch_stick_x = 0;
int Globals::touch_stick_y = 0;

unsigned char Globals::iPad = 0;
unsigned char Globals::iPadHD = 0;
unsigned char Globals::retinaDisplay = 0;
unsigned char Globals::n9 = 0;
unsigned char Globals::iPadLarge = 0;
unsigned char Globals::iPadLargePossible = 0;
unsigned char Globals::iPadAssetsWithLowerRes = 0;
unsigned char Globals::enterSpaceLounge = 0;
int Globals::switch_to_target_setting = 0;

Status *Globals::status = nullptr;
alignas(4) unsigned char Globals::options[100] = {};
FModSound *Globals::sound = nullptr;
int Globals::logoIsShown = -1;
int Globals::isInMainMenu = -1;

char *Globals::cItemListID_00 = nullptr;
char *Globals::cItemListID_01 = nullptr;
char *Globals::cItemListID_02 = nullptr;
char *Globals::cItemListID_03 = nullptr;
char *Globals::cItemListID_04 = nullptr;

char *Globals::cItemListName_00 = nullptr;
char *Globals::cItemListName_01 = nullptr;
char *Globals::cItemListName_02 = nullptr;
char *Globals::cItemListName_03 = nullptr;
char *Globals::cItemListName_04 = nullptr;
char *Globals::cItemListDescription_00 = nullptr;
char *Globals::cItemListDescription_01 = nullptr;
char *Globals::cItemListDescription_02 = nullptr;
char *Globals::cItemListDescription_03 = nullptr;
char *Globals::cItemListDescription_04 = nullptr;
char *Globals::cItemListCurrency_00 = nullptr;
char *Globals::cItemListCurrency_01 = nullptr;
char *Globals::cItemListCurrency_02 = nullptr;
char *Globals::cItemListCurrency_03 = nullptr;
char *Globals::cItemListCurrency_04 = nullptr;
char *Globals::cItemListPrice_00 = nullptr;
char *Globals::cItemListPrice_01 = nullptr;
char *Globals::cItemListPrice_02 = nullptr;
char *Globals::cItemListPrice_03 = nullptr;
char *Globals::cItemListPrice_04 = nullptr;

AbyssEngine::ApplicationManager *Globals::appManager;
unsigned char Globals::gameLoaded;
unsigned char Globals::gameSaving;
int Globals::sec_fire_x;
int Globals::sec_fire_y;
float Globals::sec_fire_z;
int Globals::autopilot_x;
int Globals::autopilot_y;
float Globals::autopilot_z;
int Globals::mouseDeltaX;
int Globals::mouseDeltaY;
int Globals::mouse_wheel;
int Globals::recordSlots;
Achievements *Globals::achievements;
ImageFactory *Globals::imageFactory;
int Globals::mouse_wheelX;
int Globals::mouse_wheelY;
int Globals::qualityLevel;
Ship *Globals::shipTemplate;
unsigned char Globals::showBestDeal = 1;
int Globals::simulateFire;
int Globals::subMenuIndex = -1;
int Globals::topMenuIndex;
int Globals::action_menu_x;
int Globals::action_menu_y;
float Globals::action_menu_z;
RecordHandler *Globals::recordHandler;
float Globals::touch_stick_z;
int Globals::turret_view_x;
int Globals::turret_view_y;
float Globals::turret_view_z;
int Globals::fast_forward_x;
int Globals::fast_forward_y;
float Globals::fast_forward_z;
int Globals::fontLangSelect;
int Globals::keyboard_swipe;
unsigned char Globals::iCloudSupported;
unsigned char Globals::isIOS6Installed;
int Globals::other_buttons_x[10];
int Globals::other_buttons_y[10];
unsigned char Globals::showWingmanMenu;
bool Globals::first_start_ever;
int Globals::globalPriceRaise;
unsigned char Globals::initMemoryWarning;
int Globals::instantActionType;
int Globals::instantActionWave;
unsigned char Globals::isTelekomCustomer;
int Globals::lastRecordWritten;
int Globals::instantActionScore;
unsigned char Globals::showNewCreditsMenu = 1;
int Globals::sub_menu_buttons_x[10];
int Globals::sub_menu_buttons_y[10];
unsigned char Globals::iap_hack_dlc1Bought;
unsigned char Globals::iap_hack_dlc2Bought;
unsigned char Globals::iap_hack_dlc3Bought;
int Globals::instantActionPoints;
unsigned char Globals::useRefractionShader;
int Globals::h;
int Globals::w;
int Globals::lastSpaceMusicPlayed;
int Globals::energyCellsProbChange;
int Globals::sub_menu_button_count;
unsigned char Globals::inAppPurchaseSupported = 1;
int Globals::lastStationMusicPlayed;
int Globals::menu_touch_window_type;
unsigned char Globals::useLowResTexturesForHD;
AbyssEngine::String *Globals::instantActionPlayerName;
unsigned char Globals::isRunningHDonWeakDevice;
float Globals::quickmenu_button_start_x;
float Globals::quickmenu_button_start_y;
int Globals::lastCampaignMissionFailed;
int Globals::secondaryWeaponsProbChange;
int Globals::lastCampaignMissionFailCount;
bool Globals::startLiteVersionWithMoreCredits;
AbyssEngine::AERandom *Globals::rnd;
void *Globals::font; // lint: void_ptr (opaque LoadingCallback userdata; ApplicationManager::SetLoadingCallback takes void*)
// lint: void_ptr (opaque LoadingCallback userdata; ApplicationManager::SetLoadingCallback takes void*)
int Globals::keys[255];
float Globals::bankZ;
unsigned char Globals::hints[59];
Array<Item *> *Globals::items;
Array<Ship *> *Globals::ships;
AbyssEngine::PaintCanvas *Globals::Canvas;
int Globals::fire_x;
int Globals::fire_y;
float Globals::fire_z;
Galaxy *Globals::galaxy;
Layout *Globals::layout;
int Globals::boost_x;
int Globals::boost_y;
float Globals::boost_z;
Globals *Globals::globals;
int Globals::pause_x;
int Globals::pause_y;
float Globals::pause_z;
GameText *Globals::gameText;
void *Globals::fontAlien; // lint: void_ptr (unknown alien-font handle, never referenced)
Generator *Globals::generator;
#include "engine/render/Mesh.h"
#include "game/ship/Ship.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"

#include "game/mission/Status.h"
#include "game/world/Galaxy.h"
#include "game/mission/Achievements.h"
#include "engine/core/AERandom.h"
#include "game/ship/Agent.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "game/ui/Layout.h"

#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "game/world/Standing.h"
#include "game/mission/Generator.h"
#include "game/world/Station.h"
#include "game/mission/RecordHandler.h"
#include "game/world/SolarSystem.h"
#include "game/core/String.h"
#include "engine/math/BoundingSphere.h"
#include "engine/render/ParticleSettingsRef.h"

#include <new>

static inline uint32_t nextInt_71aa4(AbyssEngine::AERandom *self) { return self->nextInt(); }

static inline int nextInt_71ad0(AbyssEngine::AERandom *self, int bound) { return self->nextInt(bound); }

void MatrixSetTranslation(void *m, float x, float y, float z); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

int AERandom_nextIntB(int rng, int bound);

void FileRead_ctor(void *self); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void *FileRead_dtor(void *self); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

float VectorScale(void *vec, float scalar); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * Galaxy_dtor(void *g) { ((Galaxy*)g)->~Galaxy(); return (void *)(g); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * Status_dtor(void *s) { ((Status*)s)->~Status(); return (void *)(s); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void *AERandom_dtor(void *r) { ((AbyssEngine::AERandom*)r)->~AERandom(); return r; } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * Layout_dtor(void *l) { ((Layout*)l)->~Layout(); return (void *)(l); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void *Generator_dtor(void *g); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * FModSound_dtor(void *s) { ((FModSound*)s)->~FModSound(); return (void *)(s); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * Achievements_dtor(void *a) { ((Achievements*)a)->~Achievements(); return (void *)(a); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * ImageFactory_dtor(void *f) { ((ImageFactory*)f)->~ImageFactory(); return (void *)(f); } // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void FileRead_ctor(void *f); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void *FileRead_dtor(void *f); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline int Station_getIndex(int station) { return (int)(((Station*)(long)station)->getIndex()); }


extern int g_android_leaderboard_scores[8];

void Globals::reportLeaderboards() {
    int kills = Globals::status->getKills();
    g_android_leaderboard_scores[0] = kills;
}

Array<int> *Globals::getSoundResourceList() {
    return soundResources;
}

#pragma pack(push, 1)
struct Q16 {
    int32x4_t v;
};

struct HintsBuffer {
    // Region 0x00..0x3f. Canonical storage is the aligned NEON-quad array
    // (quad[0..3] at 0x00/0x10/0x20/0x30); the misaligned quadAt2b (Q16 @0x2b)
    // overlaps quad[2]/quad[3] and is accessed via byte-offset reinterpret_cast.
    Q16 quad[4];
};
#pragma pack(pop)

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(offsetof(HintsBuffer, quad) == 0x00, "quad");
static_assert(sizeof(HintsBuffer) == 0x40, "HintsBuffer size");
#endif


void Globals::resetHints() {
    HintsBuffer *hints = (HintsBuffer *) Globals::hints;
    const int32x4_t z = vdupq_n_s32(0);
    hints->quad[0].v = z;
    reinterpret_cast<Q16 &>(reinterpret_cast<uint8_t *>(hints->quad)[0x2b]).v = z;
    hints->quad[2].v = z;
    hints->quad[1].v = z;
}

void Globals::startNewSoundResourceList() {
    if (this->soundResources != nullptr) {
        ArrayRelease(*(this->soundResources));
        delete this->soundResources;
    }
    this->soundResources = nullptr;
    Array<int> *list = new Array<int>();
    this->soundResources = list;
    ArrayAdd<int>(0x7c, *list);
    ArrayAdd<int>(0x7b, *this->soundResources);
}


String Globals::getItemName(int item) {
    String *src = Globals::gameText->getText(item + 0x4fa);
    return String(*src, false);
}

String Globals::getKeyActionName(int action) {
    (void) action;
    String r;
    return r;
}

float Globals::sqrt(float x) {
    return AbyssEngine::AEMath::Sqrtf(x);
}


SolarSystem *Globals::getRandomSystemForDrinks() {
    Galaxy *galaxy = Globals::galaxy;
    int picked = Globals::rnd->nextInt(0x16);
    return (SolarSystem *) (intptr_t) galaxy->getSystem(picked);
}

void Globals::addSoundResourceToList(int snd) {
    Array<int> *a = this->soundResources;
    if (a == nullptr) {
        return;
    }
    for (unsigned i = 0; i < a->size(); ++i) {
        if ((*a)[i] == snd) {
            return;
        }
    }
    ArrayAdd<int>(snd, *a);
}

String Globals::replaceKeyBindingTokens(const String &src) {
    return String(src, false);
}

struct FileRead {
    FileRead();

    ~FileRead();

    Station *loadStation(int32_t id);

    Array<int32_t> *loadWreckCollision(int32_t id);

    Array<Item *> *loadItemsBinary();

    Array<Ship *> *loadShipsBinary();

    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
};


Station *Globals::getRandomStation() {
    FileRead *f = new FileRead();
    int which = nextInt_71ad0(Globals::rnd, 0x87);
    Station *r = f->loadStation(which);
    delete f;
    return r;
}

void Globals::drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY) {
    drawLines(font, lines, baseX, startY, false);
}

void Globals::reportSupernovaChallengeScore() {
}


void Globals::getLineArray(unsigned int font, const String &text, int maxWidth,
                           Array<String *> *out) {
    String *line = new String();
    String work(text, false);
    work += String("\n", false);

    unsigned count = 0;
    for (int consumed = 0; consumed < work.length; count++) {
        String rest = work.SubString(consumed, work.length);
        getLine(font, String(rest, false), maxWidth, line);
        consumed += line->length;
    }
    delete line;

    ArraySetLength(count, *out);
    for (unsigned i = 0; i < count; i++)
        (*out)[i] = new String();

    int consumed = 0;
    for (unsigned i = 0; i < count; i++) {
        String rest = work.SubString(consumed, work.length);
        getLine(font, String(rest, false), maxWidth, (*out)[i]);
        int end = (*out)[i]->length;
        consumed += end;
        int start = 0;
        while (*(*(*out)[i])[start] == ' ')
            start++;
        while (*(*(*out)[i])[end - 1] == ' ')
            end--;
        *(*out)[i] = (*out)[i]->SubString(start, end);
    }
}

static const char gLTS2_secTens[] = "";
static const char gLTS2_secEmpty[] = "";
static const char gLTS2_minTens[] = "";
static const char gLTS2_minEmpty[] = "";
static const char gLTS2_zeroPrefix[] = "";
static const char gLTS2_hrTens[] = "";
static const char gLTS2_hrEmpty[] = "";
static const char gLTS2_sep1[] = "";
static const char gLTS2_sep2[] = "";

void Globals::longToTimeString(long long ms, String &out) {
    long long secQ = ms / 1000;
    int seconds = (int) (secQ % 0x3c);

    long long minQ = ms / 0xea60;
    int minute = (int) (minQ % 0x3c);

    long long hrQ = ms / 0xea60;
    int hours = (int) (hrQ % 0x18);

    String secPart, secNum, secStr;
    secPart.ctor_char(seconds < 10 ? gLTS2_secTens : gLTS2_secEmpty, false);
    secNum.Set((long long) (seconds));
    secStr = secPart + secNum;

    String minPart, minNum, minStr;
    minPart.ctor_char(minute < 10 ? gLTS2_minTens : gLTS2_minEmpty, false);
    minNum.Set((long long) (minute));
    minStr = minPart + minNum;

    if (hours == 0) {
        String prefix, left;
        prefix.ctor_char(gLTS2_zeroPrefix, false);
        left = prefix + minStr;
        out = left + secStr;
    } else {
        long long h = ms / 0xea60;
        int hv = (int) h * 0x18 + hours;

        String hrPart, hrNum, hrStr;
        hrPart.ctor_char(hv < 10 ? gLTS2_hrTens : gLTS2_hrEmpty, false);
        hrNum.Set((long long) (hv));
        hrStr = hrPart + hrNum;

        String s1, a, b, s2, c;
        s1.ctor_char(gLTS2_sep1, false);
        a = s1 + hrStr;
        b = a + minStr;
        s2.ctor_char(gLTS2_sep2, false);
        c = b + s2;
        out = c + secStr;
    }

    return;
}


String Globals::getBoundedString(const String &text, int width) {
    String result(text, false);
    if (Globals::Canvas->GetTextWidth(reinterpret_cast<unsigned int &>(Globals::font), text) > width) {
        String *line = new String();
        getLine(reinterpret_cast<unsigned int &>(Globals::font), String(text, false), width - 3, line);
        result = *line + String("...", false);
        delete line;
    }
    return result;
}

static const float gSCS_f86f8 = 210.9375f;
static const float gSCS_f86fc = 300.0f;
static const float gSCS_f8700 = 150.0f;
static const float gSCS_f8704 = -126.5625f;
static const float gSCS_f8708 = 92.8125f;
static const float gSCS_f870c = 180.0f;
static const float gSCS_f8710 = 90.0f;
static const float gSCS_f8714 = 132.0f;
static const float gSCS_f8718 = 66.0f;
static const float gSCS_f871c = 313.59375f;
static const float gSCS_f8720 = 2.8125f;
static const float gSCS_f8724 = 446.0f;
static const float gSCS_f8728 = 223.0f;
static const float gSCS_f872c = -2.8125f;
static const float gSCS_f8730 = 54.0f;


static inline char rdflag(char *const g) { return *g; }
static inline int *rdobj(int *const g) { return g; }
static inline CoordsObject *rdcoords(CoordsObject *const g) { return g; }

void Globals::setCoordsSteer(int p1, int p2, int p3, int p4,
                             unsigned short &o5, unsigned short &o6, unsigned short &o7,
                             unsigned short &o8, unsigned short &o9, unsigned short &o10,
                             unsigned short &o11, unsigned short &o12, unsigned short &o13,
                             unsigned short &o14) {
    int screenH = Globals::h;
    char isPhone = rdflag((char *) &Globals::iPadHD);
    int bottom = ((-0x19 - p2) - p3) + screenH;

    float thresh = gSCS_f86f8;
    if (isPhone == 0) {
        thresh = (rdflag((char *) &Globals::iPadLarge) == 0) ? gSCS_f8700 : gSCS_f86fc;
    }
    if (bottom < p1) {
        p1 = bottom;
    }
    float fp1 = static_cast<float>(p1);

    unsigned short uv;
    int iv;
    if (thresh < fp1) {
        iv = (int) fp1;
        uv = (unsigned short) iv;
        rdcoords((CoordsObject *) Globals::options)->steerValue = (int) fp1;
        char flag8;
        if (isPhone == 0) {
            flag8 = rdflag((char *) &Globals::iPadLarge);
            goto label8508;
        }
        goto common;
    label8508:
        uv = (flag8 == 0) ? 0x14 : 0x28;
        o5 = uv;
        o6 = (unsigned short) iv;
        o13 = uv;
        {
            float sub = (flag8 == 0) ? gSCS_f8710 : gSCS_f870c;
            float add14 = (flag8 == 0) ? gSCS_f8718 : gSCS_f8714;
            float val = static_cast<float>(o6);
            float r = val - sub;
            o14 = (unsigned short) ((0.0f < r) ? (short) (int) r : 0);
            o7 = 0x14;
            (void) add14;
            goto label8556;
        }
    } else {
        if (isPhone == 0) {
            char flag = rdflag((char *) &Globals::iPadLarge);
            iv = (flag == 0) ? 0x96 : 0x12c;
            rdcoords((CoordsObject *) Globals::options)->steerValue = iv;
            char flag8 = (flag == 0) ? 0 : 1;
            (void) flag8;
            goto label8508;
        }
        uv = 0xd2;
        rdcoords((CoordsObject *) Globals::options)->steerValue = 0xd2;
    }

common: {
        float fv = gSCS_f8708;
        o5 = 0x1c;
        o6 = uv;
        o13 = 0x1c;
        float val = static_cast<float>(o6);
        float r = val + gSCS_f8704;
        o14 = (unsigned short) ((0.0f < r) ? (short) (int) r : 0);
        o7 = 0x14;
        (void) fv;
    }

label8556: {
        unsigned absp2 = (unsigned) (p2 - (p2 >> 31));
        float fv = gSCS_f8708;
        float val6 = static_cast<float>(o6);
        float r8 = fv + val6;
        o8 = (unsigned short) ((0.0f < r8) ? (short) (int) r8 : 0);
        o9 = (unsigned short) (o7 + (short) (absp2 >> 1));
        o10 = (unsigned short) (o8 + (short) (absp2 >> 1));

        float val6b = static_cast<float>(o6);
        float fv2;
        float acc;
        if (isPhone == 0) {
            bool b = (rdflag((char *) &Globals::iPadLarge) == 0);
            acc = (b ? gSCS_f8728 : gSCS_f8724) + val6b;
            fv2 = b ? 2.0f : 4.0f;
        } else {
            acc = val6b + gSCS_f871c;
            fv2 = gSCS_f8720;
        }
        int i12 = (0.0f < acc) ? (int) acc : 0;
        o12 = (unsigned short) i12;

        float top = static_cast<float>(screenH - p4);
        float f12 = static_cast<float>((unsigned) i12);
        if (f12 <= top - fv2) {
            unsigned short u11;
            if (isPhone == 0) {
                u11 = (rdflag((char *) &Globals::iPadLarge) == 0) ? 0x14 : 0x28;
            } else {
                u11 = 0x1c;
            }
            o11 = u11;
        } else {
            int a, b;
            if (isPhone == 0) {
                float fv14 = 4.0f;
                a = 0x15a;
                char flag8 = rdflag((char *) &Globals::iPadLarge);
                if (flag8 == 0) fv14 = 2.0f;
                b = 0x10a;
                float d = f12 - (top - fv14);
                int i16 = (0.0f < d) ? (int) d : 0;
                o12 = (unsigned short) i16;
                if (flag8 == 0) {
                    a = 0xad;
                    b = 0x85;
                }
                float fd = static_cast<float>((unsigned) i16);
                float fr = static_cast<float>(a - b);
                float scale = 1.0f;
                if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
                float base = static_cast<float>(b);
                base = base + scale * fr;
                o11 = (unsigned short) ((0.0f < base) ? (short) (int) base : 0);
                float fv15 = 4.0f;
                if (rdflag((char *) &Globals::iPadLarge) == 0) fv15 = 2.0f;
                o12 = (unsigned short) ((0.0f < top - fv15) ? (short) (int) (top - fv15) : 0);
            } else {
                a = 0xf3;
                b = 0xbb;
                float d = f12 - (top + gSCS_f872c);
                int i16 = (0.0f < d) ? (int) d : 0;
                o12 = (unsigned short) i16;
                float fd = static_cast<float>((unsigned) i16);
                float fr = static_cast<float>(a - b);
                float scale = 1.0f;
                if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
                float base = static_cast<float>(b);
                base = base + scale * fr;
                o11 = (unsigned short) ((0.0f < base) ? (short) (int) base : 0);
                o12 = (unsigned short) ((0.0f < top - gSCS_f8720) ? (short) (int) (top - gSCS_f8720) : 0);
            }
        }
    }
}

String Globals::getAgentMissionText(Agent *agent) {
    if (agent == nullptr) {
        return String("");
    }

    String text;

    if (agent->isGenericAgent()) {
        switch (agent->getOffer()) {
            case 7: {
                int race = agent->getRace();
                int standing = ((Standing *) (intptr_t) Globals::status->getStanding())
                    ->getStanding(race == 2 || race == 3 ? 1 : 0);
                if (!((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(race)) {
                    text = *Globals::gameText->getText(883);
                    return text;
                }
                int costs = (int) ((float) abs(standing) / 100.0f * 32000.0f);
                int textId = race == 2 ? 878 : race == 3 ? 880 : race == 0 ? 881 : 882;
                text = *Globals::gameText->getText(textId);
                text = Globals::status->replaceHash(String(text, false),
                                                    String(Globals::layout->formatCredits(costs), false),
                                                    String("#C", false));
                break;
            }
            case 5: {
                if (agent->getMission() == nullptr) {
                    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
                    agent->setMission(Globals::generator->createMission(agent, systems));
                }
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 777 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text += *Globals::gameText->getText(firstLine);
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(String(agent->getMission()->getProductionGoodAmount()), false),
                    String("#Q", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(*Globals::gameText->getText(1274 + agent->getMission()->getProductionGoodIndex()), false),
                    String("#P", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(Globals::layout->formatCredits(agent->getMission()->getReward()), false),
                    String("#C", false));
                break;
            }
            case 1: {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 824;
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                Globals::rnd->setSeed(agent->getName().size() * agent->getRace() +
                                      Globals::status->getStation()->getIndex());
                int planetPos = text.IndexOf(String("#S"));
                if (planetPos >= 0) {
                    text = Globals::status->replaceHash(String(text, false),
                                                        String(Globals::getRandomPlanetName(), false),
                                                        String("#S", false));
                }
                text = Globals::status->replaceHash(String(text, false), String(agent->getName(), false),
                                                    String("#N", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(*Globals::gameText->getText(1428 + Globals::rnd->nextInt(10)), false),
                    String("#ORE", false));
                Globals::rnd->reset();
                break;
            }
            case 0: {
                Mission *mission = agent->getMission();
                if (mission == nullptr) {
                    agent->field_0x28 = 820;
                    text = *Globals::gameText->getText(agent->field_0x28);
                    break;
                }
                text += *Globals::gameText->getText(786 + mission->getType());
                if (mission->getType() == 5 || mission->getType() == 3) {
                    text += String(" ") + *Globals::gameText->getText(802);
                }
                if (mission->getType() == 15) {
                    text = Globals::status->replaceHash(
                        String(text, false),
                        String(*Globals::gameText->getText(1274 + mission->getProductionGoodIndex()), false),
                        String("#P", false));
                } else {
                    text = Globals::status->replaceHash(
                        String(text, false),
                        String(*Globals::gameText->getText(813 + mission->getProductionGoodIndex()), false),
                        String("#P", false));
                }
                text = Globals::status->replaceHash(String(text, false),
                                                    String(String(mission->getProductionGoodAmount()), false),
                                                    String("#Q", false));
                if (mission->getType() == 14) {
                    text = Globals::status->replaceHash(String(text, false),
                                                        String(mission->getTargetSystemName(), false),
                                                        String("#S", false));
                } else {
                    text = Globals::status->replaceHash(String(text, false),
                                                        String(mission->getTargetStationName(), false),
                                                        String("#S", false));
                }
                text = Globals::status->replaceHash(String(text, false), String(mission->getTargetName(), false),
                                                    String("#N", false));
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 764 + Globals::rnd->nextInt(3);
                }
                agent->field_0x28 = firstLine;
                text += String("\n") + *Globals::gameText->getText(firstLine);

                float bonus = ((Standing *) (intptr_t) Globals::status->getStanding())->getMissionBonus(agent->getRace());
                String bonusText = bonus > 0.0f
                                       ? String(" ") + Globals::status->replaceHash(
                                             String(*Globals::gameText->getText(767), false),
                                             String(String((int) (bonus * 100.0f)), false),
                                             String("#P", false)) + String(" ")
                                       : String("");
                int extra = (int) (bonus * (float) agent->getMission()->getReward());
                int rest = extra % 50;
                agent->getMission()->setBonus((extra + rest) % 50 == 0 ? extra + rest : extra - rest);
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(Globals::layout->formatCredits(mission->getReward() + mission->getBonus()), false) +
                    bonusText,
                    String("#C", false));
                if (mission->getStatusValue() == -1) {
                    text = *Globals::gameText->getText(803);
                    text = Globals::status->replaceHash(String(text, false), mission->getTargetStationName(),
                                                        String("#S", false));
                }
                break;
            }
            case 2: {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 768 + Globals::rnd->nextInt(5);
                }
                agent->field_0x28 = firstLine;
                text += *Globals::gameText->getText(firstLine);
                int secondLine = agent->field_0x2c;
                if (secondLine == -1) {
                    secondLine = 773 + Globals::rnd->nextInt(2);
                }
                agent->field_0x2c = secondLine;
                text += String("\n") + *Globals::gameText->getText(secondLine);
                text = Globals::status->replaceHash(String(text, false),
                                                    String(String(agent->getSellItemQuantity()), false),
                                                    String("#Q", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(*Globals::gameText->getText(1274 + agent->getSellItemIndex()), false),
                    String("#P", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                    String("#C", false));
                if (agent->getSellItemQuantity() > 1) {
                    text += String(" ") + *Globals::gameText->getText(775);
                    text = Globals::status->replaceHash(
                        String(text, false),
                        String(Globals::layout->formatCredits(agent->getSellItemPrice() / agent->getSellItemQuantity()),
                               false),
                        String("#C", false));
                }
                break;
            }
            case 6:
                text += *Globals::gameText->getText((Globals::achievements->gotAllMedals() ? 782 : 779) +
                                                    agent->getWingmanFriendsCount());
                text = Globals::status->replaceHash(String(text, false),
                                                    String(Globals::layout->formatCredits(agent->getCosts()), false),
                                                    String("#C", false));
                if (agent->getWingmanFriendsCount() > 0) {
                    text = Globals::status->replaceHash(String(text, false), String(agent->getWingmanName(1), false),
                                                        String("#W", false));
                }
                break;
            case 3:
            case 4:
            default:
                break;
        }

        if (agent->getMission() != nullptr && agent->getMission()->getType() == 12) {
            text = *Globals::gameText->getText(798);
            text = Globals::status->replaceHash(
                String(text, false),
                String(Globals::layout->formatCredits(agent->getMission()->getReward() +
                                                      agent->getMission()->getBonus()), false),
                String("#C", false));
        }
    } else {
        String offerText;
        if (agent->getEvent() > 0) {
            text = *Globals::gameText->getText(858);
        } else if (agent->hasAcceptedOffer()) {
            text = *Globals::gameText->getText(858);
        } else {
            Globals::status->field_d0++;
            if (agent->getOffer() == 8) {
                agent->setSellItemPrice(Globals::status->getShip()->getPrice() * agent->getModPricePercentage() / 100);
                if (Globals::status->getShip()->hasModInstalled(agent->getSellModIndex())) {
                    text = *Globals::gameText->getText(858);
                    Globals::status->field_d0--;
                    return text;
                }
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 874 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                text += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                text += String(" ") + *Globals::gameText->getText(879);
                text = Globals::status->replaceHash(
                    String(offerText, false),
                    String(Globals::layout->formatCredits(Globals::options[56]
                                                              ? (int) (float) agent->getSellItemPrice()
                                                              : (int) ((float) agent->getSellItemPrice() * 2.0f)),
                           false),
                    String("#C", false));
            } else if (agent->getOffer() == 9) {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 874 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                text += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                int secondLine = agent->field_0x2c;
                if (secondLine == -1) {
                    secondLine = 773 + Globals::rnd->nextInt(2);
                }
                agent->field_0x2c = secondLine;
                text += String("\n") + *Globals::gameText->getText(secondLine);
                text = Globals::status->replaceHash(String(text, false),
                                                    String(String(agent->getSellItemQuantity()), false),
                                                    String("#Q", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(*Globals::gameText->getText(1274 + agent->getSellItemIndex()), false),
                    String("#P", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(Globals::layout->formatCredits(Globals::options[56]
                                                              ? (int) (float) agent->getSellItemPrice()
                                                              : (int) ((float) agent->getSellItemPrice() * 2.0f)),
                           false),
                    String("#C", false));
            } else if (agent->getOffer() == 10) {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 874 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                text += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                int secondLine = agent->field_0x2c;
                if (secondLine == -1) {
                    secondLine = 773 + Globals::rnd->nextInt(2);
                }
                agent->field_0x2c = secondLine;
                text += String("\n") + *Globals::gameText->getText(secondLine);
                text = Globals::status->replaceHash(String(text, false), String(String(1), false),
                                                    String("#Q", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(*Globals::gameText->getText(913 + agent->getSellItemIndex()), false),
                    String("#P", false));
                text = Globals::status->replaceHash(
                    String(text, false),
                    String(Globals::layout->formatCredits(Globals::options[56]
                                                              ? (int) (float) agent->getSellItemPrice()
                                                              : (int) ((float) agent->getSellItemPrice() * 2.0f)),
                           false),
                    String("#C", false));
            } else if (agent->getOffer() == 4) {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 874 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                text += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                text += String(" ") + *Globals::gameText->getText(877);
                SolarSystem *system = (SolarSystem *) (intptr_t) Globals::galaxy->getSystem(agent->getSellSystemIndex());
                text = Globals::status->replaceHash(String(offerText, false), String(system->getName(), false),
                                                    String("#S", false));
                text = Globals::status->replaceHash(
                    String(offerText, false),
                    String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                    String("#C", false));
            } else {
                int firstLine = agent->field_0x28;
                if (firstLine == -1) {
                    firstLine = 874 + Globals::rnd->nextInt(2);
                }
                agent->field_0x28 = firstLine;
                text = *Globals::gameText->getText(firstLine);
                text += String(" ") + *Globals::gameText->getText(886 + agent->getIndex());
                text += String(" ") + *Globals::gameText->getText(876);
                text = Globals::status->replaceHash(
                    String(offerText, false),
                    String(*Globals::gameText->getText(
                               1274 + (*Globals::items)[agent->getSellBlueprintIndex()]->getIndex()), false),
                    String("#N", false));
                text = Globals::status->replaceHash(
                    String(offerText, false),
                    String(Globals::layout->formatCredits(agent->getSellItemPrice()), false),
                    String("#C", false));
            }
            text += String("\n") + *Globals::gameText->getText(841 + Globals::rnd->nextInt(3));
        }
    }

    return text;
}

static const char gIAP_prefixA[] = "www.fishlabs.net.gof2hd";
static const char gIAP_prefixB[] = ".";
static const char gIAP_id50[] = "dlcCredits1";
static const char gIAP_id0[] = "dlc1";
static const char gIAP_id1[] = "dlc2";
static const char gIAP_id2[] = "dlc3";
static const char gIAP_id3[] = "dlc4";
static const char gIAP_id4[] = "dlc5";
static const char gIAP_id51[] = "dlcCredits2";
static const char gIAP_id52[] = "dlcCredits3";
static const char gIAP_id53[] = "dlcCredits4.0";
static const char gIAP_id54[] = "dlcCredits5";

int Globals::getInAppPurchaseArrayIndex(int productCode, Array<String *> *list) {
    int result = -1;

    if (list != 0) {
        unsigned len = list->size();
        unsigned i = 0;
        bool keepGoing = true;
        while (i < len && keepGoing) {
            String *entry = (*list)[i];
            String base, pb, prefix;
            base.ctor_char(gIAP_prefixA, false);
            pb.ctor_char(gIAP_prefixB, false);
            prefix = base + pb;

            if (entry == 0) {
                result = -1;
                keepGoing = false;
            } else {
                const char *suffix = 0;
                bool known = true;
                switch (productCode) {
                    case 0: suffix = gIAP_id0;
                        break;
                    case 1: suffix = gIAP_id1;
                        break;
                    case 2: suffix = gIAP_id2;
                        break;
                    case 3: suffix = gIAP_id3;
                        break;
                    case 4: suffix = gIAP_id4;
                        break;
                    case 0x32: suffix = gIAP_id50;
                        break;
                    case 0x33: suffix = gIAP_id51;
                        break;
                    case 0x34: suffix = gIAP_id52;
                        break;
                    case 0x35: suffix = gIAP_id53;
                        break;
                    case 0x36: suffix = gIAP_id54;
                        break;
                    default: known = false;
                        break;
                }
                if (!known) {
                    keepGoing = true;
                } else {
                    String suf, full;
                    suf.ctor_char(suffix, false);
                    full = prefix + suf;
                    int cmp = entry->Compare(full);
                    if (cmp == 0) {
                        result = (int) i;
                        keepGoing = false;
                    } else {
                        keepGoing = true;
                    }
                }
            }
            i++;
        }
    }

    return result;
}

String Globals::getKeyBindingReplaceString(int key) {
    (void) key;

    String tmp;
    return String(*tmp.ToUpperCase(), false);
}

static const char gLTS_minTens[] = "";
static const char gLTS_minEmpty[] = "";
static const char gLTS_hrTens[] = "";
static const char gLTS_hrEmpty[] = "";
static const char gLTS_sep[] = "";

void Globals::longToTimeStringNoSeconds(long long ms, String &out) {
    int minutes = (int) (ms / 60000 % 60);
    int hours = (int) (ms / 3600000 % 24);
    String minuteText = (minutes < 10 ? String("0", false) : String("", false)) + String(minutes);
    hours += (int) (ms / 86400000) * 24;
    String hourText = (hours < 10 ? String("0", false) : String("", false)) + String(hours);
    out = hourText + String(":", false) + minuteText;
}

static const unsigned short gGSG_resTable[64] = {
    0x4268, 0x4269, 0x426a, 0x426b, 0x426c, 0x426d, 0x426e, 0x426f,
    0x4270, 0x4271, 0x4272, 0x4273, 0x4274, 0x4275, 0x4276, 0x4277,
    0x4278, 0x4279, 0x427a, 0x427b, 0x427c, 0x427d, 0x427e, 0x427f,
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287,
    0x4288, 0x4289, 0x428a, 0x428b, 0x428c, 0x433b, 0x4340, 0x4345,
    0x434b, 0x4351, 0x4357, 0x435d, 0x4363, 0x4369, 0x436f, 0x4375,
    0x437b, 0x4381, 0x4387, 0x438d, 0x4393, 0x4399, 0x439f, 0x43a5,
    0x43ab, 0x43b1, 0x43b7, 0x43bd, 0x43c3, 0x43c9, 0x43cf, 0x43d5,
};
static const unsigned short gGSG_meshTable[64] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0x4910, 0xffff, 0xffff, 0xffff,
    0x4914, 0x4915, 0xffff, 0xffff, 0x4918, 0xffff, 0xffff, 0xffff,
    0x491c, 0xffff, 0x491e, 0xffff, 0xffff, 0x4921, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x492b,
    0xffff, 0xffff, 0xffff, 0x492f, 0xffff, 0xffff, 0xffff, 0x4933,
    0xffff, 0x4935, 0x4936, 0x4937, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0x4386, 0x438c, 0xffff, 0xffff, 0x439e, 0x43a4, 0x4982,
    0x4983, 0x4984, 0x4985, 0x4986, 0x4987, 0xffff, 0xffff, 0x43da,
};
static const short gGSG_extraTable[64] = {
    0x490c, 0x490d, 0x490e, 0x490f, -1, 0x4911, 0x4912, 0x4913,
    -1, -1, 0x4916, 0x4917, -1, -1, -1, -1,
    -1, 0x491d, -1, 0x491f, 0x4920, -1, 0x4922, 0x4923,
    0x4924, 0x4925, 0x4926, 0x4927, 0x4928, 0x4929, 0x492a, -1,
    0x492c, 0x492d, 0x492e, -1, 0x4930, 0x433f, 0x4932, -1,
    0x434f, -1, 0x435b, 0x4361, 0x495b, 0x497b, 0x497c, 0x497d,
    0x495c, -1, 0x438b, 0x497e, 0x497f, -1, -1, 0x4982,
    0x4983, 0x4984, 0x4985, 0x4986, 0x4987, 0x4988, 0x4989, 0x498a,
};
// Per ship kind: up to three LOD mesh ids (0xffff = none).
static const unsigned gGSG_lodTable[64 * 3] = {
    0x42cc, 0x42cd, 0x42ce, 0x42cf, 0x42d0, 0x42d1,
    0x42d2, 0x42d3, 0x42d4, 0x42d5, 0x42d6, 0x42d7,
    0x42d8, 0x42d9, 0x42da, 0x42db, 0x42dc, 0x42dd,
    0x42de, 0x42df, 0x42e0, 0x42e1, 0x42e2, 0x42e3,
    0x42e4, 0x42e5, 0x42e6, 0x42e7, 0x42e8, 0x42e8,
    0x42ea, 0xffff, 0xffff, 0x42ed, 0x42ee, 0x42ef,
    0x42f0, 0x42f1, 0x42f2, 0x42f3, 0x42f4, 0x42f5,
    0x42f6, 0x42f7, 0x42f8, 0x42f9, 0x42fa, 0x42fb,
    0x42fc, 0x42fd, 0x42fe, 0x42ff, 0x4300, 0x4301,
    0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307,
    0x4308, 0x4309, 0x430a, 0x430b, 0x430c, 0x430d,
    0x430e, 0x430f, 0x4310, 0x4311, 0x4312, 0x4313,
    0x4314, 0x4315, 0x4316, 0x4317, 0x4318, 0x4319,
    0x431a, 0x431b, 0x431c, 0x431d, 0x431e, 0x431f,
    0x4320, 0x4321, 0x4322, 0x4323, 0x4324, 0x4325,
    0x4326, 0x4327, 0x4328, 0x4329, 0x432a, 0x432b,
    0x432c, 0x432d, 0x432e, 0x432f, 0x4330, 0x4331,
    0x4332, 0x4333, 0x4334, 0x4335, 0x4336, 0x4337,
    0x4338, 0x4339, 0x433a, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0x4346, 0x4347, 0xffff,
    0xffff, 0xffff, 0xffff, 0x4352, 0x4353, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};
static const unsigned gGSG_childTable[64 * 3] = {
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0x4914, 0x4914, 0x4914, 0x4915, 0x4915, 0x4915,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0x4290, 0x4291, 0x4292,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

AEGeometry *Globals::getShipGroup(int kind, int variant, bool wireframe) {
    PaintCanvas *canvas = Globals::Canvas;

    if (kind == 0xf) {
        AEGeometry *geom;

        if (variant == 0) {
            geom = new AEGeometry((uint16_t) 0x42a9, canvas, false);

            unsigned mesh0 = 0xffffffff, mesh1 = 0xffffffff, mesh2 = 0xffffffff;
            canvas->TransformCreate(mesh0);
            canvas->TransformAddMesh(mesh0, 0x42ae, false);
            geom->addChild(mesh0);
            canvas->TransformCreate(mesh1);
            canvas->TransformAddMesh(mesh1, 0x42b2, false);
            geom->addChild(mesh1);
            canvas->TransformCreate(mesh2);
            canvas->TransformAddMesh(mesh2, 0x42ad, false);
            geom->addChild(mesh2);

            uint16_t lodMeshes[2] = {0x42aa, 0x42ab};
            int lodDists[2] = {25000, 45000};
            geom->setLodMeshes(lodMeshes, lodDists, 2);
            uint16_t lodChild = 0x42ae;
            geom->setLodChildMeshes(&lodChild);
        } else if (variant == 3) {
            geom = new AEGeometry((uint16_t) 0x4299, canvas, false);

            unsigned head = 0xffffffff;
            canvas->TransformCreate(head);
            canvas->TransformAddMesh(head, 0x4299, true);
            geom->addChild(head);

            int segments = AERandom_nextIntB(0, 4);
            unsigned prevA = 0xffffffff, prevB = 0xffffffff;
            for (int i = 0; i < segments; i++) {
                unsigned a = 0xffffffff, b = 0xffffffff;
                canvas->TransformCreate(a);
                canvas->TransformCreate(b);
                canvas->TransformAddMesh(a, 0x429a, true);
                canvas->TransformAddMesh(b, 0x429a, true);
                unsigned keepA = a, keepB = b;
                if (prevA != 0xffffffff) {
                    canvas->TransformAddChild(prevA, a);
                    canvas->TransformAddChild(prevB, b);
                    keepA = prevA;
                    keepB = prevB;
                }
                prevA = keepA;
                prevB = keepB;
            }
            if (prevA != 0xffffffff) {
                geom->addChild(prevA);
            }

            unsigned tail = 0xffffffff;
            canvas->TransformCreate(tail);
            canvas->TransformAddMesh(tail, 0x429a, true);
            geom->addChild(tail);

            uint16_t lodMeshes[1] = {0x429a};
            int lodDists[1] = {35000};
            geom->setLodMeshes(lodMeshes, lodDists, 1);
            geom->setLodChildTransform(prevB);
        } else {
            geom = new AEGeometry((uint16_t) 0x42a4, canvas, false);

            unsigned mesh0 = 0xffffffff, mesh1 = 0xffffffff;
            canvas->TransformCreate(mesh0);
            canvas->TransformAddMesh(mesh0, 0x42a4, true);
            geom->addChild(mesh0);
            canvas->TransformCreate(mesh1);
            canvas->TransformAddMesh(mesh1, 0x42a4, true);
            geom->addChild(mesh1);

            uint16_t lodMeshes[2] = {0x42a6, 0x42a7};
            int lodDists[2] = {35000, 60000};
            geom->setLodMeshes(lodMeshes, lodDists, 2);
        }

        return geom;
    }
    if (kind == 0xe || kind == 0xd) {
        int resId = (kind == 0xe) ? 0x37e7 : 0x4275;
        AEGeometry *geom = new AEGeometry((uint16_t) resId, canvas, false);
        unsigned t0 = 0xffffffff;
        canvas->TransformCreate(t0);
        canvas->TransformAddMesh((unsigned) t0, 0, (bool) (1));
        geom->addChild(t0);
        unsigned t1 = 0xffffffff;
        canvas->TransformCreate(t1);
        canvas->TransformAddMesh((unsigned) t1, 0, (bool) (1));
        geom->addChild(t1);
        unsigned short lodMeshes[2] = {0, 0};
        int dist[2];
        dist[0] = (kind == 0xe) ? 35000 : 35000;
        dist[1] = (kind == 0xe) ? 60000 : 45000;
        geom->setLodMeshes(lodMeshes, dist, 2);
        unsigned short childMeshes[1] = {0};
        geom->setLodChildMeshes(childMeshes);
        if (kind == 0xe) {
            geom->setScaling(1.0f);
        }
        return geom;
    }

    {
        AEGeometry *geom = new AEGeometry((uint16_t) gGSG_resTable[kind], canvas, true);
        unsigned short mesh = gGSG_meshTable[kind];
        unsigned mainT = 0xffffffff;
        unsigned mainMesh = 0xffffffff;
        if (mesh != 0xffff) {
            canvas->MeshCreate(mesh, mainMesh, true);
            canvas->TransformCreate(mainT);
            canvas->TransformAddMeshId(mainT, mainMesh);
            geom->addChild(mainT);
            geom->meshHandle = mainMesh;
        }
        if (!wireframe) {
            unsigned short mat = (unsigned short) ((short) kind + 0x7dc8);
            unsigned matH = 0xffffffff;
            canvas->MaterialCreate(mat, matH);
            canvas->MeshChangeResourceMaterial(geom->meshId, mat);
        }
        short extra = gGSG_extraTable[kind];
        if (extra != -1) {
            unsigned t = 0xffffffff;
            canvas->TransformCreate(t);
            canvas->TransformAddMesh((unsigned) t, 0, (bool) ((int) (unsigned char) (char) extra));
            geom->addChild(t);
        }
        if (wireframe) {
            if (kind != 0x27 && kind != 0x29) {
                unsigned t = 0xffffffff;
                canvas->TransformCreate(t);
                canvas->TransformAddMesh((unsigned) t, 0, (bool) ((int) (char) (-0x14 + (char) kind)));
                geom->addChild(t);
            }
        } else {
            unsigned t = 0xffffffff;
            canvas->TransformCreate(t);
            canvas->TransformAddMesh((unsigned) t, 0, (bool) ((int) (char) (0x50 + (char) kind)));
            geom->addChild(t);
        }

        unsigned lastVisibleDist = 2;
        const unsigned *lod = &gGSG_lodTable[kind * 3];
        unsigned count = 0;
        for (int i = 0; i != 2; i++) {
            if (lod[i] != 0xffff) count++;
        }
        if (count != 0) {
            unsigned short *meshes = new unsigned short[count];
            unsigned *ids = new unsigned[count];
            int *dist = new int[count];
            int d = 5000;
            unsigned *idp = ids;
            const unsigned *src = lod;
            for (unsigned i = 0; i < count; i++) {
                unsigned m = *src;
                dist[i] = d;
                meshes[i] = (unsigned short) m;
                if (!wireframe) {
                    canvas->MeshCreate((unsigned short) m, *idp, true);
                    canvas->MeshChangeResourceMaterial(*idp,
                                                       (unsigned short) ((short) kind + 0x7dc8));
                }
                d += 8000;
                idp++;
                src++;
            }
            lastVisibleDist = (unsigned) d;
            if (wireframe) {
                geom->setLodMeshes(meshes, dist, count);
            } else {
                geom->setLodMeshesWithMeshIds(meshes, ids, dist, count);
            }

            const unsigned *childSrc = &gGSG_childTable[kind * 3];
            int childCount = 0;
            for (int i = 0; i != 2; i++) {
                if (childSrc[i] != 0xffff) childCount++;
            }
            if (childCount != 0) {
                unsigned short *childMeshes = new unsigned short[childCount];
                for (int i = 0; i != childCount; i++) {
                    childMeshes[i] = (unsigned short) childSrc[i];
                }
                geom->setLodChildMeshes(childMeshes);
                delete[] childMeshes;
            }
            delete[] meshes;
            delete[] dist;
        }
        geom->setLodLastVisibleDistance(lastVisibleDist);
        return geom;
    }
}

static const int gRaceTable[64] = {
    3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0,
    2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
    8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3,
    3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1};

unsigned Globals::getRandomEnemyFighter(int kind) {
    (void) this;
    int t = kind;
    if (kind > 3 && kind != 9 && kind != 10) {
        t = 8;
    }
    unsigned r;
    if (t == 1) {
        if (Globals::status->dlc1Won() == 0) {
            r = 9;
        } else {
            int n = nextInt_71ad0(Globals::rnd, 0x64);
            r = 0x27;
            if (n < 0x55) {
                r = 0x29;
            }
            if (n < 0x3c) {
                r = 9;
            }
        }
    } else if (t == 9) {
        r = 8;
    } else if (t == 0xa) {
        r = 0x2c;
    } else {
        do {
            do {
                r = nextInt_71ad0(Globals::rnd, 0x25);
            } while ((unsigned) ((r & ~4u) - 9) < 2);
        } while (((r < 0x10) && ((1u << r & 0x8101u) != 0)) ||
                 (gRaceTable[r] != t));
    }
    return r;
}


void Globals::drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY,
                        unsigned int rightX, bool centered) {
    int *cv = (int *) &Globals::Canvas;
    LineMetrics **lh = (LineMetrics **) &Globals::layout;
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->size(); i++) {
        if (!centered) {
            int w = ((PaintCanvas *) (long) *cv)->GetTextWidth(font, *(*lines)[i]);
            dx = (int) rightX - w;
        }
        ((PaintCanvas *) (long) *cv)->DrawString(font, *(*lines)[i], dx + baseX, yacc, false);
        yacc += (*lh)->lineHeight;
    }
}


unsigned int Globals::createBillBoard(int p1, int height, float u0, float v0, float u1, float v1,
                                      int width) {
    (void) p1;    int *canvasP = (int *) &Globals::Canvas;
    unsigned int meshOut = 0;
    ((PaintCanvas *) (long) *canvasP)->MeshCreate((unsigned short) 0xc, (unsigned short) 6, (signed char) 0x13,
                                                  (unsigned short) 0, meshOut);
    int mesh = (int) meshOut;
    int cv = *canvasP;

    PaintCanvas *meshCanvas = (PaintCanvas *) (long) cv;
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 0, 0, 1, 2);
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 1, 2, 1, 3);
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 2, 4, 5, 6);
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 3, 6, 5, 7);
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 4, 8, 9, 10);
    meshCanvas->MeshSetTriangle((unsigned int) mesh, 5, 10, 9, 0xb);

    meshCanvas->MeshSetUv((unsigned int) mesh, 0, u0, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 1, u1, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 2, v1, u0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 3, v1, v0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 4, u0, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 5, u1, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 6, v1, u0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 7, v1, v0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 8, u0, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 9, u1, 0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 10, v1, u0);
    meshCanvas->MeshSetUv((unsigned int) mesh, 0xb, v1, v0);

    float pw = static_cast<float>(width);
    float nh = static_cast<float>(-height);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 0, nh, 0, pw);
    float nw = static_cast<float>(-width);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 1, nh, 0, nw);
    float ph = static_cast<float>(height);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 2, ph, 0, pw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 3, ph, 0, nw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 4, 0, nh, pw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 5, 0, nh, nw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 6, 0, ph, pw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 7, 0, ph, nw);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 8, nh, ph, 0);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 9, nh, nh, 0);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 10, ph, ph, 0);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 0xb, ph, nh, 0);

    return meshOut;
}

void BoundingAAB_ctor(void *self, float x0, float y0, float z0, float x1, float y1, float z1); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

// lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)


Array<BoundingVolume *> *Globals::getWreckCollision(int kind, AEGeometry *geom) {

    FileRead *fr = (FileRead *) ::operator new(1);
    FileRead_ctor(fr);
    Array<int> *data = fr->loadWreckCollision(kind);
    ::operator delete(FileRead_dtor(fr));

    Array<BoundingVolume *> *outArr = nullptr;
    if (data != 0) {
        int count = (*data)[0];

        float v[3] = {0, 0, 0};
        float c[3] = {0, 0, 0};

        outArr = new Array<BoundingVolume *>();
        ArraySetLength((unsigned) count, *outArr);

        int pos = 1;
        for (int i = 0; i < count; i++) {
            int *base = data->data();
            int kindWord = base[pos];
            BoundingVolume *bound = nullptr;
            if (kindWord == 1) {
                int *rec = base + pos;
                c[2] = (float) (-base[pos + 1]);
                c[0] = (float) rec[2];
                c[1] = (float) rec[3];
                v[0] = (float) rec[4];
                v[1] = (float) rec[5];
                v[2] = (float) rec[6];
                float mag = VectorScale(v, c[1]);
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = (BoundingVolume *) ::operator new(0x2c);
                BoundingAAB_ctor(bound, v[2] + v[2], c[1] + c[1], v[1] + v[1], c[2], c[0], v[0]);
                pos += 7;
            } else if (kindWord == 0) {
                int *rec = base + pos;
                c[2] = (float) (-base[pos + 1]);
                c[0] = (float) rec[2];
                c[1] = (float) rec[3];
                v[0] = (float) rec[4];
                VectorScale(v, v[0]);
                float mag = v[0];
                if (v[0] < 0.0f) {
                    mag = VectorScale(v, v[0]);
                }
                if (geom == 0) {
                    mag = VectorScale(c, mag);
                    VectorScale(v, mag);
                }
                bound = (BoundingVolume *) ::operator new(0x48);
                new(bound) BoundingSphere(c[2], c[1], c[0], 0.0f, 0.0f, 0.0f, v[0]);
                pos += 5;
            } else {
                pos += 1;
                continue;
            }
            (*outArr)[i] = bound;
        }

        ArrayRelease(*data);
        delete data;
    }

    return outArr;
}

// Globals::Globals() below zero-initializes the Globals static members directly
// (the original reads each via a GOT-indirect load, which &Global compiles to in
// PIC). Previously stubbed as null gGC_p_* placeholder pointers, which folded the
// constructor to a 2-byte stub. Each placeholder->member mapping was verified
// against the original via Ghidra (recovered symbol names + plate comments).

Globals::Globals() {
    Globals *self = this;
    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    CtorSecondaryObject *secondary = (CtorSecondaryObject *) Globals::hints;
    int *p5 = (int *) &Globals::recordHandler;
    int *p7 = &Globals::lastStationMusicPlayed;
    int *p9 = &Globals::lastCampaignMissionFailCount;
    int *p11 = &Globals::lastSpaceMusicPlayed;
    int *p12 = &Globals::lastCampaignMissionFailed;

    Globals::Canvas = 0;
    Globals::shipTemplate = 0;
    Globals::gameText = 0;
    Globals::font = 0;
    Globals::fontAlien = 0;
    Globals::fontLangSelect = 0;
    Globals::rnd = 0;
    Globals::globals = 0;
    Globals::layout = 0;
    Globals::appManager = 0;
    *p5 = 0;
    Globals::generator = 0;

    *(int *) self = 0;

    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    reinterpret_cast<int16_t &>(settings->_region[0]) = 0x101;
    reinterpret_cast<int16_t &>(settings->_region[4]) = 0;

    settings->glowR = 0.5f;
    settings->glowG = 0.5f;

    settings->tintR = 0.6f;
    settings->tintG = 0.6f;
    settings->tintB = 0.5f;
    settings->brightness = 1.0f;
    settings->contrast = 0.5f;
    settings->_region[2] = 1;
    settings->enableFlag30 = 1;
    settings->shortAt40 = 0;
    settings->intAt50 = 0;
    settings->flagAt39 = 0;
    settings->intAt35 = 0;
    settings->intAt31 = 0;
    secondary->slot1 = 0;
    secondary->flagAt13 = 0;

    settings->flagAt3f = 0;
    settings->intAt3b = 0;
    settings->flagAt4e = 0;
    settings->shortAt4c = 0;
    settings->intAt48 = 0;
    secondary->slot0 = 0;

    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    *p11 = -1;
    Globals::initMemoryWarning = 0;
    *p7 = -1;
    *p9 = 0;
    *p12 = -1;

    int *p7b = (int *) &Globals::imageFactory;
    int *p5b = (int *) &Globals::achievements;

    float fv = settings->qualityLevel;
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    settings->resWidth = v54;

    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    settings->resHeight = v58;

    int *p11b = (int *) &Globals::galaxy;
    Globals::instantActionScore = 0;
    Globals::instantActionWave = 0;
    int *p9b = (int *) &Globals::status;
    Globals::instantActionType = -1;
    Globals::instantActionPlayerName = 0;
    Globals::items = 0;
    *p9b = 0;
    settings->shortAt60 = 0x100;
    *p11b = 0;
    int *p8b = (int *) &Globals::ships;
    *p5b = 0;
    *p7b = 0;
    *p8b = 0;
    self->soundResources = 0;
}

Globals::~Globals() {
    if (Globals::recordHandler != nullptr)
        Globals::recordHandler->saveOptions();
    delete Globals::galaxy;
    Globals::galaxy = nullptr;
    delete Globals::status;
    Globals::status = nullptr;
    delete Globals::gameText;
    Globals::gameText = nullptr;
    delete Globals::rnd;
    Globals::rnd = nullptr;
    delete Globals::layout;
    Globals::layout = nullptr;
    delete Globals::generator;
    Globals::generator = nullptr;
    delete Globals::recordHandler;
    Globals::recordHandler = nullptr;
    delete Globals::instantActionPlayerName;
    Globals::instantActionPlayerName = nullptr;
    delete Globals::sound;
    Globals::sound = nullptr;
    if (Globals::items != nullptr) {
        ArrayReleaseClasses(*Globals::items);
        delete Globals::items;
    }
    Globals::items = nullptr;
    if (Globals::ships != nullptr) {
        ArrayReleaseClasses(*Globals::ships);
        delete Globals::ships;
    }
    Globals::ships = nullptr;
    delete Globals::galaxy;
    Globals::galaxy = nullptr;
    delete Globals::achievements;
    Globals::achievements = nullptr;
    delete Globals::status;
    Globals::status = nullptr;
    delete Globals::imageFactory;
    Globals::imageFactory = nullptr;
    if (this->soundResources != nullptr) {
        ArrayRelease(*this->soundResources);
        delete this->soundResources;
    }
    this->soundResources = nullptr;
    Globals::globals = nullptr;
}


void Globals::drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY,
                        bool centered) {
    int *cv = (int *) &Globals::Canvas;
    LineMetrics **lh = (LineMetrics **) &Globals::layout;
    int yacc = startY;
    int dx = 0;
    for (unsigned i = 0; i < lines->size(); i++) {
        if (centered) {
            int w = ((PaintCanvas *) (long) *cv)->GetTextWidth(font, *(*lines)[i]);
            dx = -(w >> 1);
        }
        ((PaintCanvas *) (long) *cv)->DrawString(font, *(*lines)[i], dx + baseX, yacc, false);
        yacc += (*lh)->lineHeight;
    }
}

static const float gSCF_b90 = 56.25f;
static const float gSCF_b10 = 63.28125f;
static const float gSCF_b14 = 210.9375f;
static const float gSCF_b7c = 63.28125f;
static const float gSCF_b80 = 90.0f;
static const float gSCF_b84 = 45.0f;
static const float gSCF_b88 = 300.0f;
static const float gSCF_b8c = 150.0f;
static const float gSCF_b94 = 80.0f;
static const float gSCF_b98 = 40.0f;
static const float gSCF_b9c = 107.28125f;
static const float gSCF_ba0 = 5.625f;
static const float gSCF_ba4 = 18.09375f;
static const float gSCF_ba8 = 19.28125f;
static const float gSCF_bac = 112.5f;
static const float gSCF_bb0 = 94.21875f;
static const float gSCF_bb4 = 152.0f;
static const float gSCF_bb8 = 77.0f;
static const float gSCF_bbc = 160.0f;
static const float gSCF_bc0 = 80.0f;
static const float gSCF_bc4 = 134.0f;
static const float gSCF_bc8 = 67.0f;
static const float gSCF_bcc = 100.0625f;
static const float gSCF_bd0 = 209.53125f;
static const float gSCF_bd4 = 144.0f;
static const float gSCF_bd8 = 74.0f;
static const float gSCF_bdc = 298.0f;
static const float gSCF_be0 = 149.0f;
static const float gSCF_be4 = 132.1875f;
static const float gSCF_be8 = 0.0f;
static const float gSCF_bec = 189.0f;
static const float gSCF_bf0 = 94.0f;


static inline char rf(char *const g) { return *g; }
static inline int rint(int *const g) { return *g; }
static inline int *robj(int *const g) { return g; }
static inline CoordsObject *rcoords(CoordsObject *const g) { return g; }

static inline unsigned short clampU(float v) {
    return (unsigned short) ((0.0f < v) ? (short) (int) v : 0);
}

void Globals::setCoordsFire(int p1, int p2, unsigned p3, unsigned p4,
                            unsigned &o5, unsigned short &o6, unsigned short &o7,
                            unsigned short &o8, unsigned short &o9, unsigned short &o10,
                            unsigned short &o11, unsigned short &o12, unsigned short &o13,
                            unsigned short &o14, unsigned short &o15, unsigned short &o16,
                            unsigned short &o17) {
    char isPhone = rf((char *) &Globals::iPadHD);
    float fbase = gSCF_b14;
    float fwid = gSCF_b10;
    if (isPhone == 0) {
        bool b = (rf((char *) &Globals::iPadLarge) == 0);
        fwid = b ? gSCF_b84 : gSCF_b80;
        fbase = b ? gSCF_b8c : gSCF_b88;
    }

    int colDelta = rint((int *) &Globals::h) - p2;
    float fcol = static_cast<float>(colDelta);
    float fp1 = static_cast<float>(p1);
    float fmax = fp1;
    if (fwid + fcol < fp1) {
        fmax = gSCF_b7c;
        if (isPhone == 0) {
            fmax = (rf((char *) &Globals::iPadLarge) == 0) ? gSCF_b84 : gSCF_b80;
        }
        fmax = fmax + fcol;
    }

    int iv;
    float adj13 = gSCF_b90;
    float wField;
    if (fbase < fmax) {
        float t = gSCF_b7c;
        if (isPhone == 0) {
            t = (rf((char *) &Globals::iPadLarge) == 0) ? gSCF_b84 : gSCF_b80;
        }
        float fp1b = fp1;
        if (t + fcol < fp1) {
            fp1b = gSCF_b7c;
            if (isPhone == 0) {
                fp1b = (rf((char *) &Globals::iPadLarge) == 0) ? gSCF_b84 : gSCF_b80;
            }
            fp1b = fp1b + fcol;
        }
        wField = static_cast<float>(rint((int *) &Globals::w) - p2);
        iv = (int) fp1b;
        rcoords((CoordsObject *) Globals::options)->fireValue = (int) fp1b;
        adj13 = gSCF_b90;
        if (isPhone == 0) {
            char flag6 = rf((char *) &Globals::iPadLarge);
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        }
    } else {
        if (isPhone == 0) {
            wField = static_cast<float>(rint((int *) &Globals::w) - p2);
            char flag6 = rf((char *) &Globals::iPadLarge);
            iv = (flag6 == 0) ? 0x96 : 0x12c;
            rcoords((CoordsObject *) Globals::options)->fireValue = iv;
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        } else {
            iv = 0xd2;
            wField = static_cast<float>(rint((int *) &Globals::w) - p2);
            rcoords((CoordsObject *) Globals::options)->fireValue = 0xd2;
        }
    }

    o6 = clampU(wField + adj13);
    o7 = (unsigned short) iv;
    o8 = (unsigned short) (o6 + (short) (p2 >> 1));
    o9 = (unsigned short) (o7 + (short) (p2 >> 1));

    float tail;
    unsigned short u16;
    if (isPhone == 0) {
        char flag6 = rf((char *) &Globals::iPadLarge);
        float a10 = (flag6 == 0) ? gSCF_bb8 : gSCF_bb4;
        o10 = clampU(a10 + static_cast<float>(o6));
        float a11 = (flag6 == 0) ? 4.0f : 8.0f;
        o11 = clampU(a11 + static_cast<float>(o7));
        float a12 = (flag6 == 0) ? 15.0f : 28.0f;
        o12 = clampU(a12 + static_cast<float>(o6));
        float a13 = (flag6 == 0) ? 13.0f : 27.0f;
        o13 = clampU(a13 + static_cast<float>(o7));
        float a16 = (flag6 == 0) ? gSCF_bc0 : gSCF_bbc;
        u16 = clampU(a16 + static_cast<float>(o6));
        tail = (flag6 == 0) ? gSCF_bc8 : gSCF_bc4;
    } else {
        o10 = clampU(static_cast<float>(o6) + gSCF_b9c);
        o11 = clampU(static_cast<float>(o7) + gSCF_ba0);
        o12 = clampU(static_cast<float>(o6) + gSCF_ba4);
        o13 = clampU(static_cast<float>(o7) + gSCF_ba8);
        u16 = clampU(static_cast<float>(o6) + gSCF_bac);
        tail = gSCF_bb0;
    }

    o16 = u16;
    o17 = clampU(static_cast<float>(o7) - tail);

    unsigned short u14;
    float t15;
    if (colDelta < iv) {
        o5 = p3;
        float w = static_cast<float>(o6);
        if (isPhone != 0) {
            u14 = clampU(w + -2.0f);
            t15 = gSCF_be4;
        } else {
            char flag = rf((char *) &Globals::iPadLarge);
            float a = (flag == 0) ? gSCF_be8 : -2.0f;
            u14 = clampU(a + gSCF_be4);
            t15 = (flag == 0) ? gSCF_bf0 : gSCF_bec;
        }
    } else {
        o5 = p4;
        float w = static_cast<float>(o6);
        if (isPhone != 0) {
            u14 = clampU(w + gSCF_bcc);
            t15 = gSCF_bd0;
        } else {
            char flag = rf((char *) &Globals::iPadLarge);
            float a = (flag == 0) ? gSCF_bd8 : gSCF_bd4;
            u14 = clampU(a + gSCF_bd0);
            t15 = (flag == 0) ? gSCF_be0 : gSCF_bdc;
        }
    }

    o14 = u14;
    o15 = clampU(t15 + static_cast<float>(o7));
}


void Globals::releaseResources() {
    Globals::Canvas->ReleaseAllResources();
    Globals::layout->reload();
}



static inline char flag(char *const g) { return *g; }

void Globals::loadFont(int language) {
    // The font handles are stored in pointer-typed statics shared with other TUs.
    unsigned int &mainFont = reinterpret_cast<unsigned int &>(Globals::font);
    unsigned int &alienFont = reinterpret_cast<unsigned int &>(Globals::fontAlien);
    unsigned int &langSelectFont = reinterpret_cast<unsigned int &>(Globals::fontLangSelect);
    bool latinFont = true;
    switch (language) {
        case 9:
            Globals::Canvas->FontCreate(0x2d74, mainFont, true);
            Globals::Canvas->FontSetSpacing(mainFont,
                                            Globals::iPadHD ? -6 : (Globals::retinaDisplay ? -8 : -4));
            latinFont = false;
            break;
        case 10:
            Globals::Canvas->FontCreate(0x2d78, mainFont, true);
            Globals::Canvas->FontSetSpacing(mainFont,
                                            Globals::iPadHD ? -6 : (Globals::retinaDisplay ? -8 : -4));
            break;
        case 11:
            Globals::Canvas->FontCreate(0x2d76, mainFont, true);
            Globals::Canvas->FontSetSpacing(mainFont,
                                            Globals::iPadHD ? -6 : (Globals::retinaDisplay ? -8 : -4));
            break;
        case 14:
            Globals::Canvas->FontCreate(0x2d7c, mainFont, true);
            Globals::Canvas->FontSetSpacing(mainFont,
                                            Globals::iPadHD ? -6 : (Globals::retinaDisplay ? -8 : -4));
            break;
        default:
            if (language == 15) {
                Globals::Canvas->FontCreate(0x2d7e, mainFont, true);
                Globals::Canvas->FontSetSpacing(mainFont, Globals::iPadHD ? -7 : (Globals::retinaDisplay ? -10 : -5));
            } else {
                Globals::Canvas->FontCreate(0x457, mainFont, true);
                Globals::Canvas->FontSetSpacing(mainFont, Globals::retinaDisplay ? -5
                                                          : Globals::n9 ? -4
                                                          : Globals::iPadHD ? -4 : -2);
            }
            break;
    }
    Globals::Canvas->field_0x1c = latinFont;
    Globals::Canvas->FontCreate(0x51e, alienFont, true);
    Globals::Canvas->FontSetSpacing(alienFont, 0);
    Globals::Canvas->FontCreate(0x2d7a, langSelectFont, true);
    Globals::Canvas->FontSetSpacing(langSelectFont, 0);
}

// Globals::init below accesses these engine globals directly (Mission::empty,
// Globals::options/items/ships/Canvas/globals/generator/recordHandler/sound/
// layout/imageFactory and the init flag bytes); the original reads each via a
// GOT-indirect load, which &Global compiles to in PIC. (Previously stubbed as
// null gI_* placeholder pointers, which folded init to a 2-byte stub.)

int Globals::init(AbyssEngine::ApplicationManager *app, AbyssEngine::Engine *engine) {
    (void) engine;
    int *missionSlot = (int *) &Mission::empty;
    if (*missionSlot == 0) {
        Mission *m = new Mission();
        *missionSlot = (int) (long) m;
    }

    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    int *flagFFFF = (int *) &Globals::lastRecordWritten;
    int *langSettingSlot = &Globals::recordSlots;
    char *langFlag = (char *) &Globals::iPad;
    char *zeroByte = (char *) &Globals::startLiteVersionWithMoreCredits;

    char lang = *langFlag;
    settings->_region[5] = 1;
    settings->enableFlag30 = 1;
    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    settings->tintB = 0.5f;
    settings->brightness = 1.0f;
    settings->glowR = 1.0f;
    settings->glowG = 1.0f;
    settings->tintR = 0.6f;
    settings->tintG = 0.6f;
    reinterpret_cast<int16_t &>(settings->_region[3]) = 0x101;
    *zeroByte = 0;
    *flagFFFF = -1;
    *langSettingSlot = (lang == 0) ? 6 : 0xc;

    Galaxy *galaxy = new Galaxy();
    Globals::galaxy = galaxy;
    Achievements *ach = new Achievements();
    Globals::achievements = ach;
    Status *status = new Status();
    Globals::status = status;
    ImageFactory *imgFac = new ImageFactory();
    Globals::imageFactory = imgFac;

    FileRead *fr = new FileRead();
    Globals::items = (Array<Item *> *) (long) fr->loadItemsBinary();
    Globals::ships = (Array<Ship *> *) (long) fr->loadShipsBinary();
    delete fr;

    int *engineSlot = (int *) &Globals::Canvas;
    if (*engineSlot == 0) {
        *engineSlot = *reinterpret_cast<int *>(app);
    }
    Globals::appManager = app;
    app->VibrateEnable(0);

    AbyssEngine::AERandom *rng = new AbyssEngine::AERandom();
    Globals::globals = this;
    Globals::rnd = rng;

    Generator *gen = new Generator();
    Globals::generator = gen;

    RecordHandler *rh = new RecordHandler();
    RecordHandler **rhSlotP = &Globals::recordHandler;
    *rhSlotP = rh;
    Globals::status->resetGame();
    (*rhSlotP)->loadOptions();

    FModSound *fmod = new FModSound();
    FModSound **fmodSlotP = &Globals::sound;
    *fmodSlotP = fmod;
    fmod->init();

    (*fmodSlotP)->enableCategory(1, settings->_region[1]);
    (*fmodSlotP)->enableCategory(2, settings->_region[0]);
    (*fmodSlotP)->enableCategory(3, settings->_region[2]);
    (*fmodSlotP)->setVolume(1, settings->colorR);
    (*fmodSlotP)->setVolume(2, settings->colorG);
    (*fmodSlotP)->setVolume(3, settings->colorB);

    if (Globals::sound->tryToStopMusicForBGMusic() != 0) {
        settings->_region[1] = 0;
    }

    Globals::instantActionPoints = 0;
    Globals::first_start_ever = true;
    InitZeroObject *obj = (InitZeroObject *) Globals::hints;
    obj->slots[0] = 0;
    obj->slots[1] = 0;
    obj->slots[2] = 0;
    obj->slots[3] = 0;
    obj->slots[4] = 0;
    obj->slots[5] = 0;
    obj->slots[6] = 0;
    obj->slots[7] = 0;
    obj->slots[8] = 0;
    obj->slots[9] = 0;
    obj->slots[10] = 0;
    obj->slots[11] = 0;
    reinterpret_cast<int32_t &>(reinterpret_cast<uint8_t *>(obj->slots)[0x2b]) = 0;
    reinterpret_cast<int32_t &>(reinterpret_cast<uint8_t *>(obj->slots)[0x2f]) = 0;
    reinterpret_cast<int32_t &>(reinterpret_cast<uint8_t *>(obj->slots)[0x33]) = 0;
    reinterpret_cast<int32_t &>(reinterpret_cast<uint8_t *>(obj->slots)[0x37]) = 0;
    InitFlagByte *flagByteObj = (InitFlagByte *) &Globals::gameLoaded;
    flagByteObj->flag = 0;
    Globals::gameSaving = 0;
    Globals::initMemoryWarning = 0;

    Layout *layout = new Layout();
    Globals::layout = layout;
    layout->reload();
    ParticleSettingsRef::initialize();

    Array<int> *arr = new Array<int>();
    this->soundResources = arr;
    return (int) (long) arr;
}

static const int gStationMusicByRace[4] = {128, 127, 129, 130};
static const int gSpaceMusicByRace[4] = {134, 139, 138, 137};

void Globals::playMusicAndFadeOutCurrent(int mode) {
    switch (mode) {
        case 0: {
            int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            Globals::sound->stop(Globals::sound->currentMusicEvent);
            if (Globals::status->getStation()->getIndex() == 108) {
                Globals::sound->play(132, nullptr, nullptr, 0.0f);
            } else if (Globals::status->getStation()->getIndex() == 101) {
                Globals::sound->play(131, nullptr, nullptr, 0.0f);
            } else if (Globals::status->getStation()->getIndex() == 10 ||
                       Globals::status->getStation()->getIndex() == 100) {
                if (Globals::status->getStation()->getIndex() == 10 &&
                    Globals::status->getCurrentCampaignMission() == 159)
                    Globals::sound->play(144, nullptr, nullptr, 0.0f);
                else
                    Globals::sound->play(133, nullptr, nullptr, 0.0f);
            } else {
                int track = gStationMusicByRace[race];
                Globals::sound->play(track, nullptr, nullptr, 0.0f);
            }
            break;
        }
        case 1: {
            if (Globals::status->inAlienOrbit()) {
                Globals::sound->stop(Globals::sound->currentMusicEvent);
                Globals::sound->play(Globals::status->getCurrentCampaignMission() >= 147 &&
                                     Globals::status->getCurrentCampaignMission() < 154 ? 145 : 136,
                                     nullptr, nullptr, 0.0f);
                break;
            }
            int race = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace();
            Globals::sound->stop(Globals::sound->currentMusicEvent);
            if (Globals::status->getStation()->getIndex() == 108) {
                Globals::sound->play(146, nullptr, nullptr, 0.0f);
            } else if (Globals::status->getStation()->getIndex() == 101) {
                Globals::sound->play(147, nullptr, nullptr, 0.0f);
            } else if (Globals::status->inSupernovaSystem()) {
                if (Globals::status->getCurrentCampaignMission() == 89) {
                    Globals::sound->play(2238, nullptr, nullptr, 0.0f);
                } else if (Globals::status->getMission() != nullptr && !Globals::status->getMission()->isEmpty() &&
                           Globals::status->getMission()->getTargetStation() ==
                           Globals::status->getStation()->getIndex()) {
                    Globals::sound->play(Globals::status->getCurrentCampaignMission() < 106 ? 2241 : 2242, nullptr,
                                         nullptr, 0.0f);
                } else {
                    Globals::sound->play(148, nullptr, nullptr, 0.0f);
                }
            } else if (Globals::status->inDeepScienceOrbit()) {
                Globals::sound->play(152, nullptr, nullptr, 0.0f);
            } else if (Globals::status->getStation()->getIndex() == 120 &&
                       (Globals::status->getCurrentCampaignMission() == 126 ||
                        Globals::status->getCurrentCampaignMission() == 133)) {
                Globals::sound->play(2239, nullptr, nullptr, 0.0f);
            } else {
                int track = gSpaceMusicByRace[race];
                Globals::sound->play(track, nullptr, nullptr, 0.0f);
            }
            break;
        }
        case 2:
            Globals::sound->stop(Globals::sound->currentMusicEvent);
            Globals::sound->play(145, nullptr, nullptr, 0.0f);
            break;
    }
}

// (text id, sound event id) pairs for scripted dialogue lines.
static const int gGDS_pairTable[3008] = {
    1795, 163, 1819, 164, 1836, 165, 1837, 166, 1877, 167, 1897, 168,
    1913, 169, 1914, 170, 1915, 171, 1916, 172, 1928, 173, 1929, 174,
    1930, 175, 1947, 176, 1721, 177, 1722, 178, 1723, 179, 1724, 180,
    1970, 181, 2016, 182, 2017, 183, 2028, 184, 2049, 185, 2050, 186,
    2051, 187, 1738, 188, 1748, 189, 1783, 190, 1784, 191, 1793, 192,
    1794, 193, 1785, 194, 1786, 195, 1787, 196, 1788, 197, 1789, 198,
    1790, 199, 1791, 200, 1792, 201, 1796, 202, 1797, 203, 1806, 204,
    1807, 205, 1808, 206, 1798, 207, 1799, 208, 1800, 209, 1801, 210,
    1802, 211, 1803, 212, 1804, 213, 1805, 214, 1809, 215, 1810, 216,
    1811, 217, 1812, 218, 1813, 219, 1814, 220, 1815, 221, 1817, 222,
    1818, 223, 1825, 224, 1826, 225, 1835, 226, 1827, 227, 1828, 228,
    1829, 229, 1830, 230, 1831, 231, 1832, 232, 1833, 233, 1834, 234,
    1843, 235, 1844, 236, 1845, 237, 1846, 238, 1847, 239, 1856, 240,
    1857, 241, 1858, 242, 1859, 243, 1860, 244, 1861, 245, 1848, 246,
    1849, 247, 1850, 248, 1851, 249, 1852, 250, 1853, 251, 1854, 252,
    1855, 253, 1863, 254, 1864, 255, 1865, 256, 1866, 257, 1867, 258,
    1868, 259, 1869, 260, 1870, 261, 1871, 262, 1872, 263, 1873, 264,
    1874, 265, 1875, 266, 1700, 267, 1701, 268, 1710, 269, 1711, 270,
    1712, 271, 1713, 272, 1714, 273, 1715, 274, 1716, 275, 1717, 276,
    1702, 277, 1703, 278, 1704, 279, 1705, 280, 1706, 281, 1707, 282,
    1708, 283, 1709, 284, 1876, 285, 1886, 286, 1887, 287, 1888, 288,
    1889, 289, 1890, 290, 1891, 291, 1892, 292, 1893, 293, 1894, 294,
    1895, 295, 1898, 296, 1899, 297, 1908, 298, 1909, 299, 1910, 300,
    1911, 301, 1900, 302, 1901, 303, 1902, 304, 1903, 305, 1904, 306,
    1905, 307, 1906, 308, 1907, 309, 1922, 310, 1923, 311, 1924, 312,
    1931, 313, 1932, 314, 1933, 315, 1934, 316, 1943, 317, 1935, 318,
    1936, 319, 1937, 320, 1938, 321, 1939, 322, 1940, 323, 1941, 324,
    1942, 325, 1954, 326, 1728, 327, 1729, 328, 1955, 329, 1956, 330,
    1957, 331, 1958, 332, 1959, 333, 1960, 334, 1961, 335, 1962, 336,
    1963, 337, 1964, 338, 1965, 339, 1966, 340, 1967, 341, 1968, 342,
    1969, 343, 1971, 344, 1972, 345, 1981, 346, 1973, 347, 1974, 348,
    1975, 349, 1976, 350, 1977, 351, 1978, 352, 1979, 353, 1980, 354,
    1982, 355, 1983, 356, 1984, 357, 1985, 358, 1986, 359, 1987, 360,
    1988, 361, 1989, 362, 1990, 363, 1991, 364, 1992, 365, 2001, 366,
    2002, 367, 2003, 368, 2004, 369, 2005, 370, 1993, 371, 1994, 372,
    1995, 373, 1996, 374, 1997, 375, 1998, 376, 1999, 377, 2000, 378,
    2006, 379, 2007, 380, 2008, 381, 2009, 382, 2010, 383, 2011, 384,
    2012, 385, 2013, 386, 2014, 387, 2015, 388, 2018, 389, 2019, 390,
    2020, 391, 2021, 392, 2022, 393, 2023, 394, 2024, 395, 2025, 396,
    2026, 397, 2027, 398, 2030, 399, 2031, 400, 2032, 401, 2033, 402,
    2034, 403, 2035, 404, 2036, 405, 2037, 406, 2038, 407, 2039, 408,
    2040, 409, 2041, 410, 1733, 411, 1734, 412, 1735, 413, 1736, 414,
    1737, 415, 2058, 416, 2059, 417, 2060, 418, 2061, 419, 2062, 420,
    2065, 421, 2066, 422, 2067, 423, 2068, 424, 2069, 425, 2070, 426,
    2075, 427, 2076, 428, 2077, 429, 2078, 430, 1739, 431, 1740, 432,
    1741, 433, 1742, 434, 1743, 435, 1744, 436, 1745, 437, 1747, 438,
    1758, 439, 1759, 440, 1760, 441, 1761, 442, 1762, 443, 1763, 444,
    1764, 445, 1765, 446, 1766, 447, 1767, 448, 1768, 449, 1769, 450,
    1770, 451, 1771, 452, 1772, 453, 1773, 454, 1774, 455, 1775, 456,
    1776, 457, 1777, 458, 1778, 459, 1779, 460, 1780, 461, 1781, 462,
    1912, 463, 533, 464, 534, 465, 532, 466, 535, 467, 536, 468,
    1668, 469, 1669, 470, 1684, 471, 1685, 472, 1686, 473, 1687, 474,
    1688, 475, 1689, 476, 1690, 477, 1691, 478, 1692, 479, 1693, 480,
    1670, 481, 1694, 482, 1695, 483, 1696, 484, 1671, 485, 1672, 486,
    1673, 487, 1674, 488, 1675, 489, 1676, 490, 1683, 491, 1820, 492,
    1821, 493, 1822, 494, 1823, 495, 1824, 496, 1839, 497, 1840, 498,
    1841, 499, 1842, 500, 1697, 501, 1698, 502, 1699, 503, 1881, 504,
    1882, 505, 1883, 506, 1884, 507, 1885, 508, 1918, 509, 1919, 510,
    1920, 511, 1921, 512, 1925, 513, 1926, 514, 1927, 515, 1944, 516,
    1946, 517, 1948, 518, 1949, 519, 1950, 520, 1951, 521, 1953, 522,
    2029, 523, 2042, 524, 2043, 525, 2044, 526, 2045, 527, 2046, 528,
    2047, 529, 2048, 530, 2052, 531, 2053, 532, 2054, 533, 2055, 534,
    2056, 535, 2057, 536, 2063, 537, 2064, 538, 2071, 539, 2072, 540,
    2073, 541, 2074, 542, 1756, 543, 1757, 544, 1952, 545, 1838, 546,
    1917, 547, 1945, 548, 443, 584, 444, 585, 433, 587, 2106, 1124,
    2108, 1126, 2110, 1128, 2185, 1130, 2187, 1132, 2189, 1134, 2241, 1136,
    2243, 1139, 2245, 1141, 2247, 1143, 2249, 1145, 2283, 1146, 2285, 1148,
    2079, 1150, 2081, 1152, 2083, 1154, 2085, 1156, 2087, 1158, 2089, 1160,
    2091, 1168, 2093, 1170, 2095, 1172, 2097, 1174, 2099, 1162, 2101, 1164,
    2103, 1166, 2112, 1176, 2114, 1178, 2116, 1180, 2118, 1182, 2130, 1184,
    2132, 1186, 2134, 1188, 2136, 1190, 2140, 1192, 2142, 1198, 2144, 1200,
    2146, 1202, 2148, 1204, 2150, 1194, 2152, 1196, 2160, 1206, 2162, 1208,
    2164, 1210, 2166, 1212, 2168, 1214, 2170, 1216, 2174, 1218, 2176, 1221,
    2178, 1223, 2180, 1225, 2182, 1227, 2192, 1231, 2194, 1232, 2196, 1234,
    2198, 1236, 2201, 1239, 2203, 1241, 2205, 1243, 2207, 1245, 2210, 1247,
    2212, 1252, 2214, 1254, 2216, 1256, 2218, 1258, 2220, 1249, 2222, 1251,
    2226, 1260, 2236, 1263, 2238, 1265, 2254, 1267, 2256, 1276, 2258, 1278,
    2260, 1280, 2262, 1282, 2264, 1269, 2266, 1271, 2268, 1273, 2271, 1284,
    2273, 1288, 2275, 1290, 2277, 1292, 2279, 1294, 2280, 1295, 2282, 1287,
    2300, 1297, 2302, 1299, 2304, 1301, 2308, 1304, 2310, 1306, 2312, 1308,
    2314, 1310, 2316, 1312, 2325, 1315, 2327, 1317, 2329, 1319, 2331, 1321,
    2333, 1323, 2335, 1330, 2337, 1332, 2339, 1334, 2341, 1336, 2343, 1325,
    2345, 1327, 2356, 1338, 2358, 1340, 2360, 1342, 2362, 1344, 2364, 1346,
    2367, 1349, 2369, 1354, 2371, 1356, 2373, 1358, 2375, 1360, 2377, 1351,
    2379, 1361, 2381, 1363, 2383, 1365, 2385, 1367, 2387, 1369, 2389, 1371,
    2391, 1383, 2393, 1385, 2395, 1387, 2397, 1389, 2398, 1390, 2399, 1373,
    2400, 1374, 2402, 1376, 2404, 1378, 2406, 1380, 2408, 1382, 2427, 1392,
    2429, 1394, 2431, 1396, 2436, 1397, 2438, 1399, 2440, 1401, 2442, 1403,
    2447, 1408, 2449, 1410, 2451, 1412, 2307, 1414, 2453, 1415, 2455, 1417,
    2457, 1419, 2459, 1421, 2461, 1423, 2463, 1425, 2464, 1426, 2465, 1427,
    2467, 1429, 2469, 1431, 2471, 1433, 2472, 1434, 449, 1436, 450, 1437,
    455, 1438, 456, 1439, 453, 1440, 454, 1441, 451, 1442, 452, 1443,
    459, 1444, 461, 1453, 463, 1455, 465, 1457, 467, 1459, 469, 1446,
    471, 1448, 473, 1450, 475, 1452, 479, 1461, 481, 1463, 483, 1465,
    458, 1467, 457, 1468, 2121, 1470, 2123, 1472, 2125, 1474, 2127, 1476,
    2154, 1481, 2156, 1483, 2159, 1486, 2225, 1490, 2229, 1492, 2231, 1494,
    2233, 1496, 2252, 1499, 2288, 1502, 2289, 1505, 2291, 1507, 2294, 1510,
    2296, 1512, 2298, 1504, 2322, 1517, 2349, 1521, 2352, 1524, 2412, 1530,
    2415, 1533, 2416, 1536, 2418, 1538, 2419, 1539, 2420, 1540, 2421, 1541,
    2423, 1543, 2424, 1534, 2425, 1535, 2432, 1544, 2433, 1545, 2434, 1546,
    2435, 1547, 2115, 1179, 2113, 1177, 2098, 1175, 2096, 1173, 2094, 1171,
    2092, 1169, 2104, 1167, 2102, 1165, 2100, 1163, 2090, 1161, 2430, 1395,
    2428, 1393, 2426, 1391, 2363, 1345, 2361, 1343, 2359, 1341, 2357, 1339,
    2355, 1337, 2340, 1335, 2346, 1328, 2344, 1326, 2342, 1324, 2338, 1333,
    2336, 1331, 2334, 1329, 2332, 1322, 2324, 1314, 2305, 1302, 2303, 1300,
    2301, 1298, 2299, 1296, 2239, 1266, 2237, 1264, 2235, 1262, 2227, 1261,
    2208, 1246, 2206, 1244, 2204, 1242, 2202, 1240, 2190, 1228, 2161, 1207,
    2088, 1159, 2086, 1157, 2084, 1155, 2082, 1153, 2080, 1151, 2286, 1149,
    2284, 1147, 2250, 1137, 2248, 1144, 2246, 1142, 2244, 1140, 2242, 1138,
    2240, 1135, 2473, 1435, 2470, 1432, 2468, 1430, 2466, 1428, 2462, 1424,
    2460, 1422, 2458, 1420, 2456, 1418, 2454, 1416, 2422, 1542, 2417, 1537,
    2414, 1532, 2413, 1531, 2411, 1529, 2410, 1528, 2409, 1527, 2354, 1526,
    2353, 1525, 2351, 1523, 2350, 1522, 2348, 1520, 2347, 1519, 2323, 1518,
    2321, 1516, 2320, 1515, 2319, 1514, 2318, 1513, 2297, 1503, 2295, 1511,
    2293, 1509, 2292, 1508, 2290, 1506, 2287, 1501, 2253, 1500, 2251, 1498,
    2234, 1497, 2232, 1495, 2230, 1493, 2228, 1491, 2224, 1489, 2223, 1488,
    2209, 1487, 2158, 1485, 2157, 1484, 2155, 1482, 2139, 1480, 2138, 1479,
    2129, 1478, 2128, 1477, 2126, 1475, 2124, 1473, 2122, 1471, 2120, 1469,
    2188, 1133, 2186, 1131, 2111, 1129, 2109, 1127, 2107, 1125, 2105, 1123,
    484, 1466, 482, 1464, 480, 1462, 468, 1460, 466, 1458, 464, 1456,
    474, 1451, 472, 1449, 470, 1447, 462, 1454, 460, 1445, 2452, 1413,
    2450, 1411, 2448, 1409, 2446, 1407, 2445, 1406, 2444, 1405, 2443, 1404,
    2441, 1402, 2439, 1400, 2437, 1398, 2396, 1388, 2394, 1386, 2407, 1381,
    2405, 1379, 2403, 1377, 2401, 1375, 2392, 1384, 2390, 1372, 2388, 1370,
    2386, 1368, 2384, 1366, 2382, 1364, 2380, 1362, 2378, 1352, 2376, 1350,
    2374, 1359, 2372, 1357, 2370, 1355, 2368, 1353, 2366, 1348, 2365, 1347,
    2330, 1320, 2328, 1318, 2326, 1316, 2317, 1313, 2315, 1311, 2313, 1309,
    2311, 1307, 2309, 1305, 2306, 1303, 2281, 1286, 2278, 1293, 2276, 1291,
    2274, 1289, 2272, 1285, 2270, 1275, 2269, 1274, 2267, 1272, 2265, 1270,
    2263, 1283, 2261, 1281, 2259, 1279, 2257, 1277, 2255, 1268, 2221, 1250,
    2219, 1259, 2217, 1257, 2215, 1255, 2213, 1253, 2211, 1248, 2200, 1238,
    2199, 1237, 2197, 1235, 2195, 1233, 2193, 1230, 2191, 1229, 2183, 1219,
    2181, 1226, 2179, 1224, 2177, 1222, 2175, 1220, 2173, 1217, 2169, 1215,
    2167, 1213, 2165, 1211, 2163, 1209, 2153, 1197, 2151, 1195, 2149, 1205,
    2147, 1203, 2145, 1201, 2143, 1199, 2141, 1193, 2137, 1191, 2135, 1189,
    2133, 1187, 2131, 1185, 2119, 1183, 2117, 1181, 3156, 1548, 3157, 1549,
    3158, 1550, 3159, 1551, 3160, 1552, 3168, 1553, 3169, 1554, 3170, 1555,
    3171, 1556, 3172, 1557, 3173, 1558, 3162, 1559, 3163, 1560, 2716, 1561,
    2717, 1562, 2718, 1563, 2719, 1564, 2474, 1565, 2475, 1566, 2476, 1567,
    2477, 1568, 2478, 1569, 2479, 1570, 2487, 1571, 2488, 1572, 2489, 1573,
    2490, 1574, 2491, 1575, 2492, 1576, 2493, 1577, 2495, 1578, 2496, 1579,
    2497, 1580, 2498, 1581, 2499, 1582, 2500, 1583, 2501, 1584, 2502, 1585,
    2503, 1586, 2523, 1587, 2524, 1588, 2525, 1589, 2526, 1590, 2527, 1591,
    2528, 1592, 2529, 1593, 2530, 1594, 2531, 1595, 2532, 1596, 2533, 1597,
    2534, 1598, 2535, 1599, 2536, 1600, 2537, 1601, 2538, 1602, 2539, 1603,
    2540, 1604, 2548, 1605, 2549, 1606, 2550, 1607, 2551, 1608, 2552, 1609,
    2553, 1610, 2554, 1611, 2556, 1612, 2557, 1613, 2558, 1614, 2559, 1615,
    2560, 1616, 2561, 1617, 2562, 1618, 2563, 1619, 2564, 1620, 2565, 1621,
    2566, 1622, 2567, 1623, 2568, 1624, 2569, 1625, 2570, 1626, 2571, 1627,
    2572, 1628, 2573, 1629, 2574, 1630, 2575, 1631, 2576, 1632, 2577, 1633,
    2578, 1634, 2579, 1635, 2580, 1636, 2581, 1637, 2585, 1638, 2586, 1639,
    2587, 1640, 2588, 1641, 2589, 1642, 2590, 1643, 2591, 1644, 2592, 1645,
    2593, 1646, 2594, 1647, 2595, 1648, 2596, 1649, 2597, 1650, 2598, 1651,
    2599, 1652, 2601, 1653, 2602, 1654, 2603, 1655, 2604, 1656, 2605, 1657,
    2606, 1658, 2607, 1659, 2608, 1660, 2609, 1661, 2610, 1662, 2611, 1663,
    2612, 1664, 2615, 1665, 2616, 1666, 2617, 1667, 2618, 1668, 2619, 1669,
    2620, 1670, 2621, 1671, 2622, 1672, 2623, 1673, 2624, 1674, 2634, 1675,
    2635, 1676, 2636, 1677, 2637, 1678, 2638, 1679, 2639, 1680, 2640, 1681,
    2641, 1682, 2642, 1683, 2643, 1684, 2644, 1685, 2645, 1686, 2646, 1687,
    2647, 1688, 2648, 1689, 2649, 1690, 2650, 1691, 2651, 1692, 2652, 1693,
    2653, 1694, 2654, 1695, 2655, 1696, 2656, 1697, 2672, 1698, 2673, 1699,
    2674, 1700, 2675, 1701, 2676, 1702, 2677, 1703, 2678, 1704, 2679, 1705,
    2680, 1706, 2681, 1707, 2682, 1708, 2683, 1709, 2684, 1710, 2685, 1711,
    2686, 1712, 2688, 1713, 2689, 1714, 2690, 1715, 2691, 1716, 2692, 1717,
    2693, 1718, 2694, 1719, 2695, 1720, 2696, 1721, 2697, 1722, 2698, 1723,
    2699, 1724, 2700, 1725, 2701, 1726, 2702, 1727, 2703, 1728, 2704, 1729,
    2705, 1730, 2706, 1731, 2707, 1732, 2711, 1733, 2712, 1734, 2713, 1735,
    2714, 1736, 2715, 1737, 2720, 1738, 2721, 1739, 2722, 1740, 2723, 1741,
    2724, 1742, 2725, 1743, 2726, 1744, 2727, 1745, 2728, 1746, 2729, 1747,
    2730, 1748, 2731, 1749, 2732, 1750, 2733, 1751, 2734, 1752, 2735, 1753,
    2736, 1754, 2737, 1755, 2738, 1756, 2739, 1757, 2740, 1758, 2741, 1759,
    2742, 1760, 2743, 1761, 2744, 1762, 2746, 1763, 2747, 1764, 2748, 1765,
    2749, 1766, 2750, 1767, 2751, 1768, 2752, 1769, 2753, 1770, 2754, 1771,
    2755, 1772, 2756, 1773, 2758, 1774, 2759, 1775, 2760, 1776, 2761, 1777,
    2762, 1778, 2763, 1779, 2764, 1780, 2765, 1781, 2766, 1782, 2767, 1783,
    2768, 1784, 2769, 1785, 2770, 1786, 2771, 1787, 2772, 1788, 2773, 1789,
    2774, 1790, 2775, 1791, 2776, 1792, 2777, 1793, 2778, 1794, 2779, 1795,
    2780, 1796, 2781, 1797, 2782, 1798, 2783, 1799, 2784, 1800, 2789, 1801,
    2790, 1802, 2791, 1803, 2792, 1804, 2793, 1805, 2794, 1806, 2795, 1807,
    2796, 1808, 2797, 1809, 2798, 1810, 2799, 1811, 2800, 1812, 2801, 1813,
    2802, 1814, 2803, 1815, 2822, 1816, 2823, 1817, 2824, 1818, 2825, 1819,
    2826, 1820, 2827, 1821, 2829, 1822, 2830, 1823, 2831, 1824, 2832, 1825,
    2833, 1826, 2834, 1827, 2835, 1828, 2836, 1829, 2837, 1830, 2838, 1831,
    2839, 1832, 2840, 1833, 2841, 1834, 2842, 1835, 2843, 1836, 2844, 1837,
    2845, 1838, 2846, 1839, 2847, 1840, 2848, 1841, 2849, 1842, 2850, 1843,
    2851, 1844, 2852, 1845, 2853, 1846, 2854, 1847, 2855, 1848, 2856, 1849,
    2857, 1850, 2858, 1851, 2860, 1852, 2861, 1853, 2862, 1854, 2863, 1855,
    2864, 1856, 2865, 1857, 2866, 1858, 2868, 1859, 2869, 1860, 2870, 1861,
    2871, 1862, 2872, 1863, 2873, 1864, 2874, 1865, 2875, 1866, 2876, 1867,
    2877, 1868, 2878, 1869, 2887, 1870, 2888, 1871, 2889, 1872, 2890, 1873,
    2891, 1874, 2892, 1875, 2893, 1876, 2894, 1877, 2900, 1878, 2901, 1879,
    2902, 1880, 2903, 1881, 2904, 1882, 2905, 1883, 2906, 1884, 2907, 1885,
    2908, 1886, 2909, 1887, 2910, 1888, 2911, 1889, 2912, 1890, 2913, 1891,
    2914, 1892, 2915, 1893, 2916, 1894, 2917, 1895, 2918, 1896, 2919, 1897,
    2920, 1898, 2921, 1899, 2922, 1900, 2923, 1901, 2929, 1902, 2930, 1903,
    2931, 1904, 2932, 1905, 2933, 1906, 2934, 1907, 2935, 1908, 2936, 1909,
    2937, 1910, 2938, 1911, 2939, 1912, 2940, 1913, 2941, 1914, 2942, 1915,
    2943, 1916, 2944, 1917, 2945, 1918, 2946, 1919, 2947, 1920, 2948, 1921,
    2949, 1922, 2950, 1923, 2951, 1924, 2957, 1925, 2958, 1926, 2959, 1927,
    2960, 1928, 2961, 1929, 2962, 1930, 2963, 1931, 2964, 1932, 2965, 1933,
    2966, 1934, 2967, 1935, 2975, 1936, 2976, 1937, 2977, 1938, 2978, 1939,
    2979, 1940, 2980, 1941, 2981, 1942, 2982, 1943, 2983, 1944, 2984, 1945,
    2985, 1946, 2986, 1947, 2987, 1948, 2990, 1949, 2991, 1950, 2992, 1951,
    2993, 1952, 2994, 1953, 2995, 1954, 2996, 1955, 2997, 1956, 3012, 1957,
    3013, 1958, 3014, 1959, 3015, 1960, 3016, 1961, 3017, 1962, 3018, 1963,
    3019, 1964, 3020, 1965, 3021, 1966, 3022, 1967, 3023, 1968, 3024, 1969,
    3025, 1970, 3026, 1971, 3027, 1972, 3028, 1973, 3029, 1974, 3030, 1975,
    3031, 1976, 3032, 1977, 3033, 1978, 3034, 1979, 3035, 1980, 3036, 1981,
    3037, 1982, 3038, 1983, 3051, 1984, 3052, 1985, 3053, 1986, 3054, 1987,
    3055, 1988, 3056, 1989, 3057, 1990, 3058, 1991, 3059, 1992, 3060, 1993,
    3061, 1994, 3062, 1995, 3063, 1996, 3064, 1997, 3065, 1998, 3066, 1999,
    3067, 2000, 3068, 2001, 3069, 2002, 3070, 2003, 3071, 2004, 3072, 2005,
    3073, 2006, 3074, 2007, 3075, 2008, 3076, 2009, 3105, 2010, 3106, 2011,
    3107, 2012, 3108, 2013, 3109, 2014, 3110, 2015, 3111, 2016, 3112, 2017,
    3113, 2018, 3114, 2019, 3115, 2020, 3116, 2021, 3117, 2022, 3118, 2023,
    3119, 2024, 3120, 2025, 3122, 2026, 3123, 2027, 3124, 2028, 3126, 2029,
    3127, 2030, 3128, 2031, 3129, 2032, 3130, 2033, 3131, 2034, 2879, 2035,
    2880, 2036, 2881, 2037, 2882, 2038, 2998, 2039, 2999, 2040, 3000, 2041,
    3001, 2042, 3002, 2043, 3003, 2044, 3004, 2045, 3005, 2046, 3006, 2047,
    3007, 2048, 3008, 2049, 3009, 2050, 3010, 2051, 3011, 2052, 3164, 2053,
    3165, 2054, 3166, 2055, 3167, 2056, 2480, 2057, 2481, 2058, 2482, 2059,
    2483, 2060, 2484, 2061, 2485, 2062, 2504, 2063, 2505, 2064, 2506, 2065,
    2507, 2066, 2508, 2067, 2509, 2068, 2510, 2069, 2511, 2070, 2513, 2071,
    2512, 2072, 2514, 2073, 2515, 2074, 2516, 2075, 2517, 2076, 2518, 2077,
    2519, 2078, 2520, 2079, 2521, 2080, 2522, 2081, 2541, 2082, 2542, 2083,
    2543, 2084, 2544, 2085, 2545, 2086, 2546, 2087, 2547, 2088, 2582, 2089,
    2583, 2090, 2584, 2091, 2613, 2092, 2614, 2093, 2625, 2094, 2626, 2095,
    2627, 2096, 2628, 2097, 2629, 2098, 2630, 2099, 2631, 2100, 2632, 2101,
    2633, 2102, 2657, 2103, 2658, 2104, 2659, 2105, 2660, 2106, 2661, 2107,
    2662, 2108, 2663, 2109, 2664, 2110, 2665, 2111, 2666, 2112, 2667, 2113,
    2668, 2114, 2669, 2115, 2670, 2116, 2671, 2117, 2708, 2118, 2709, 2119,
    2710, 2120, 2757, 2121, 2785, 2122, 2786, 2123, 2787, 2124, 2788, 2125,
    2804, 2126, 2805, 2127, 2806, 2128, 2807, 2129, 2808, 2130, 2809, 2131,
    2810, 2132, 2811, 2133, 2812, 2134, 2813, 2135, 2814, 2136, 2815, 2137,
    2816, 2138, 2817, 2139, 2818, 2140, 2819, 2141, 2820, 2142, 2821, 2143,
    2859, 2144, 2883, 2145, 2884, 2146, 2885, 2147, 2886, 2148, 2895, 2149,
    2896, 2150, 2897, 2151, 2898, 2152, 2899, 2153, 2924, 2237, 2925, 2154,
    2926, 2155, 2927, 2156, 2928, 2157, 2952, 2158, 2953, 2159, 2954, 2160,
    2955, 2161, 2956, 2162, 2968, 2163, 2969, 2164, 2970, 2165, 2971, 2166,
    2972, 2167, 2973, 2168, 2974, 2169, 2988, 2170, 2989, 2171, 3039, 2172,
    3040, 2173, 3041, 2174, 3042, 2175, 3043, 2176, 3044, 2177, 3045, 2178,
    3046, 2179, 3047, 2180, 3048, 2181, 3049, 2182, 3050, 2183, 3077, 2184,
    3078, 2185, 3079, 2186, 3080, 2187, 3081, 2188, 3082, 2189, 3083, 2190,
    3084, 2191, 3085, 2192, 3086, 2193, 3087, 2194, 3088, 2195, 3089, 2196,
    3090, 2197, 3091, 2198, 3092, 2199, 3093, 2200, 3094, 2201, 3095, 2202,
    3096, 2203, 3097, 2204, 3098, 2205, 3099, 2206, 3100, 2207, 3101, 2208,
    3102, 2209, 3103, 2210, 3104, 2211, 3161, 2212, 3132, 2213, 3133, 2214,
    3134, 2215, 3135, 2216, 3136, 2217, 3137, 2218, 3138, 2219, 3139, 2220,
    3140, 2221, 3146, 2222, 3147, 2223, 3148, 2224, 3149, 2225, 3150, 2226,
    3151, 2227, 3152, 2228, 3153, 2229, 3154, 2230, 3155, 2231, 3141, 2232,
    3142, 2233, 3143, 2234, 3144, 2235, 3145, 2236,
};

int Globals::getDialogueSoundId(int code, Agent *agent) {
    const int *t = gGDS_pairTable;
    for (unsigned i = 0; (i >> 6) < 0x2f; i += 2) {
        if (t[i] == code) {
            return t[i + 1];
        }
    }

    if (agent == 0) {
        return -1;
    }

    int race = ((Agent *) (agent))->getRace();
    int male = ((Agent *) (agent))->isMale();

    int category;
    if (race == 3) {
        int *parts = ((Agent *) (agent))->getImageParts();
        if (parts != 0) {
            int *p = ((Agent *) (agent))->getImageParts();
            category = (*p == 2) ? 3 : 0;
        } else {
            category = 2;
        }
    } else {
        category = race;
    }

    int isMale = male;
    switch (category) {
        case 0:
        case 5: {
            if (isMale == 0) {
                if ((unsigned) (code - 0x172) > 0x13) {
                    return -1;
                }
                static const int femaleTable[20] = {
                    0x2a7, 0x29b, 0x2a1, 0x2a2, 0x2a3, 0x2a4, 0x2a5, 0x2a6, 0x295, 0x29c,
                    0x29d, 0x29e, 0x29f, 0x2a0, 0x296, 0x297, 0x298, 0x299, 0x29a, 0x294,
                };
                return femaleTable[code - 0x172];
            }

            switch (code) {
                case 0x172: return 0x2bb;
                case 0x173: return 0x2af;
                case 0x174: return 0x2b5;
                case 0x175: return 0x2b6;
                case 0x176: return 0x2b7;
                case 0x177: return 0x2b8;
                case 0x178: return 0x2b9;
                case 0x179: return 0x2ba;
                case 0x17a: return 0x2a9;
                case 0x17b: return 0x2b0;
                case 0x17c: return 0x2b1;
                case 0x17d: return 0x2b2;
                case 0x17e: return 0x2b3;
                case 0x17f: return 0x2b4;
                case 0x180: return 0x2aa;
                case 0x181: return 0x2ab;
                case 0x182: return 0x2ac;
                case 0x183: return 0x2ad;
                case 0x184: return 0x2ae;
                case 0x185: return 0x2a8;
                case 0x1aa: return 0x2c1;
                case 0x1ab: return 0x2c2;
                case 0x1ac: return 0x2c3;
                case 0x1ad: return 0x2c6;
                case 0x1ae: return 0x2c7;
                case 0x1af: return 0x2c8;
                case 0x1bd: return 0x2bc;
                case 0x1be: return 0x2bd;
                case 0x1bf: return 0x2be;
                case 0x139: return 0x2cb;
                default: return -1;
            }
        }
        case 1: {
            switch (code) {
                case 0x172: return 0x2df;
                case 0x173: return 0x2d3;
                case 0x174: return 0x2d9;
                case 0x175: return 0x2da;
                case 0x176: return 0x2db;
                case 0x177: return 0x2dc;
                case 0x178: return 0x2dd;
                case 0x179: return 0x2de;
                case 0x17a: return 0x2cd;
                case 0x17b: return 0x2d4;
                case 0x17c: return 0x2d5;
                case 0x17d: return 0x2d6;
                case 0x17e: return 0x2d7;
                case 0x17f: return 0x2d8;
                case 0x180: return 0x2ce;
                case 0x181: return 0x2cf;
                case 0x182: return 0x2d0;
                case 0x183: return 0x2d1;
                case 0x184: return 0x2d2;
                case 0x185: return 0x2cc;
                case 0x1aa: return 0x2e5;
                case 0x1ab: return 0x2e6;
                case 0x1ac: return 0x2e7;
                case 0x1ad: return 0x2ea;
                case 0x1ae: return 0x2eb;
                case 0x1af: return 0x2ec;
                case 0x1bd: return 0x2e0;
                case 0x1be: return 0x2e1;
                case 0x1bf: return 0x2e2;
                default: return -1;
            }
        }
        case 2:
        case 3:
            switch (code) {
                case 0x172: return 0x27e;
                case 0x173: return 0x272;
                case 0x174: return 0x278;
                case 0x175: return 0x279;
                case 0x176: return 0x27a;
                case 0x177: return 0x27b;
                case 0x178: return 0x27c;
                case 0x179: return 0x27d;
                case 0x17a: return 0x26c;
                case 0x17b: return 0x273;
                case 0x17c: return 0x274;
                case 0x17d: return 0x275;
                case 0x17e: return 0x276;
                case 0x17f: return 0x277;
                case 0x180: return 0x26d;
                case 0x181: return 0x26e;
                case 0x182: return 0x26f;
                case 0x183: return 0x270;
                case 0x184: return 0x271;
                case 0x185: return 0x26b;
                case 0x1aa: return 0x285;
                case 0x1ab: return 0x286;
                case 0x1ac: return 0x287;
                case 0x1ad: return 0x28a;
                case 0x1ae: return 0x28b;
                case 0x1af: return 0x28c;
                case 0x1b0: return 0x24c;
                case 0x1b2: return 0x24a;
                case 0x1ba: return 0x24d;
                case 0x1bd: return 0x27f;
                case 0x1be: return 0x281;
                case 0x1bf: return 0x282;
                case 0x139: return 0x28f;
                default: return -1;
            }
        case 4: {
            switch (code) {
                case 0x172: return 0x267;
                case 0x173: return 0x25b;
                case 0x174: return 0x261;
                case 0x175: return 0x262;
                case 0x176: return 0x263;
                case 0x177: return 0x264;
                case 0x178: return 0x265;
                case 0x179: return 0x266;
                case 0x17a: return 0x255;
                case 0x17b: return 0x25c;
                case 0x17c: return 0x25d;
                case 0x17d: return 0x25e;
                case 0x17e: return 0x25f;
                case 0x17f: return 0x260;
                case 0x180: return 0x256;
                case 0x181: return 599;
                case 0x182: return 600;
                case 0x183: return 0x259;
                case 0x184: return 0x25a;
                case 0x185: return 0x26a;
                case 0x139: return 0x268;
                default: return -1;
            }
        }
        case 6: {
            switch (code) {
                case 0x172: return 0x230;
                case 0x173: return 0x2f6;
                case 0x174: return 0x22a;
                case 0x175: return 0x22b;
                case 0x176: return 0x22c;
                case 0x177: return 0x22d;
                case 0x178: return 0x22e;
                case 0x179: return 0x22f;
                case 0x17a: return 0x2f0;
                case 0x17b: return 0x225;
                case 0x17c: return 0x226;
                case 0x17d: return 0x227;
                case 0x17e: return 0x228;
                case 0x17f: return 0x229;
                case 0x180: return 0x2f1;
                case 0x181: return 0x2f2;
                case 0x182: return 0x2f3;
                case 0x183: return 0x2f4;
                case 0x184: return 0x2f5;
                case 0x185: return 0x2ef;
                case 0x139: return 0x231;
                default: return -1;
            }
        }
        case 7: {
            switch (code) {
                case 0x172: return 0x246;
                case 0x173: return 0x23a;
                case 0x174: return 0x240;
                case 0x175: return 0x241;
                case 0x176: return 0x242;
                case 0x177: return 0x243;
                case 0x178: return 0x244;
                case 0x179: return 0x245;
                case 0x17a: return 0x234;
                case 0x17b: return 0x23b;
                case 0x17c: return 0x23c;
                case 0x17d: return 0x23d;
                case 0x17e: return 0x23e;
                case 0x17f: return 0x23f;
                case 0x180: return 0x235;
                case 0x181: return 0x236;
                case 0x182: return 0x237;
                case 0x183: return 0x238;
                case 0x184: return 0x239;
                case 0x185: return 0x233;
                case 0x139: return 0x247;
                default: return -1;
            }
        }
        case 8:
            if ((unsigned) (code - 0x1b3) <= 5) {
                return code + 0x9b;
            }
            return -1;
        default:
            return -1;
    }
}


String Globals::getRandomPlanetName() {
    FileRead *f = new FileRead();
    int which = nextInt_71ad0(Globals::rnd, 0x64);
    Station *st = f->loadStation(which);
    String name = st->getName();
    delete st;
    delete f;
    return name;
}

static const char gGRN_noFirst[] = "";
static const char gGRN_noLast[] = "";
static const char gGRN_space[] = "";

String Globals::getRandomName(int race, bool female) {
    FileRead *reader = new FileRead();
    Array<String *> *firstNames = reader->loadNamesBinary(race, female, true);
    Array<String *> *lastNames = reader->loadNamesBinary(race, female, false);
    String firstName = firstNames == nullptr
                           ? String("", false)
                           : String(*(*firstNames)[Globals::rnd->nextInt(firstNames->size())], false);
    String lastName = lastNames == nullptr
                          ? String("", false)
                          : String(*(*lastNames)[Globals::rnd->nextInt(lastNames->size())], false);
    if (firstNames != nullptr) {
        ArrayReleaseClasses(*firstNames);
        delete firstNames;
    }
    if (lastNames != nullptr) {
        ArrayReleaseClasses(*lastNames);
        delete lastNames;
    }
    delete reader;
    if (lastName.length != 0)
        return firstName + String(" ", false) + lastName;
    return String(firstName, false);
}


void Globals::getLine(unsigned font, String text, int maxWidth, String *out) {
    int lang = GameText::getLanguage();
    int width = (lang | 1) == 11 ? 15 : 5;
    if (lang == 15) width = lang;
    unsigned lastSpace = 0;
    for (unsigned i = 0; i < (unsigned) text.length; i++) {
        unsigned short c = *text[i];
        width += Globals::Canvas->GetTextWidth(font, text, i, i + 1);
        if (c == ' ') lastSpace = i;
        if (width >= maxWidth) {
            if ((int) lastSpace >= 1)
                *out = text.SubString(0, lastSpace + 1);
            else
                *out = text.SubString(0, i + 1);
            return;
        }
        if (*text[i] == '\n' || *text[i] == '\r') {
            *out = text.SubString(0, i + 1);
            return;
        }
    }
    if (text.length >= 2)
        *out = text.SubString(0, text.length);
    else
        *out = String("", false);
}

#include "engine/core/AERandom.h"
