#include "game/core/Globals.h"
#include <arm_neon.h>
#include <cstdint>

namespace {
#pragma pack(push, 1)

    struct GameSettingsRecord {
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

    struct AgentBusyObject {
        uint8_t _pad[0xd0];
        int32_t guardCounter;
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

    static_assert(offsetof(AgentBusyObject, guardCounter) == 0xd0, "guardCounter");
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
unsigned char Globals::isStarMapVisible = 0;
unsigned char Globals::isCinematicModeActive = 0;
int Globals::mouseCursorActivated = 0;
unsigned char Globals::showMouseDuringGameOver = 0;
unsigned char Globals::keyBindings[8] = {};

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
unsigned char Globals::options[100] = {};
FModSound *Globals::sound = nullptr;
int Globals::logoIsShown = 0;
int Globals::isInMainMenu = 0;

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
float Globals::sec_fire_x;
float Globals::sec_fire_y;
float Globals::sec_fire_z;
float Globals::autopilot_x;
float Globals::autopilot_y;
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
unsigned char Globals::showBestDeal;
int Globals::simulateFire;
int Globals::subMenuIndex;
int Globals::topMenuIndex;
float Globals::action_menu_x;
float Globals::action_menu_y;
float Globals::action_menu_z;
RecordHandler *Globals::recordHandler;
float Globals::touch_stick_z;
float Globals::turret_view_x;
float Globals::turret_view_y;
float Globals::turret_view_z;
float Globals::fast_forward_x;
float Globals::fast_forward_y;
float Globals::fast_forward_z;
int Globals::fontLangSelect;
int Globals::keyboard_swipe;
unsigned char Globals::iCloudSupported;
unsigned char Globals::isIOS6Installed;
int Globals::other_buttons_x[10];
int Globals::other_buttons_y[10];
unsigned char Globals::showWingmanMenu;
unsigned char Globals::first_start_ever;
int Globals::globalPriceRaise;
unsigned char Globals::initMemoryWarning;
int Globals::instantActionType;
int Globals::instantActionWave;
unsigned char Globals::isTelekomCustomer;
int Globals::lastRecordWritten;
int Globals::instantActionScore;
unsigned char Globals::showNewCreditsMenu;
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
unsigned char Globals::inAppPurchaseSupported;
int Globals::lastStationMusicPlayed;
int Globals::menu_touch_window_type;
unsigned char Globals::useLowResTexturesForHD;
char *Globals::instantActionPlayerName;
unsigned char Globals::isRunningHDonWeakDevice;
float Globals::quickmenu_button_start_x;
float Globals::quickmenu_button_start_y;
int Globals::lastCampaignMissionFailed;
int Globals::secondaryWeaponsProbChange;
int Globals::lastCampaignMissionFailCount;
unsigned char Globals::startLiteVersionWithMoreCredits;
AbyssEngine::AERandom *Globals::rnd;
void *Globals::font;
// lint: void_ptr (opaque LoadingCallback userdata; ApplicationManager::SetLoadingCallback takes void*)
unsigned char Globals::keys[1020];
void *Globals::bankZ; // lint: void_ptr (unknown audio-bank handle, never referenced)
unsigned char Globals::hints[59];
Array<Item *> *Globals::items;
Array<Ship *> *Globals::ships;
AbyssEngine::PaintCanvas *Globals::Canvas;
float Globals::fire_x;
float Globals::fire_y;
float Globals::fire_z;
Galaxy *Globals::galaxy;
Layout *Globals::layout;
float Globals::boost_x;
float Globals::boost_y;
float Globals::boost_z;
Globals *Globals::globals;
float Globals::pause_x;
float Globals::pause_y;
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
#include "game/world/Station.h"
#include "game/mission/RecordHandler.h"
#include "game/world/SolarSystem.h"
#include "game/core/String.h"
#include "engine/math/BoundingSphere.h"

#include <new>

float VectorSignedToFloat(int v, int mode);

float VectorUnsignedToFloat(unsigned v, int mode);

static inline uint32_t nextInt_71aa4(AbyssEngine::AERandom *self) { return self->nextInt(); }

static inline int nextInt_71ad0(AbyssEngine::AERandom *self, int bound) { return self->nextInt(bound); }

int idiv(int a, int b);

void MatrixSetTranslation(void *m, float x, float y, float z);

// lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

int AERandom_nextIntB(int rng, int bound);

void FileRead_ctor(void *self); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void *FileRead_dtor(void *self); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

float VectorScale(void *vec, float scalar); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * Galaxy_dtor(void *g) { ((Galaxy*)g)->~Galaxy(); return (void *)(g); }

static inline void * Status_dtor(void *s) { ((Status*)s)->~Status(); return (void *)(s); }

static inline void *AERandom_dtor(void *r) { ((AbyssEngine::AERandom*)r)->~AERandom(); return r; }

static inline void * Layout_dtor(void *l) { ((Layout*)l)->~Layout(); return (void *)(l); }

void *Generator_dtor(void *g); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void * FModSound_dtor(void *s) { ((FModSound*)s)->~FModSound(); return (void *)(s); }

static inline void * Achievements_dtor(void *a) { ((Achievements*)a)->~Achievements(); return (void *)(a); }

static inline void * ImageFactory_dtor(void *f) { ((ImageFactory*)f)->~ImageFactory(); return (void *)(f); }

void Mission_ctor(void *m); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void Galaxy_ctor(void *g); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void Achievements_ctor(void *a); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void Status_ctor(void *s); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void FileRead_ctor(void *f); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void *FileRead_dtor(void *f); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline void AERandom_ctor(void *r) { new(r) AbyssEngine::AERandom(); }

void Generator_ctor(void *g); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

void FModSound_ctor(void *s); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)

static inline int FModSound_tryToStopMusicForBGMusic() { return (int)(Globals::sound->tryToStopMusicForBGMusic()); }

void ParticleSettingsRef_initialize();

static inline int Station_getIndex(int station) { return (int)(((Station*)(long)station)->getIndex()); }

static Status **g_status;

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
    return *src;
}

String Globals::getKeyActionName(int action) {
    (void) action;
    String r;
    return r;
}

float Globals::sqrt(float x) {
    (void) this;
    return __builtin_sqrtf(x);
}


void Globals::getRandomSystemForDrinks() {
    int slot = *(int *) &Globals::galaxy;
    int picked = nextInt_71ad0((AbyssEngine::AERandom *) *(int *) &Globals::rnd, 0x16);
    *(int *) (long) slot = picked;
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
    return src;
}

struct FileRead {
    Station *loadStation(int32_t id);

    Array<int32_t> *loadWreckCollision(int32_t id);

    Array<Item *> *loadItemsBinary();

    Array<Ship *> *loadShipsBinary();

    Array<String *> *loadNamesBinary(int32_t type, bool first, bool second);
};


Station *Globals::getRandomStation() {
    FileRead *f = (FileRead *) ::operator new(1);
    FileRead_ctor(f);
    int which = nextInt_71ad0(Globals::rnd, 0x87);
    Station *r = f->loadStation(which);
    ::operator delete(FileRead_dtor(f));
    return r;
}

void Globals::drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY) {
    drawLines(font, lines, baseX, startY, false);
}

void Globals::reportSupernovaChallengeScore() {
}

static const char gGLA_newline[] = "";

void Globals::getLineArray(unsigned int font, const String &text, int maxWidth,
                           Array<String *> *out) {
    String *line = static_cast<String *>(::operator new(sizeof(String)));
    {
        String *_s = line;
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }

    String work;
    work.Set((const_cast<String *>(&text))->data);
    String nl;
    nl.ctor_char(gGLA_newline, false);
    work += nl;

    const int total = static_cast<int>(work.size());

    unsigned count = 0;
    for (int consumed = 0; consumed < total;) {
        String rest;
        rest = work.SubString(consumed, total);
        getLine(font, rest, maxWidth, line);
        consumed += static_cast<int>(line->size());
        count++;
    }
    {
        String *_s = line;
        if (_s->data) delete[] _s->data;
        _s->data = nullptr;
        _s->length = 0;
    }
    ::operator delete(line);

    ArraySetLength(count, *out);
    for (unsigned i = 0; i < count; i++) {
        String *s = static_cast<String *>(::operator new(sizeof(String)));
        {
            String *_s = s;
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
        (*out)[i] = s;
    }

    int consumed = 0;
    for (unsigned i = 0; i < count; i++) {
        String *slot = (*out)[i];
        String rest;
        rest = work.SubString(consumed, total);
        getLine(font, rest, maxWidth, slot);
        consumed += static_cast<int>(slot->size());

        int lo = 0;
        int hi = static_cast<int>(slot->size());
        while (*slot->index(lo) == 0x20) {
            lo++;
        }
        hi++;
        do {
            hi--;
        } while (*slot->index(hi - 2) == 0x20);

        String trimmed;
        trimmed = slot->SubString(lo, hi);
        *slot = trimmed;
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

static int **const gGBS_guardHolder = nullptr;
static String ***const gGBS_strPtr = nullptr;
static int ***const gGBS_canvas = nullptr;
static const char gGBS_prefix[] = "";

String Globals::getBoundedString(const String &text, int width) {
    int *guardP = *gGBS_guardHolder;
    int saved = *guardP;

    String result;
    result.Set((const_cast<String *>(&text))->data);

    String **strPtr = *gGBS_strPtr;
    int **canvas = *gGBS_canvas;
    int w = ((PaintCanvas *) (long) **canvas)->GetTextWidth(0, **strPtr);
    if (width < w) {
        String *line = (String *) ::operator new(0xc);
        {
            String *_s = line;
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }

        int font = (int) (long) *strPtr;
        String tmpText;
        tmpText.Set((const_cast<String *>(&text))->data);
        Globals::globals->getLine((unsigned) font, tmpText, width - 3, line);

        String prefix;
        prefix.ctor_char(gGBS_prefix, false);
        result = prefix + *line;

        {
            String *_s = line;
            if (_s->data) delete[] _s->data;
            _s->data = nullptr;
            _s->length = 0;
        }
    }

    return result;
}

static const float gSCS_f86f8 = 0;
static const float gSCS_f86fc = 0;
static const float gSCS_f8700 = 0;
static const float gSCS_f8704 = 0;
static const float gSCS_f8708 = 0;
static const float gSCS_f870c = 0;
static const float gSCS_f8710 = 0;
static const float gSCS_f8714 = 0;
static const float gSCS_f8718 = 0;
static const float gSCS_f871c = 0;
static const float gSCS_f8720 = 0;
static const float gSCS_f8724 = 0;
static const float gSCS_f8728 = 0;
static const float gSCS_f872c = 0;
static const float gSCS_f8730 = 0;


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
    float fp1 = VectorSignedToFloat(p1, 0);

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
            float val = VectorUnsignedToFloat(o6, 0);
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
        float val = VectorUnsignedToFloat(o6, 0);
        float r = val + gSCS_f8704;
        o14 = (unsigned short) ((0.0f < r) ? (short) (int) r : 0);
        o7 = 0x14;
        (void) fv;
    }

label8556: {
        unsigned absp2 = (unsigned) (p2 - (p2 >> 31));
        float fv = gSCS_f8708;
        float val6 = VectorUnsignedToFloat(o6, 0);
        float r8 = fv + val6;
        o8 = (unsigned short) ((0.0f < r8) ? (short) (int) r8 : 0);
        o9 = (unsigned short) (o7 + (short) (absp2 >> 1));
        o10 = (unsigned short) (o8 + (short) (absp2 >> 1));

        float val6b = VectorUnsignedToFloat(o6, 0);
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

        float top = VectorSignedToFloat(screenH - p4, 0);
        float f12 = VectorUnsignedToFloat((unsigned) i12, 0);
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
                float fd = VectorUnsignedToFloat((unsigned) i16, 0);
                float fr = VectorSignedToFloat(a - b, 0);
                float scale = 1.0f;
                if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
                float base = VectorSignedToFloat(b, 0);
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
                float fd = VectorUnsignedToFloat((unsigned) i16, 0);
                float fr = VectorSignedToFloat(a - b, 0);
                float scale = 1.0f;
                if (fd / gSCS_f8730 < 1.0f) scale = fd / gSCS_f8730;
                float base = VectorSignedToFloat(b, 0);
                base = base + scale * fr;
                o11 = (unsigned short) ((0.0f < base) ? (short) (int) base : 0);
                o12 = (unsigned short) ((0.0f < top - gSCS_f8720) ? (short) (int) (top - gSCS_f8720) : 0);
            }
        }
    }
}

static const char gGAMT_noAgent[] = "";

String Globals::getAgentMissionText(Agent *agent) {

    String result;

    if (agent == nullptr) {
        result.ctor_char(gGAMT_noAgent, false);
        return result;
    }

    String acc;

    if (agent->isGenericAgent() == 0) {
        int event = agent->getEvent();
        if (event < 1 && agent->hasAcceptedOffer() == 0) {
            AgentBusyObject **busySlot = (AgentBusyObject **) &Globals::layout;
            AgentBusyObject *busy = *busySlot;

            busy->guardCounter += 1;
            int offer = agent->getOffer();

            if (offer == 8) {
                int ship = (int) (long) Globals::status->getShip();
                int price = ((Ship *) (ship))->getPrice();
                int pct = agent->getModPricePercentage();
                agent->setSellItemPrice(idiv(price * pct, 100));
                ship = (int) (long) Globals::status->getShip();
                int modIdx = agent->getSellModIndex();
                if (((Ship *) (ship))->hasModInstalled(modIdx) != 0) {
                    String *t = Globals::gameText->getText(modIdx);
                    (void) t;
                    busy->guardCounter -= 1;
                    result.Set((acc).data);
                    return result;
                }
            }

            {
                Agent *agentArg = agent;
                int bamtOffer = offer;
                String mt;
                {
                    if (mt.data) delete[] mt.data;
                    mt.data = nullptr;
                    mt.length = 0;
                }
                if (bamtOffer < 0 || agentArg == 0) {
                    String *line = ((GameText *) (long) **(int **) &g_status)->getText(0x300);
                    mt = *line;
                    acc.Set((mt).data);
                } else {
                    int baseKey;
                    switch (bamtOffer) {
                        case 8: baseKey = 0x36a;
                            break;
                        case 9: baseKey = 0x36a;
                            break;
                        case 10: baseKey = 0x36a;
                            break;
                        case 4: baseKey = 0x36a;
                            break;
                        default: baseKey = 0x36a;
                            break;
                    }
                    String *base = ((GameText *) (long) **(int **) &g_status)->getText(baseKey);
                    mt = *base;
                    {
                        String tok;
                        {
                            if (tok.data) delete[] tok.data;
                            tok.data = nullptr;
                            tok.length = 0;
                        }
                        ((Agent *) agentArg)->getName();
                    }
                    acc.Set((mt).data);
                }
            }
            busy->guardCounter -= 1;
        } else {
            {
                Agent *agentArg = agent;
                int bamtOffer = -1;
                String mt;
                {
                    if (mt.data) delete[] mt.data;
                    mt.data = nullptr;
                    mt.length = 0;
                }
                if (bamtOffer < 0 || agentArg == 0) {
                    String *line = ((GameText *) (long) **(int **) &g_status)->getText(0x300);
                    mt = *line;
                    acc.Set((mt).data);
                } else {
                    int baseKey;
                    switch (bamtOffer) {
                        case 8: baseKey = 0x36a;
                            break;
                        case 9: baseKey = 0x36a;
                            break;
                        case 10: baseKey = 0x36a;
                            break;
                        case 4: baseKey = 0x36a;
                            break;
                        default: baseKey = 0x36a;
                            break;
                    }
                    String *base = ((GameText *) (long) **(int **) &g_status)->getText(baseKey);
                    mt = *base;
                    {
                        String tok;
                        {
                            if (tok.data) delete[] tok.data;
                            tok.data = nullptr;
                            tok.length = 0;
                        }
                        ((Agent *) agentArg)->getName();
                    }
                    acc.Set((mt).data);
                }
            }
        }
    } else {
        {
            Agent *agentArg = agent;
            int bamtOffer = -1;
            String mt;
            {
                if (mt.data) delete[] mt.data;
                mt.data = nullptr;
                mt.length = 0;
            }
            if (bamtOffer < 0 || agentArg == 0) {
                String *line = ((GameText *) (long) **(int **) &g_status)->getText(0x300);
                mt = *line;
                acc.Set((mt).data);
            } else {
                int baseKey;
                switch (bamtOffer) {
                    case 8: baseKey = 0x36a;
                        break;
                    case 9: baseKey = 0x36a;
                        break;
                    case 10: baseKey = 0x36a;
                        break;
                    case 4: baseKey = 0x36a;
                        break;
                    default: baseKey = 0x36a;
                        break;
                }
                String *base = ((GameText *) (long) **(int **) &g_status)->getText(baseKey);
                mt = *base;
                {
                    String tok;
                    {
                        if (tok.data) delete[] tok.data;
                        tok.data = nullptr;
                        tok.length = 0;
                    }
                    ((Agent *) agentArg)->getName();
                }
                acc.Set((mt).data);
            }
        }
    }

    result.Set((acc).data);
    return result;
}

static const char gIAP_prefixA[] = "";
static const char gIAP_prefixB[] = "";
static const char gIAP_id50[] = "";
static const char gIAP_id0[] = "";
static const char gIAP_id1[] = "";
static const char gIAP_id2[] = "";
static const char gIAP_id3[] = "";
static const char gIAP_id4[] = "";
static const char gIAP_id51[] = "";
static const char gIAP_id52[] = "";
static const char gIAP_id53[] = "";
static const char gIAP_id54[] = "";

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
                    int cmp = entry->Compare_str(&full);
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
    {
        if (tmp.data) delete[] tmp.data;
        tmp.data = nullptr;
        tmp.length = 0;
    }
    tmp.ToUpperCase();
    String result;
    return result;
}

static const char gLTS_minTens[] = "";
static const char gLTS_minEmpty[] = "";
static const char gLTS_hrTens[] = "";
static const char gLTS_hrEmpty[] = "";
static const char gLTS_sep[] = "";

void Globals::longToTimeStringNoSeconds(long long ms, String &out) {
    long long q = ms / 0xea60;
    int minute = (int) (q % 0x3c);

    long long q2 = ms / 0xea60;
    int hours = (int) (q2 % 0x18);

    String mPart, mNum, minStr;
    mPart.ctor_char(minute < 10 ? gLTS_minTens : gLTS_minEmpty, false);
    mNum.Set((long long) (minute));
    minStr = mPart + mNum;

    long long h = ms / 0xea60;
    int hv = (int) h * 0x18 + hours;

    String hPart, hNum, hrStr;
    hPart.ctor_char(hv < 10 ? gLTS_hrTens : gLTS_hrEmpty, false);
    hNum.Set((long long) (hv));
    hrStr = hPart + hNum;

    String sep, left;
    sep.ctor_char(gLTS_sep, false);
    left = sep + hrStr;
    out = left + minStr;

    return;
}

static const unsigned short gGSG_resTable[1] = {};
static const unsigned short gGSG_meshTable[1] = {};
static const short gGSG_extraTable[1] = {};
static const unsigned gGSG_lodTable[1] = {};
static const unsigned gGSG_childTable[1] = {};

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

static const int gREF_table = 0;

unsigned Globals::getRandomEnemyFighter(int kind) {
    (void) this;
    int t = kind;
    if ((unsigned) (kind - 9) > 1) {
        t = 8;
    }
    if (kind <= 3) {
        t = kind;
    }
    unsigned r;
    if (t == 1) {
        if (Globals::status->dlc1Won() == 0) {
            r = 9;
        } else {
            int n = AERandom_nextIntB(*(int *) &Globals::rnd, 0x64);
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
        const int *table = &gREF_table;
        int *rng = (int *) &Globals::rnd;
        do {
            do {
                r = AERandom_nextIntB(*rng, 0x25);
            } while ((unsigned) ((r & ~4u) - 9) < 2);
        } while (((r < 0x10) && ((1u << r & 0x8101u) != 0)) ||
                 (table[r] != t));
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

    float pw = VectorSignedToFloat(width, 0);
    float nh = VectorSignedToFloat(-height, 0);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 0, nh, 0, pw);
    float nw = VectorSignedToFloat(-width, 0);
    meshCanvas->MeshSetPoint((unsigned int) mesh, 1, nh, 0, nw);
    float ph = VectorSignedToFloat(height, 0);
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

void BoundingAAB_ctor(void *self, float x0, float y0, float z0, float x1, float y1, float z1);

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

struct PolymorphicSingleton {
    virtual ~PolymorphicSingleton() {
    }
};

static RecordHandler **const gG_recordHandler = nullptr;
static Galaxy **const gG_galaxy = nullptr;
static Status **const gG_status = nullptr;
static GameText **const gG_gameText = nullptr;
static AbyssEngine::AERandom **const gG_random = nullptr;
static Layout **const gG_layout = nullptr;
static Generator **const gG_generator = nullptr;
static PolymorphicSingleton **const gG_polyObj = nullptr;
static FModSound **const gG_fmod = nullptr;
static Array<Item *> **const gG_items = nullptr;
static Array<Ship *> **const gG_ships = nullptr;
static Achievements **const gG_achievements = nullptr;
static ImageFactory **const gG_imageFactory = nullptr;
static int **const gG_tail = nullptr;

Globals::~Globals() {
    RecordHandler **rhSlot = gG_recordHandler;
    if (*rhSlot != 0) {
        (*rhSlot)->saveOptions();
    }
    Galaxy **galSlot = gG_galaxy;
    if (*galSlot != 0) {
        ::operator delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    Status **statSlot = gG_status;
    if (*statSlot != 0) {
        ::operator delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    GameText **gtSlot = gG_gameText;
    if (*gtSlot != 0) {
        delete *gtSlot;
    }
    *gtSlot = 0;
    AbyssEngine::AERandom **rngSlot = gG_random;
    if (*rngSlot != 0) {
        ::operator delete(AERandom_dtor(*rngSlot));
    }
    *rngSlot = 0;
    Layout **laySlot = gG_layout;
    if (*laySlot != 0) {
        ::operator delete(Layout_dtor(*laySlot));
    }
    *laySlot = 0;
    Generator **genSlot = gG_generator;
    if (*genSlot != 0) {
        ::operator delete(Generator_dtor(*genSlot));
    }
    *genSlot = 0;
    if (*rhSlot != 0) {
        delete *rhSlot;
    }
    *rhSlot = 0;
    PolymorphicSingleton **polySlot = gG_polyObj;
    PolymorphicSingleton *poly = *polySlot;
    if (poly != 0) {
        delete poly;
    }
    *polySlot = 0;
    FModSound **fmodSlot = gG_fmod;
    if (*fmodSlot != 0) {
        ::operator delete(FModSound_dtor(*fmodSlot));
    }
    *fmodSlot = 0;
    Array<Item *> **itemSlot = gG_items;
    if (*itemSlot != 0) {
        Array<Item *> *items = *itemSlot;
        ArrayReleaseClasses(*items);
        delete items;
    }
    *itemSlot = 0;
    Array<Ship *> **shipSlot = gG_ships;
    if (*shipSlot != 0) {
        Array<Ship *> *ships = *shipSlot;
        ArrayReleaseClasses(*ships);
        delete ships;
    }
    *shipSlot = 0;
    if (*galSlot != 0) {
        ::operator delete(Galaxy_dtor(*galSlot));
    }
    *galSlot = 0;
    Achievements **achSlot = gG_achievements;
    if (*achSlot != 0) {
        ::operator delete(Achievements_dtor(*achSlot));
    }
    *achSlot = 0;
    if (*statSlot != 0) {
        ::operator delete(Status_dtor(*statSlot));
    }
    *statSlot = 0;
    ImageFactory **ifSlot = gG_imageFactory;
    if (*ifSlot != 0) {
        ::operator delete(ImageFactory_dtor(*ifSlot));
    }
    *ifSlot = 0;

    if (this->soundResources != 0) {
        ArrayRelease(*(this->soundResources));
        delete this->soundResources;
    }
    this->soundResources = 0;
    **gG_tail = 0;
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

static const float gSCF_b90 = 0;
static const float gSCF_b10 = 0;
static const float gSCF_b14 = 0;
static const float gSCF_b7c = 0;
static const float gSCF_b80 = 0;
static const float gSCF_b84 = 0;
static const float gSCF_b88 = 0;
static const float gSCF_b8c = 0;
static const float gSCF_b94 = 0;
static const float gSCF_b98 = 0;
static const float gSCF_b9c = 0;
static const float gSCF_ba0 = 0;
static const float gSCF_ba4 = 0;
static const float gSCF_ba8 = 0;
static const float gSCF_bac = 0;
static const float gSCF_bb0 = 0;
static const float gSCF_bb4 = 0;
static const float gSCF_bb8 = 0;
static const float gSCF_bbc = 0;
static const float gSCF_bc0 = 0;
static const float gSCF_bc4 = 0;
static const float gSCF_bc8 = 0;
static const float gSCF_bcc = 0;
static const float gSCF_bd0 = 0;
static const float gSCF_bd4 = 0;
static const float gSCF_bd8 = 0;
static const float gSCF_bdc = 0;
static const float gSCF_be0 = 0;
static const float gSCF_be4 = 0;
static const float gSCF_be8 = 0;
static const float gSCF_bec = 0;
static const float gSCF_bf0 = 0;


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
    float fcol = VectorSignedToFloat(colDelta, 0);
    float fp1 = VectorSignedToFloat(p1, 0);
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
        wField = VectorSignedToFloat(rint((int *) &Globals::w) - p2, 0);
        iv = (int) fp1b;
        rcoords((CoordsObject *) Globals::options)->fireValue = (int) fp1b;
        adj13 = gSCF_b90;
        if (isPhone == 0) {
            char flag6 = rf((char *) &Globals::iPadLarge);
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        }
    } else {
        if (isPhone == 0) {
            wField = VectorSignedToFloat(rint((int *) &Globals::w) - p2, 0);
            char flag6 = rf((char *) &Globals::iPadLarge);
            iv = (flag6 == 0) ? 0x96 : 0x12c;
            rcoords((CoordsObject *) Globals::options)->fireValue = iv;
            adj13 = (flag6 == 0) ? gSCF_b98 : gSCF_b94;
        } else {
            iv = 0xd2;
            wField = VectorSignedToFloat(rint((int *) &Globals::w) - p2, 0);
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
        o10 = clampU(a10 + VectorUnsignedToFloat(o6, 0));
        float a11 = (flag6 == 0) ? 4.0f : 8.0f;
        o11 = clampU(a11 + VectorUnsignedToFloat(o7, 0));
        float a12 = (flag6 == 0) ? 15.0f : 28.0f;
        o12 = clampU(a12 + VectorUnsignedToFloat(o6, 0));
        float a13 = (flag6 == 0) ? 13.0f : 27.0f;
        o13 = clampU(a13 + VectorUnsignedToFloat(o7, 0));
        float a16 = (flag6 == 0) ? gSCF_bc0 : gSCF_bbc;
        u16 = clampU(a16 + VectorUnsignedToFloat(o6, 0));
        tail = (flag6 == 0) ? gSCF_bc8 : gSCF_bc4;
    } else {
        o10 = clampU(VectorUnsignedToFloat(o6, 0) + gSCF_b9c);
        o11 = clampU(VectorUnsignedToFloat(o7, 0) + gSCF_ba0);
        o12 = clampU(VectorUnsignedToFloat(o6, 0) + gSCF_ba4);
        o13 = clampU(VectorUnsignedToFloat(o7, 0) + gSCF_ba8);
        u16 = clampU(VectorUnsignedToFloat(o6, 0) + gSCF_bac);
        tail = gSCF_bb0;
    }

    o16 = u16;
    o17 = clampU(VectorUnsignedToFloat(o7, 0) - tail);

    unsigned short u14;
    float t15;
    if (colDelta < iv) {
        o5 = p3;
        float w = VectorUnsignedToFloat(o6, 0);
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
        float w = VectorUnsignedToFloat(o6, 0);
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
    o15 = clampU(t15 + VectorUnsignedToFloat(o7, 0));
}

static PaintCanvas **const gRR_arg = nullptr;

void Globals::releaseResources() {
    Globals::Canvas->ReleaseAllResources();

    PaintCanvas *secondaryCanvas = *gRR_arg;
    if (secondaryCanvas != nullptr) {
        secondaryCanvas->ReleaseAllResources();
    }
}



static inline char flag(char *const g) { return *g; }

void Globals::loadFont(int kind) {
    unsigned glyph;
    short spacing;
    unsigned char isMainFontPersian = 1;

    switch (kind) {
        case 9: {
            Globals::Canvas->FontCreate((unsigned short) 0x2d74, *(unsigned int *) &Globals::font, false);
            if (flag((char *) &Globals::iPadHD) != 0) {
                spacing = -6;
            } else {
                spacing = flag((char *) &Globals::retinaDisplay) != 0 ? -8 : -4;
            }
            Globals::Canvas->FontSetSpacing(*(unsigned int *) &Globals::font, spacing);
            isMainFontPersian = 0;
            goto epilogue;
        }
        case 10:
            glyph = 0x2d78;
            break;
        case 0xb:
            glyph = 0x2d76;
            break;
        case 0xe:
            glyph = 0x2d7c;
            break;
        default: {
            if (kind == 0xf) {
                Globals::Canvas->FontCreate((unsigned short) 0x2d7e, *(unsigned int *) &Globals::font, false);
                if (flag((char *) &Globals::iPadHD) != 0) {
                    spacing = -7;
                } else {
                    spacing = flag((char *) &Globals::retinaDisplay) != 0 ? -10 : -5;
                }
            } else {
                Globals::Canvas->FontCreate((unsigned short) 0x457, *(unsigned int *) &Globals::font, false);
                if (flag((char *) &Globals::retinaDisplay) != 0) {
                    spacing = -5;
                } else if (flag((char *) &Globals::n9) != 0) {
                    spacing = -4;
                } else if (flag((char *) &Globals::iPadHD) != 0) {
                    spacing = -4;
                } else {
                    spacing = -2;
                }
            }
            Globals::Canvas->FontSetSpacing(*(unsigned int *) &Globals::font, spacing);
            goto setMain;
        }
    }

    Globals::Canvas->FontCreate((unsigned short) glyph, *(unsigned int *) &Globals::font, false);
    if (flag((char *) &Globals::iPadHD) != 0) {
        spacing = -6;
    } else {
        spacing = flag((char *) &Globals::retinaDisplay) != 0 ? -8 : -4;
    }
    Globals::Canvas->FontSetSpacing(*(unsigned int *) &Globals::font, spacing);

setMain:
    isMainFontPersian = 1;
epilogue: {
        int *mainCanvas = (int *) &Globals::Canvas;
        unsigned *mainFont = (unsigned *) &Globals::fontAlien;
        int cv = *mainCanvas;

        ((PaintCanvas *) (long) cv)->field_0x1c = isMainFontPersian;
        ((PaintCanvas *) (long) cv)->FontCreate((unsigned short) 0x51e, *mainFont, false);
        ((PaintCanvas *) (long) *mainCanvas)->FontSetSpacing(*mainFont, 0);
        unsigned *extra = (unsigned *) &Globals::fontLangSelect;
        ((PaintCanvas *) (long) *mainCanvas)->FontCreate((unsigned short) 0x2d7a, *extra, false);

        ((PaintCanvas *) (long) *mainCanvas)->FontSetSpacing(*extra, 0);
    }
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
        Mission *m = (Mission *) ::operator new(0x78);
        Mission_ctor(m);
        *missionSlot = (int) (long) m;
    }

    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    int *flagFFFF = (int *) &Globals::lastRecordWritten;
    int *langSettingSlot = &Globals::recordSlots;
    char *langFlag = (char *) &Globals::iPad;
    char *zeroByte = (char *) &Globals::startLiteVersionWithMoreCredits;

    settings->_region[5] = 1;
    settings->enableFlag30 = 1;
    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    settings->tintB = 0.5f;
    settings->brightness = 1.0f;
    *zeroByte = 0;
    char lang = *langFlag;
    reinterpret_cast<int16_t &>(settings->_region[3]) = 0x101;
    *flagFFFF = -1;
    *langSettingSlot = (lang == 0) ? 6 : 0xc;

    Galaxy *galaxy = (Galaxy *) ::operator new(8);
    Galaxy_ctor(galaxy);
    Globals::galaxy = galaxy;
    Achievements *ach = (Achievements *) ::operator new(0x28);
    Achievements_ctor(ach);
    Globals::achievements = ach;
    Status *status = (Status *) ::operator new(0x1f0);
    Status_ctor(status);
    Globals::status = status;
    ImageFactory *imgFac = new ImageFactory();
    Globals::imageFactory = imgFac;

    FileRead *fr = (FileRead *) ::operator new(1);
    FileRead_ctor(fr);
    Globals::items = (Array<Item *> *) (long) fr->loadItemsBinary();
    Globals::ships = (Array<Ship *> *) (long) fr->loadShipsBinary();
    ::operator delete(FileRead_dtor(fr));

    int *engineSlot = (int *) &Globals::Canvas;
    if (*engineSlot == 0) {
        *engineSlot = *reinterpret_cast<int *>(app);
    }
    Globals::appManager = app;
    app->VibrateEnable(0);

    AbyssEngine::AERandom *rng = (AbyssEngine::AERandom *) ::operator new(8);
    AERandom_ctor(rng);
    Globals::globals = this;
    Globals::rnd = rng;

    Generator *gen = (Generator *) ::operator new(1);
    Generator_ctor(gen);
    Globals::generator = gen;

    RecordHandler *rh = new RecordHandler();
    RecordHandler **rhSlotP = &Globals::recordHandler;
    *rhSlotP = rh;
    Globals::status->resetGame();
    (*rhSlotP)->loadOptions();

    FModSound *fmod = (FModSound *) ::operator new(0x243c);
    FModSound_ctor(fmod);
    FModSound **fmodSlotP = &Globals::sound;
    *fmodSlotP = fmod;
    fmod->init();

    (*fmodSlotP)->enableCategory(1, settings->_region[1]);
    (*fmodSlotP)->enableCategory(2, settings->_region[0]);
    (*fmodSlotP)->enableCategory(3, settings->_region[2]);
    (*fmodSlotP)->setVolume(1, settings->colorR);
    (*fmodSlotP)->setVolume(2, settings->colorG);
    (*fmodSlotP)->setVolume(3, settings->colorB);

    if (FModSound_tryToStopMusicForBGMusic() != 0) {
        settings->_region[1] = 0;
    }

    Globals::instantActionPoints = 0;
    Globals::first_start_ever = 1;
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
    ParticleSettingsRef_initialize();

    Array<int> *arr = new Array<int>();
    this->soundResources = arr;
    return (int) (long) arr;
}

static const int gPM_table0 = 0;
static const int gPM_table1 = 0;

void Globals::playMusicAndFadeOutCurrent(int mode) {
    int snd;
    int track;
    int vol = 0;

    if (mode == 2) {
        int *sndP = (int *) &Globals::sound;
        ((FModSound *) (*sndP))->stop(0);
        snd = *sndP;
        track = 0x91;
        ((FModSound *) (snd))->play(track, 0, 0, (float) vol);
        return;
    }
    if (mode == 1) {
        int *statSnd = (int *) &Globals::status;
        if (Globals::status->inAlienOrbit() != 0) {
            int *sndP = (int *) &Globals::sound;
            ((FModSound *) (*sndP))->stop(0);
            snd = *sndP;
            track = 0x88;
            int m = Globals::status->getCurrentCampaignMission();
            if (m > 0x92 && Globals::status->getCurrentCampaignMission() < 0x9a) {
                track = 0x91;
            }
            ((FModSound *) (snd))->play(track, 0, 0, (float) vol);
            return;
        }
        ((SolarSystem *) (long) Globals::status->getSystem())->getRace();
        int *sndP = (int *) &Globals::sound;
        ((FModSound *) (*sndP))->stop(0);
        if (Station_getIndex((int) (long) Globals::status->getStation()) == 0x6c) {
            ((FModSound *) (*sndP))->play(0x92, 0, 0, (float) vol);
            return;
        }
        if (Station_getIndex((int) (long) Globals::status->getStation()) == 0x65) {
            ((FModSound *) (*sndP))->play(0x93, 0, 0, (float) vol);
            return;
        }
        if (Globals::status->inSupernovaSystem() != 0) {
            if (Globals::status->getCurrentCampaignMission() == 0x59) {
                ((FModSound *) (*sndP))->play(0x8be, 0, 0, (float) vol);
                return;
            }
            if (Globals::status->getMission() != 0 && ((Mission *) (long) Globals::status->getMission())->isEmpty() ==
                0) {
                int tgt = ((Mission *) (long) Globals::status->getMission())->getTargetStation();
                if (tgt == Station_getIndex((int) (long) Globals::status->getStation())) {
                    int cm = Globals::status->getCurrentCampaignMission();
                    track = cm < 0x6a ? 0x8c1 : 0x8c2;
                    ((FModSound *) (*sndP))->play(track, 0, 0, (float) vol);
                    return;
                }
            }
            ((FModSound *) (*sndP))->play(0x94, 0, 0, (float) vol);
            return;
        }
        if (Globals::status->inDeepScienceOrbit() != 0) {
            ((FModSound *) (*sndP))->play(0x98, 0, 0, (float) vol);
            return;
        }
        if (Station_getIndex((int) (long) Globals::status->getStation()) == 0x78 &&
            (Globals::status->getCurrentCampaignMission() == 0x7e ||
             Globals::status->getCurrentCampaignMission() == 0x85)) {
            ((FModSound *) (*sndP))->play(0x8bf, 0, 0, (float) vol);
            return;
        }
        const int *table = &gPM_table1;
        track = table[((SolarSystem *) (long) Globals::status->getSystem())->getRace()];
        ((FModSound *) (*sndP))->play(track, 0, 0, (float) vol);
        return;
    }
    if (mode != 0) {
        return;
    }

    int race = ((SolarSystem *) (long) Globals::status->getSystem())->getRace();
    int *sndP = (int *) &Globals::sound;
    ((FModSound *) (*sndP))->stop(0);
    if (Station_getIndex((int) (long) Globals::status->getStation()) == 0x6c) {
        ((FModSound *) (*sndP))->play(0x84, 0, 0, (float) vol);
        return;
    }
    if (Station_getIndex((int) (long) Globals::status->getStation()) == 0x65) {
        ((FModSound *) (*sndP))->play(0x83, 0, 0, (float) vol);
        return;
    }
    int idx = Station_getIndex((int) (long) Globals::status->getStation());
    if (idx == 10 || Station_getIndex((int) (long) Globals::status->getStation()) == 100) {
        if (Station_getIndex((int) (long) Globals::status->getStation()) == 10 &&
            Globals::status->getCurrentCampaignMission() == 0x9f) {
            ((FModSound *) (*sndP))->play(0x90, 0, 0, (float) vol);
            return;
        }
        ((FModSound *) (*sndP))->play(0x85, 0, 0, (float) vol);
        return;
    }
    const int *table = &gPM_table0;
    track = table[race];
    ((FModSound *) (*sndP))->play(track, 0, 0, (float) vol);
}

static const int gGDS_pairTable[1] = {};

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
    FileRead *f = (FileRead *) ::operator new(1);
    FileRead_ctor(f);
    int which = nextInt_71ad0(Globals::rnd, 0x64);
    Station *st = f->loadStation(which);
    String name = st->getName();
    delete st;
    ::operator delete(FileRead_dtor(f));
    return name;
}

static const char gGRN_noFirst[] = "";
static const char gGRN_noLast[] = "";
static const char gGRN_space[] = "";

String Globals::getRandomName(int kind, bool both) {

    FileRead *fr = (FileRead *) ::operator new(1);
    FileRead_ctor(fr);
    Array<String *> *first = fr->loadNamesBinary(kind, both, 1);
    Array<String *> *last = fr->loadNamesBinary(kind, both, 0);

    String firstStr, lastStr;
    if (first == 0) {
        firstStr.ctor_char(gGRN_noFirst, false);
    } else {
        int idx = nextInt_71aa4(Globals::rnd);
        firstStr.Set(((*first)[idx])->data);
    }
    if (last == 0) {
        lastStr.ctor_char(gGRN_noLast, false);
    } else {
        int idx = nextInt_71aa4(Globals::rnd);
        lastStr.Set(((*last)[idx])->data);
    }

    if (first != 0) {
        ArrayReleaseClasses(*first);
        delete first;
    }
    if (last != 0) {
        ArrayReleaseClasses(*last);
        delete last;
    }
    ::operator delete(FileRead_dtor(fr));

    String result;
    if (firstStr.size() == 0) {
        result.Set((firstStr).data);
    } else {
        String space, mid;
        space.ctor_char(gGRN_space, false);
        mid = firstStr + space;
        result = mid + lastStr;
    }

    return result;
}

static const char gGL_empty[] = "";

void Globals::getLine(unsigned font, String text, int maxWidth, String *out) {
    int lang = GameText::getLanguage();
    int width = 5;
    if (((unsigned) (lang | 1)) == 0xb) width = 0xf;
    if ((unsigned) lang == 0xf) width = 0xf;

    int *canvas = (int *) &Globals::Canvas;
    unsigned lastSpace = 0;
    unsigned len = text.size();

    String tmp;
    for (unsigned i = 0; i < len;) {
        short ch = *text.index(i);
        width += ((PaintCanvas *) (long) *canvas)->GetTextWidth(font, text, i, i + 1);
        if (ch == 0x20) {
            lastSpace = i;
        }
        if (maxWidth <= width) {
            if (ch == 0x0a || ch == 0x0d) {
                tmp = text.SubString(0, i + 1);
            } else if ((int) lastSpace < 1) {
                tmp = text.SubString(0, i + 1);
            } else {
                tmp = text.SubString(0, lastSpace + 1);
            }
            *out = tmp;
            return;
        }
        i++;
        len = text.size();
    }

    if ((int) len < 2) {
        tmp.ctor_char(gGL_empty, false);
    } else {
        tmp = text.SubString(0, len);
    }
    *out = tmp;
}

#include "engine/core/AERandom.h"


