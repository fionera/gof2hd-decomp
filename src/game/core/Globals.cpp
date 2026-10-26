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
        // interpretations. The leading volume bytes stay in a byte array; flagAt0f
        // is a named int16 so its store does not alias the volume bytes:
        //   musicVolume  uint8  @0x0c -> _region[0]
        //   volumePair   int16  @0x0c -> reinterpret_cast<int16_t&>(_region[0])
        //   sfxVolume    uint8  @0x0d -> _region[1]
        //   ambientVolume uint8 @0x0e -> _region[2]
        //   flagAt0f     int16  @0x0f -> flagAt0f
        //   flagAt10     int16  @0x10 -> byte-offset 0x10 cast
        //   flagAt11     uint8  @0x11 -> flagAt11
        uint8_t _region[3];
        int16_t flagAt0f;
        uint8_t flagAt11;
        uint8_t _pad12[2];

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
    static_assert(offsetof(GameSettingsRecord, flagAt0f) == 0x0f, "flagAt0f");
    static_assert(offsetof(GameSettingsRecord, flagAt11) == 0x11, "flagAt11");
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
float Globals::rotateShipInStation = 0.0f;
float Globals::translateStarMapInXDirection = 0.0f;
float Globals::translateStarMapInYDirection = 0.0f;
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
#include "engine/math/BoundingAAB.h"
#include "engine/file/FileRead.h"
#include "engine/math/Transform.h"
#include "engine/render/ParticleSettingsRef.h"
#include "engine/core/ArrayReleaseClasses.h"

#include <new>

static inline int nextInt_71ad0(AbyssEngine::AERandom *self, int bound) { return self->nextInt(bound); }

void MatrixSetTranslation(void *m, float x, float y, float z); // lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)


extern int g_android_leaderboard_scores[8];

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








static const unsigned short gShipGeometryRes[64] = {
    17000, 17001, 17002, 17003, 17004, 17005, 17006, 17007, 17008, 17009, 17010, 17011, 17012, 17013, 17014, 17015,
    17016, 17017, 17018, 17019, 17020, 17021, 17022, 17023, 17024, 17025, 17026, 17027, 17028, 17029, 17030, 17031,
    17032, 17033, 17034, 17035, 17036, 17211, 17216, 17221, 17227, 17233, 17239, 17245, 17251, 17257, 17263, 17269,
    17275, 17281, 17287, 17293, 17299, 17305, 17311, 17317, 17323, 17329, 17335, 17341, 17347, 17353, 17359, 17365,
};
static const unsigned short gShipMainMesh[64] = {
    65535, 65535, 65535, 65535, 18704, 65535, 65535, 65535, 18708, 18709, 65535, 65535, 18712, 65535, 65535, 65535,
    18716, 65535, 18718, 65535, 65535, 18721, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 18731,
    65535, 65535, 65535, 18735, 65535, 65535, 65535, 18739, 65535, 18741, 18742, 18743, 65535, 65535, 65535, 65535,
    65535, 17286, 17292, 65535, 65535, 17310, 17316, 18818, 18819, 18820, 18821, 18822, 18823, 65535, 65535, 17370,
};
static const unsigned short gShipExtraMesh[64] = {
    18700, 18701, 18702, 18703, 65535, 18705, 18706, 18707, 65535, 65535, 18710, 18711, 65535, 65535, 65535, 65535,
    65535, 18717, 65535, 18719, 18720, 65535, 18722, 18723, 18724, 18725, 18726, 18727, 18728, 18729, 18730, 65535,
    18732, 18733, 18734, 65535, 18736, 17215, 18738, 65535, 17231, 65535, 17243, 17249, 18779, 18811, 18812, 18813,
    18780, 65535, 17291, 18814, 18815, 65535, 65535, 18818, 18819, 18820, 18821, 18822, 18823, 18824, 18825, 18826,
};
static const int gShipLodMeshes[64][3] = {
    17100, 17101, 17102, 17103, 17104, 17105, 17106, 17107, 17108, 17109, 17110, 17111,
    17112, 17113, 17114, 17115, 17116, 17117, 17118, 17119, 17120, 17121, 17122, 17123,
    17124, 17125, 17126, 17127, 17128, 17128, 17130, 65535, 65535, 17133, 17134, 17135,
    17136, 17137, 17138, 17139, 17140, 17141, 17142, 17143, 17144, 17145, 17146, 17147,
    17148, 17149, 17150, 17151, 17152, 17153, 17154, 17155, 17156, 17157, 17158, 17159,
    17160, 17161, 17162, 17163, 17164, 17165, 17166, 17167, 17168, 17169, 17170, 17171,
    17172, 17173, 17174, 17175, 17176, 17177, 17178, 17179, 17180, 17181, 17182, 17183,
    17184, 17185, 17186, 17187, 17188, 17189, 17190, 17191, 17192, 17193, 17194, 17195,
    17196, 17197, 17198, 17199, 17200, 17201, 17202, 17203, 17204, 17205, 17206, 17207,
    17208, 17209, 17210, 65535, 65535, 65535, 65535, 65535, 65535, 17222, 17223, 65535,
    65535, 65535, 65535, 17234, 17235, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const int gShipLodChildMeshes[64][3] = {
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    18708, 18708, 18708, 18709, 18709, 18709, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 17040, 17041, 17042, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 65535, 65535, 65535, 0, 0, 0, 0, 0, 0,
};

static const int gRaceTable[64] = {
    3, 0, 8, 3, 2, 0, 3, 0, 9, 1, 0, 8, 2, 0, 0, 0,
    2, 0, 2, 3, 3, 2, 0, 8, 8, 8, 0, 0, 0, 8, 3, 2,
    8, 0, 0, 2, 0, 0, 0, 1, 0, 1, 1, 2, 1, 3, 3, 3,
    3, 1, 1, 0, 8, 1, 1, 0, 3, 2, 0, 0, 8, 1, 3, 1};



static inline int rint(int *const g) { return *g; }
static inline CoordsObject *rcoords(CoordsObject *const g) { return g; }

static inline unsigned short clampU(float v) {
    return (unsigned short) ((0.0f < v) ? (short) (int) v : 0);
}



static inline char flag(char *const g) { return *g; }

static const int gStationMusicByRace[4] = {128, 127, 129, 130};
static const int gSpaceMusicByRace[4] = {134, 139, 138, 137};

// Dialogue text id -> speech sound id pairs.
static const int gDialogueSoundPairs[3008] = {
    1795, 163, 1819, 164, 1836, 165, 1837, 166, 1877, 167, 1897, 168, 1913, 169, 1914, 170,
    1915, 171, 1916, 172, 1928, 173, 1929, 174, 1930, 175, 1947, 176, 1721, 177, 1722, 178,
    1723, 179, 1724, 180, 1970, 181, 2016, 182, 2017, 183, 2028, 184, 2049, 185, 2050, 186,
    2051, 187, 1738, 188, 1748, 189, 1783, 190, 1784, 191, 1793, 192, 1794, 193, 1785, 194,
    1786, 195, 1787, 196, 1788, 197, 1789, 198, 1790, 199, 1791, 200, 1792, 201, 1796, 202,
    1797, 203, 1806, 204, 1807, 205, 1808, 206, 1798, 207, 1799, 208, 1800, 209, 1801, 210,
    1802, 211, 1803, 212, 1804, 213, 1805, 214, 1809, 215, 1810, 216, 1811, 217, 1812, 218,
    1813, 219, 1814, 220, 1815, 221, 1817, 222, 1818, 223, 1825, 224, 1826, 225, 1835, 226,
    1827, 227, 1828, 228, 1829, 229, 1830, 230, 1831, 231, 1832, 232, 1833, 233, 1834, 234,
    1843, 235, 1844, 236, 1845, 237, 1846, 238, 1847, 239, 1856, 240, 1857, 241, 1858, 242,
    1859, 243, 1860, 244, 1861, 245, 1848, 246, 1849, 247, 1850, 248, 1851, 249, 1852, 250,
    1853, 251, 1854, 252, 1855, 253, 1863, 254, 1864, 255, 1865, 256, 1866, 257, 1867, 258,
    1868, 259, 1869, 260, 1870, 261, 1871, 262, 1872, 263, 1873, 264, 1874, 265, 1875, 266,
    1700, 267, 1701, 268, 1710, 269, 1711, 270, 1712, 271, 1713, 272, 1714, 273, 1715, 274,
    1716, 275, 1717, 276, 1702, 277, 1703, 278, 1704, 279, 1705, 280, 1706, 281, 1707, 282,
    1708, 283, 1709, 284, 1876, 285, 1886, 286, 1887, 287, 1888, 288, 1889, 289, 1890, 290,
    1891, 291, 1892, 292, 1893, 293, 1894, 294, 1895, 295, 1898, 296, 1899, 297, 1908, 298,
    1909, 299, 1910, 300, 1911, 301, 1900, 302, 1901, 303, 1902, 304, 1903, 305, 1904, 306,
    1905, 307, 1906, 308, 1907, 309, 1922, 310, 1923, 311, 1924, 312, 1931, 313, 1932, 314,
    1933, 315, 1934, 316, 1943, 317, 1935, 318, 1936, 319, 1937, 320, 1938, 321, 1939, 322,
    1940, 323, 1941, 324, 1942, 325, 1954, 326, 1728, 327, 1729, 328, 1955, 329, 1956, 330,
    1957, 331, 1958, 332, 1959, 333, 1960, 334, 1961, 335, 1962, 336, 1963, 337, 1964, 338,
    1965, 339, 1966, 340, 1967, 341, 1968, 342, 1969, 343, 1971, 344, 1972, 345, 1981, 346,
    1973, 347, 1974, 348, 1975, 349, 1976, 350, 1977, 351, 1978, 352, 1979, 353, 1980, 354,
    1982, 355, 1983, 356, 1984, 357, 1985, 358, 1986, 359, 1987, 360, 1988, 361, 1989, 362,
    1990, 363, 1991, 364, 1992, 365, 2001, 366, 2002, 367, 2003, 368, 2004, 369, 2005, 370,
    1993, 371, 1994, 372, 1995, 373, 1996, 374, 1997, 375, 1998, 376, 1999, 377, 2000, 378,
    2006, 379, 2007, 380, 2008, 381, 2009, 382, 2010, 383, 2011, 384, 2012, 385, 2013, 386,
    2014, 387, 2015, 388, 2018, 389, 2019, 390, 2020, 391, 2021, 392, 2022, 393, 2023, 394,
    2024, 395, 2025, 396, 2026, 397, 2027, 398, 2030, 399, 2031, 400, 2032, 401, 2033, 402,
    2034, 403, 2035, 404, 2036, 405, 2037, 406, 2038, 407, 2039, 408, 2040, 409, 2041, 410,
    1733, 411, 1734, 412, 1735, 413, 1736, 414, 1737, 415, 2058, 416, 2059, 417, 2060, 418,
    2061, 419, 2062, 420, 2065, 421, 2066, 422, 2067, 423, 2068, 424, 2069, 425, 2070, 426,
    2075, 427, 2076, 428, 2077, 429, 2078, 430, 1739, 431, 1740, 432, 1741, 433, 1742, 434,
    1743, 435, 1744, 436, 1745, 437, 1747, 438, 1758, 439, 1759, 440, 1760, 441, 1761, 442,
    1762, 443, 1763, 444, 1764, 445, 1765, 446, 1766, 447, 1767, 448, 1768, 449, 1769, 450,
    1770, 451, 1771, 452, 1772, 453, 1773, 454, 1774, 455, 1775, 456, 1776, 457, 1777, 458,
    1778, 459, 1779, 460, 1780, 461, 1781, 462, 1912, 463, 533, 464, 534, 465, 532, 466,
    535, 467, 536, 468, 1668, 469, 1669, 470, 1684, 471, 1685, 472, 1686, 473, 1687, 474,
    1688, 475, 1689, 476, 1690, 477, 1691, 478, 1692, 479, 1693, 480, 1670, 481, 1694, 482,
    1695, 483, 1696, 484, 1671, 485, 1672, 486, 1673, 487, 1674, 488, 1675, 489, 1676, 490,
    1683, 491, 1820, 492, 1821, 493, 1822, 494, 1823, 495, 1824, 496, 1839, 497, 1840, 498,
    1841, 499, 1842, 500, 1697, 501, 1698, 502, 1699, 503, 1881, 504, 1882, 505, 1883, 506,
    1884, 507, 1885, 508, 1918, 509, 1919, 510, 1920, 511, 1921, 512, 1925, 513, 1926, 514,
    1927, 515, 1944, 516, 1946, 517, 1948, 518, 1949, 519, 1950, 520, 1951, 521, 1953, 522,
    2029, 523, 2042, 524, 2043, 525, 2044, 526, 2045, 527, 2046, 528, 2047, 529, 2048, 530,
    2052, 531, 2053, 532, 2054, 533, 2055, 534, 2056, 535, 2057, 536, 2063, 537, 2064, 538,
    2071, 539, 2072, 540, 2073, 541, 2074, 542, 1756, 543, 1757, 544, 1952, 545, 1838, 546,
    1917, 547, 1945, 548, 443, 584, 444, 585, 433, 587, 2106, 1124, 2108, 1126, 2110, 1128,
    2185, 1130, 2187, 1132, 2189, 1134, 2241, 1136, 2243, 1139, 2245, 1141, 2247, 1143, 2249, 1145,
    2283, 1146, 2285, 1148, 2079, 1150, 2081, 1152, 2083, 1154, 2085, 1156, 2087, 1158, 2089, 1160,
    2091, 1168, 2093, 1170, 2095, 1172, 2097, 1174, 2099, 1162, 2101, 1164, 2103, 1166, 2112, 1176,
    2114, 1178, 2116, 1180, 2118, 1182, 2130, 1184, 2132, 1186, 2134, 1188, 2136, 1190, 2140, 1192,
    2142, 1198, 2144, 1200, 2146, 1202, 2148, 1204, 2150, 1194, 2152, 1196, 2160, 1206, 2162, 1208,
    2164, 1210, 2166, 1212, 2168, 1214, 2170, 1216, 2174, 1218, 2176, 1221, 2178, 1223, 2180, 1225,
    2182, 1227, 2192, 1231, 2194, 1232, 2196, 1234, 2198, 1236, 2201, 1239, 2203, 1241, 2205, 1243,
    2207, 1245, 2210, 1247, 2212, 1252, 2214, 1254, 2216, 1256, 2218, 1258, 2220, 1249, 2222, 1251,
    2226, 1260, 2236, 1263, 2238, 1265, 2254, 1267, 2256, 1276, 2258, 1278, 2260, 1280, 2262, 1282,
    2264, 1269, 2266, 1271, 2268, 1273, 2271, 1284, 2273, 1288, 2275, 1290, 2277, 1292, 2279, 1294,
    2280, 1295, 2282, 1287, 2300, 1297, 2302, 1299, 2304, 1301, 2308, 1304, 2310, 1306, 2312, 1308,
    2314, 1310, 2316, 1312, 2325, 1315, 2327, 1317, 2329, 1319, 2331, 1321, 2333, 1323, 2335, 1330,
    2337, 1332, 2339, 1334, 2341, 1336, 2343, 1325, 2345, 1327, 2356, 1338, 2358, 1340, 2360, 1342,
    2362, 1344, 2364, 1346, 2367, 1349, 2369, 1354, 2371, 1356, 2373, 1358, 2375, 1360, 2377, 1351,
    2379, 1361, 2381, 1363, 2383, 1365, 2385, 1367, 2387, 1369, 2389, 1371, 2391, 1383, 2393, 1385,
    2395, 1387, 2397, 1389, 2398, 1390, 2399, 1373, 2400, 1374, 2402, 1376, 2404, 1378, 2406, 1380,
    2408, 1382, 2427, 1392, 2429, 1394, 2431, 1396, 2436, 1397, 2438, 1399, 2440, 1401, 2442, 1403,
    2447, 1408, 2449, 1410, 2451, 1412, 2307, 1414, 2453, 1415, 2455, 1417, 2457, 1419, 2459, 1421,
    2461, 1423, 2463, 1425, 2464, 1426, 2465, 1427, 2467, 1429, 2469, 1431, 2471, 1433, 2472, 1434,
    449, 1436, 450, 1437, 455, 1438, 456, 1439, 453, 1440, 454, 1441, 451, 1442, 452, 1443,
    459, 1444, 461, 1453, 463, 1455, 465, 1457, 467, 1459, 469, 1446, 471, 1448, 473, 1450,
    475, 1452, 479, 1461, 481, 1463, 483, 1465, 458, 1467, 457, 1468, 2121, 1470, 2123, 1472,
    2125, 1474, 2127, 1476, 2154, 1481, 2156, 1483, 2159, 1486, 2225, 1490, 2229, 1492, 2231, 1494,
    2233, 1496, 2252, 1499, 2288, 1502, 2289, 1505, 2291, 1507, 2294, 1510, 2296, 1512, 2298, 1504,
    2322, 1517, 2349, 1521, 2352, 1524, 2412, 1530, 2415, 1533, 2416, 1536, 2418, 1538, 2419, 1539,
    2420, 1540, 2421, 1541, 2423, 1543, 2424, 1534, 2425, 1535, 2432, 1544, 2433, 1545, 2434, 1546,
    2435, 1547, 2115, 1179, 2113, 1177, 2098, 1175, 2096, 1173, 2094, 1171, 2092, 1169, 2104, 1167,
    2102, 1165, 2100, 1163, 2090, 1161, 2430, 1395, 2428, 1393, 2426, 1391, 2363, 1345, 2361, 1343,
    2359, 1341, 2357, 1339, 2355, 1337, 2340, 1335, 2346, 1328, 2344, 1326, 2342, 1324, 2338, 1333,
    2336, 1331, 2334, 1329, 2332, 1322, 2324, 1314, 2305, 1302, 2303, 1300, 2301, 1298, 2299, 1296,
    2239, 1266, 2237, 1264, 2235, 1262, 2227, 1261, 2208, 1246, 2206, 1244, 2204, 1242, 2202, 1240,
    2190, 1228, 2161, 1207, 2088, 1159, 2086, 1157, 2084, 1155, 2082, 1153, 2080, 1151, 2286, 1149,
    2284, 1147, 2250, 1137, 2248, 1144, 2246, 1142, 2244, 1140, 2242, 1138, 2240, 1135, 2473, 1435,
    2470, 1432, 2468, 1430, 2466, 1428, 2462, 1424, 2460, 1422, 2458, 1420, 2456, 1418, 2454, 1416,
    2422, 1542, 2417, 1537, 2414, 1532, 2413, 1531, 2411, 1529, 2410, 1528, 2409, 1527, 2354, 1526,
    2353, 1525, 2351, 1523, 2350, 1522, 2348, 1520, 2347, 1519, 2323, 1518, 2321, 1516, 2320, 1515,
    2319, 1514, 2318, 1513, 2297, 1503, 2295, 1511, 2293, 1509, 2292, 1508, 2290, 1506, 2287, 1501,
    2253, 1500, 2251, 1498, 2234, 1497, 2232, 1495, 2230, 1493, 2228, 1491, 2224, 1489, 2223, 1488,
    2209, 1487, 2158, 1485, 2157, 1484, 2155, 1482, 2139, 1480, 2138, 1479, 2129, 1478, 2128, 1477,
    2126, 1475, 2124, 1473, 2122, 1471, 2120, 1469, 2188, 1133, 2186, 1131, 2111, 1129, 2109, 1127,
    2107, 1125, 2105, 1123, 484, 1466, 482, 1464, 480, 1462, 468, 1460, 466, 1458, 464, 1456,
    474, 1451, 472, 1449, 470, 1447, 462, 1454, 460, 1445, 2452, 1413, 2450, 1411, 2448, 1409,
    2446, 1407, 2445, 1406, 2444, 1405, 2443, 1404, 2441, 1402, 2439, 1400, 2437, 1398, 2396, 1388,
    2394, 1386, 2407, 1381, 2405, 1379, 2403, 1377, 2401, 1375, 2392, 1384, 2390, 1372, 2388, 1370,
    2386, 1368, 2384, 1366, 2382, 1364, 2380, 1362, 2378, 1352, 2376, 1350, 2374, 1359, 2372, 1357,
    2370, 1355, 2368, 1353, 2366, 1348, 2365, 1347, 2330, 1320, 2328, 1318, 2326, 1316, 2317, 1313,
    2315, 1311, 2313, 1309, 2311, 1307, 2309, 1305, 2306, 1303, 2281, 1286, 2278, 1293, 2276, 1291,
    2274, 1289, 2272, 1285, 2270, 1275, 2269, 1274, 2267, 1272, 2265, 1270, 2263, 1283, 2261, 1281,
    2259, 1279, 2257, 1277, 2255, 1268, 2221, 1250, 2219, 1259, 2217, 1257, 2215, 1255, 2213, 1253,
    2211, 1248, 2200, 1238, 2199, 1237, 2197, 1235, 2195, 1233, 2193, 1230, 2191, 1229, 2183, 1219,
    2181, 1226, 2179, 1224, 2177, 1222, 2175, 1220, 2173, 1217, 2169, 1215, 2167, 1213, 2165, 1211,
    2163, 1209, 2153, 1197, 2151, 1195, 2149, 1205, 2147, 1203, 2145, 1201, 2143, 1199, 2141, 1193,
    2137, 1191, 2135, 1189, 2133, 1187, 2131, 1185, 2119, 1183, 2117, 1181, 3156, 1548, 3157, 1549,
    3158, 1550, 3159, 1551, 3160, 1552, 3168, 1553, 3169, 1554, 3170, 1555, 3171, 1556, 3172, 1557,
    3173, 1558, 3162, 1559, 3163, 1560, 2716, 1561, 2717, 1562, 2718, 1563, 2719, 1564, 2474, 1565,
    2475, 1566, 2476, 1567, 2477, 1568, 2478, 1569, 2479, 1570, 2487, 1571, 2488, 1572, 2489, 1573,
    2490, 1574, 2491, 1575, 2492, 1576, 2493, 1577, 2495, 1578, 2496, 1579, 2497, 1580, 2498, 1581,
    2499, 1582, 2500, 1583, 2501, 1584, 2502, 1585, 2503, 1586, 2523, 1587, 2524, 1588, 2525, 1589,
    2526, 1590, 2527, 1591, 2528, 1592, 2529, 1593, 2530, 1594, 2531, 1595, 2532, 1596, 2533, 1597,
    2534, 1598, 2535, 1599, 2536, 1600, 2537, 1601, 2538, 1602, 2539, 1603, 2540, 1604, 2548, 1605,
    2549, 1606, 2550, 1607, 2551, 1608, 2552, 1609, 2553, 1610, 2554, 1611, 2556, 1612, 2557, 1613,
    2558, 1614, 2559, 1615, 2560, 1616, 2561, 1617, 2562, 1618, 2563, 1619, 2564, 1620, 2565, 1621,
    2566, 1622, 2567, 1623, 2568, 1624, 2569, 1625, 2570, 1626, 2571, 1627, 2572, 1628, 2573, 1629,
    2574, 1630, 2575, 1631, 2576, 1632, 2577, 1633, 2578, 1634, 2579, 1635, 2580, 1636, 2581, 1637,
    2585, 1638, 2586, 1639, 2587, 1640, 2588, 1641, 2589, 1642, 2590, 1643, 2591, 1644, 2592, 1645,
    2593, 1646, 2594, 1647, 2595, 1648, 2596, 1649, 2597, 1650, 2598, 1651, 2599, 1652, 2601, 1653,
    2602, 1654, 2603, 1655, 2604, 1656, 2605, 1657, 2606, 1658, 2607, 1659, 2608, 1660, 2609, 1661,
    2610, 1662, 2611, 1663, 2612, 1664, 2615, 1665, 2616, 1666, 2617, 1667, 2618, 1668, 2619, 1669,
    2620, 1670, 2621, 1671, 2622, 1672, 2623, 1673, 2624, 1674, 2634, 1675, 2635, 1676, 2636, 1677,
    2637, 1678, 2638, 1679, 2639, 1680, 2640, 1681, 2641, 1682, 2642, 1683, 2643, 1684, 2644, 1685,
    2645, 1686, 2646, 1687, 2647, 1688, 2648, 1689, 2649, 1690, 2650, 1691, 2651, 1692, 2652, 1693,
    2653, 1694, 2654, 1695, 2655, 1696, 2656, 1697, 2672, 1698, 2673, 1699, 2674, 1700, 2675, 1701,
    2676, 1702, 2677, 1703, 2678, 1704, 2679, 1705, 2680, 1706, 2681, 1707, 2682, 1708, 2683, 1709,
    2684, 1710, 2685, 1711, 2686, 1712, 2688, 1713, 2689, 1714, 2690, 1715, 2691, 1716, 2692, 1717,
    2693, 1718, 2694, 1719, 2695, 1720, 2696, 1721, 2697, 1722, 2698, 1723, 2699, 1724, 2700, 1725,
    2701, 1726, 2702, 1727, 2703, 1728, 2704, 1729, 2705, 1730, 2706, 1731, 2707, 1732, 2711, 1733,
    2712, 1734, 2713, 1735, 2714, 1736, 2715, 1737, 2720, 1738, 2721, 1739, 2722, 1740, 2723, 1741,
    2724, 1742, 2725, 1743, 2726, 1744, 2727, 1745, 2728, 1746, 2729, 1747, 2730, 1748, 2731, 1749,
    2732, 1750, 2733, 1751, 2734, 1752, 2735, 1753, 2736, 1754, 2737, 1755, 2738, 1756, 2739, 1757,
    2740, 1758, 2741, 1759, 2742, 1760, 2743, 1761, 2744, 1762, 2746, 1763, 2747, 1764, 2748, 1765,
    2749, 1766, 2750, 1767, 2751, 1768, 2752, 1769, 2753, 1770, 2754, 1771, 2755, 1772, 2756, 1773,
    2758, 1774, 2759, 1775, 2760, 1776, 2761, 1777, 2762, 1778, 2763, 1779, 2764, 1780, 2765, 1781,
    2766, 1782, 2767, 1783, 2768, 1784, 2769, 1785, 2770, 1786, 2771, 1787, 2772, 1788, 2773, 1789,
    2774, 1790, 2775, 1791, 2776, 1792, 2777, 1793, 2778, 1794, 2779, 1795, 2780, 1796, 2781, 1797,
    2782, 1798, 2783, 1799, 2784, 1800, 2789, 1801, 2790, 1802, 2791, 1803, 2792, 1804, 2793, 1805,
    2794, 1806, 2795, 1807, 2796, 1808, 2797, 1809, 2798, 1810, 2799, 1811, 2800, 1812, 2801, 1813,
    2802, 1814, 2803, 1815, 2822, 1816, 2823, 1817, 2824, 1818, 2825, 1819, 2826, 1820, 2827, 1821,
    2829, 1822, 2830, 1823, 2831, 1824, 2832, 1825, 2833, 1826, 2834, 1827, 2835, 1828, 2836, 1829,
    2837, 1830, 2838, 1831, 2839, 1832, 2840, 1833, 2841, 1834, 2842, 1835, 2843, 1836, 2844, 1837,
    2845, 1838, 2846, 1839, 2847, 1840, 2848, 1841, 2849, 1842, 2850, 1843, 2851, 1844, 2852, 1845,
    2853, 1846, 2854, 1847, 2855, 1848, 2856, 1849, 2857, 1850, 2858, 1851, 2860, 1852, 2861, 1853,
    2862, 1854, 2863, 1855, 2864, 1856, 2865, 1857, 2866, 1858, 2868, 1859, 2869, 1860, 2870, 1861,
    2871, 1862, 2872, 1863, 2873, 1864, 2874, 1865, 2875, 1866, 2876, 1867, 2877, 1868, 2878, 1869,
    2887, 1870, 2888, 1871, 2889, 1872, 2890, 1873, 2891, 1874, 2892, 1875, 2893, 1876, 2894, 1877,
    2900, 1878, 2901, 1879, 2902, 1880, 2903, 1881, 2904, 1882, 2905, 1883, 2906, 1884, 2907, 1885,
    2908, 1886, 2909, 1887, 2910, 1888, 2911, 1889, 2912, 1890, 2913, 1891, 2914, 1892, 2915, 1893,
    2916, 1894, 2917, 1895, 2918, 1896, 2919, 1897, 2920, 1898, 2921, 1899, 2922, 1900, 2923, 1901,
    2929, 1902, 2930, 1903, 2931, 1904, 2932, 1905, 2933, 1906, 2934, 1907, 2935, 1908, 2936, 1909,
    2937, 1910, 2938, 1911, 2939, 1912, 2940, 1913, 2941, 1914, 2942, 1915, 2943, 1916, 2944, 1917,
    2945, 1918, 2946, 1919, 2947, 1920, 2948, 1921, 2949, 1922, 2950, 1923, 2951, 1924, 2957, 1925,
    2958, 1926, 2959, 1927, 2960, 1928, 2961, 1929, 2962, 1930, 2963, 1931, 2964, 1932, 2965, 1933,
    2966, 1934, 2967, 1935, 2975, 1936, 2976, 1937, 2977, 1938, 2978, 1939, 2979, 1940, 2980, 1941,
    2981, 1942, 2982, 1943, 2983, 1944, 2984, 1945, 2985, 1946, 2986, 1947, 2987, 1948, 2990, 1949,
    2991, 1950, 2992, 1951, 2993, 1952, 2994, 1953, 2995, 1954, 2996, 1955, 2997, 1956, 3012, 1957,
    3013, 1958, 3014, 1959, 3015, 1960, 3016, 1961, 3017, 1962, 3018, 1963, 3019, 1964, 3020, 1965,
    3021, 1966, 3022, 1967, 3023, 1968, 3024, 1969, 3025, 1970, 3026, 1971, 3027, 1972, 3028, 1973,
    3029, 1974, 3030, 1975, 3031, 1976, 3032, 1977, 3033, 1978, 3034, 1979, 3035, 1980, 3036, 1981,
    3037, 1982, 3038, 1983, 3051, 1984, 3052, 1985, 3053, 1986, 3054, 1987, 3055, 1988, 3056, 1989,
    3057, 1990, 3058, 1991, 3059, 1992, 3060, 1993, 3061, 1994, 3062, 1995, 3063, 1996, 3064, 1997,
    3065, 1998, 3066, 1999, 3067, 2000, 3068, 2001, 3069, 2002, 3070, 2003, 3071, 2004, 3072, 2005,
    3073, 2006, 3074, 2007, 3075, 2008, 3076, 2009, 3105, 2010, 3106, 2011, 3107, 2012, 3108, 2013,
    3109, 2014, 3110, 2015, 3111, 2016, 3112, 2017, 3113, 2018, 3114, 2019, 3115, 2020, 3116, 2021,
    3117, 2022, 3118, 2023, 3119, 2024, 3120, 2025, 3122, 2026, 3123, 2027, 3124, 2028, 3126, 2029,
    3127, 2030, 3128, 2031, 3129, 2032, 3130, 2033, 3131, 2034, 2879, 2035, 2880, 2036, 2881, 2037,
    2882, 2038, 2998, 2039, 2999, 2040, 3000, 2041, 3001, 2042, 3002, 2043, 3003, 2044, 3004, 2045,
    3005, 2046, 3006, 2047, 3007, 2048, 3008, 2049, 3009, 2050, 3010, 2051, 3011, 2052, 3164, 2053,
    3165, 2054, 3166, 2055, 3167, 2056, 2480, 2057, 2481, 2058, 2482, 2059, 2483, 2060, 2484, 2061,
    2485, 2062, 2504, 2063, 2505, 2064, 2506, 2065, 2507, 2066, 2508, 2067, 2509, 2068, 2510, 2069,
    2511, 2070, 2513, 2071, 2512, 2072, 2514, 2073, 2515, 2074, 2516, 2075, 2517, 2076, 2518, 2077,
    2519, 2078, 2520, 2079, 2521, 2080, 2522, 2081, 2541, 2082, 2542, 2083, 2543, 2084, 2544, 2085,
    2545, 2086, 2546, 2087, 2547, 2088, 2582, 2089, 2583, 2090, 2584, 2091, 2613, 2092, 2614, 2093,
    2625, 2094, 2626, 2095, 2627, 2096, 2628, 2097, 2629, 2098, 2630, 2099, 2631, 2100, 2632, 2101,
    2633, 2102, 2657, 2103, 2658, 2104, 2659, 2105, 2660, 2106, 2661, 2107, 2662, 2108, 2663, 2109,
    2664, 2110, 2665, 2111, 2666, 2112, 2667, 2113, 2668, 2114, 2669, 2115, 2670, 2116, 2671, 2117,
    2708, 2118, 2709, 2119, 2710, 2120, 2757, 2121, 2785, 2122, 2786, 2123, 2787, 2124, 2788, 2125,
    2804, 2126, 2805, 2127, 2806, 2128, 2807, 2129, 2808, 2130, 2809, 2131, 2810, 2132, 2811, 2133,
    2812, 2134, 2813, 2135, 2814, 2136, 2815, 2137, 2816, 2138, 2817, 2139, 2818, 2140, 2819, 2141,
    2820, 2142, 2821, 2143, 2859, 2144, 2883, 2145, 2884, 2146, 2885, 2147, 2886, 2148, 2895, 2149,
    2896, 2150, 2897, 2151, 2898, 2152, 2899, 2153, 2924, 2237, 2925, 2154, 2926, 2155, 2927, 2156,
    2928, 2157, 2952, 2158, 2953, 2159, 2954, 2160, 2955, 2161, 2956, 2162, 2968, 2163, 2969, 2164,
    2970, 2165, 2971, 2166, 2972, 2167, 2973, 2168, 2974, 2169, 2988, 2170, 2989, 2171, 3039, 2172,
    3040, 2173, 3041, 2174, 3042, 2175, 3043, 2176, 3044, 2177, 3045, 2178, 3046, 2179, 3047, 2180,
    3048, 2181, 3049, 2182, 3050, 2183, 3077, 2184, 3078, 2185, 3079, 2186, 3080, 2187, 3081, 2188,
    3082, 2189, 3083, 2190, 3084, 2191, 3085, 2192, 3086, 2193, 3087, 2194, 3088, 2195, 3089, 2196,
    3090, 2197, 3091, 2198, 3092, 2199, 3093, 2200, 3094, 2201, 3095, 2202, 3096, 2203, 3097, 2204,
    3098, 2205, 3099, 2206, 3100, 2207, 3101, 2208, 3102, 2209, 3103, 2210, 3104, 2211, 3161, 2212,
    3132, 2213, 3133, 2214, 3134, 2215, 3135, 2216, 3136, 2217, 3137, 2218, 3138, 2219, 3139, 2220,
    3140, 2221, 3146, 2222, 3147, 2223, 3148, 2224, 3149, 2225, 3150, 2226, 3151, 2227, 3152, 2228,
    3153, 2229, 3154, 2230, 3155, 2231, 3141, 2232, 3142, 2233, 3143, 2234, 3144, 2235, 3145, 2236,
};


#include "engine/core/AERandom.h"

// Globals::Globals() below zero-initializes the Globals static members directly
// (the original reads each via a GOT-indirect load, which &Global compiles to in
// PIC). Previously stubbed as null gGC_p_* placeholder pointers, which folded the
// constructor to a 2-byte stub. Each placeholder->member mapping was verified
// against the original via Ghidra (recovered symbol names + plate comments).

Globals::Globals() {
    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    CtorSecondaryObject *secondary = (CtorSecondaryObject *) Globals::hints;
    Globals::recordHandler = 0;
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
    Globals::generator = 0;

    this->field_0x0 = 0;

    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    reinterpret_cast<int16_t &>(settings->_region[0]) = 0x101;
    *reinterpret_cast<int16_t *>(reinterpret_cast<uint8_t *>(settings) + 0x10) = 0;

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
    secondary->slot0 = 0;
    settings->flagAt4e = 0;
    settings->shortAt4c = 0;
    settings->intAt48 = 0;

    Globals::imageFactory = 0;
    this->soundResources = 0;
    Globals::instantActionType = -1;
    Globals::status = 0;
    settings->shortAt60 = 0x100;
    Globals::items = 0;
    Globals::achievements = 0;
    Globals::galaxy = 0;
    Globals::instantActionScore = 0;
    Globals::instantActionWave = 0;
    Globals::instantActionPlayerName = 0;
    Globals::ships = 0;

    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    Globals::initMemoryWarning = 0;
    Globals::lastSpaceMusicPlayed = -1;
    Globals::lastStationMusicPlayed = -1;
    Globals::lastCampaignMissionFailed = -1;
    Globals::lastCampaignMissionFailCount = 0;

    float fv = settings->qualityLevel;
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    settings->resWidth = v54;

    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    settings->resHeight = v58;
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

// Globals::init below accesses these engine globals directly (Mission::empty,
// Globals::options/items/ships/Canvas/globals/generator/recordHandler/sound/
// layout/imageFactory and the init flag bytes); the original reads each via a
// GOT-indirect load, which &Global compiles to in PIC. (Previously stubbed as
// null gI_* placeholder pointers, which folded init to a 2-byte stub.)

void Globals::init(AbyssEngine::ApplicationManager *app, AbyssEngine::Engine *engine) {
    (void) engine;
    int *missionSlot = (int *) &Mission::empty;
    if (*missionSlot == 0) {
        Mission *m = new Mission();
        *missionSlot = (int) (long) m;
    }

    int *flagFFFF = (int *) &Globals::lastRecordWritten;
    int *langSettingSlot = &Globals::recordSlots;
    char *langFlag = (char *) &Globals::iPad;
    char *zeroByte = (char *) &Globals::startLiteVersionWithMoreCredits;
    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;

    settings->flagAt0f = 0x101;
    settings->flagAt11 = 1;
    settings->glowR = 1.0f;
    settings->glowG = 1.0f;
    settings->tintR = 0.6f;
    settings->tintG = 0.6f;
    settings->enableFlag30 = 1;
    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    settings->tintB = 0.5f;
    settings->brightness = 1.0f;
    *zeroByte = 0;
    *flagFFFF = -1;
    *langSettingSlot = (*langFlag == 0) ? 6 : 0xc;

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
    Globals::rnd = rng;
    Globals::globals = this;

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

    (*fmodSlotP)->enableCategory(1, reinterpret_cast<bool &>(settings->_region[1]));
    (*fmodSlotP)->enableCategory(2, reinterpret_cast<bool &>(settings->_region[0]));
    (*fmodSlotP)->enableCategory(3, reinterpret_cast<bool &>(settings->_region[2]));
    (*fmodSlotP)->setVolume(1, settings->colorR);
    (*fmodSlotP)->setVolume(2, settings->colorG);
    (*fmodSlotP)->setVolume(3, settings->colorB);

    if (Globals::sound->tryToStopMusicForBGMusic() != 0) {
        settings->_region[1] = 0;
    }

    Globals::globals->resetHints();
    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    Globals::initMemoryWarning = 0;
    Globals::instantActionPoints = 0;
    Globals::first_start_ever = true;

    Layout *layout = new Layout();
    Globals::layout = layout;
    layout->reload();
    ParticleSettingsRef::initialize();

    this->soundResources = new Array<int>();
}

void Globals::resetHints() {
    HintsBuffer *hints = (HintsBuffer *) Globals::hints;
    const int32x4_t z = vdupq_n_s32(0);
    hints->quad[0].v = z;
    reinterpret_cast<Q16 &>(reinterpret_cast<uint8_t *>(hints->quad)[0x2b]).v = z;
    hints->quad[2].v = z;
    hints->quad[1].v = z;
}

void Globals::longToTimeString(long long ms, String &out) {
    int seconds = (int) (ms / 1000 % 60);
    int minutes = (int) (ms / 60000 % 60);
    int hours = (int) (ms / 3600000 % 24);
    String secondText = (seconds < 10 ? String("0", false) : String("", false)) + String(seconds);
    String minuteText = (minutes < 10 ? String("0", false) : String("", false)) + String(minutes);
    if (hours == 0) {
        out = minuteText + String(":", false) + secondText;
    } else {
        hours += (int) (ms / 86400000) * 24;
        String hourText = (hours < 10 ? String("0", false) : String("", false)) + String(hours);
        out = hourText + String(":", false) + minuteText + String(":", false) + secondText;
    }
}

void Globals::longToTimeStringNoSeconds(long long ms, String &out) {
    int minutes = (int) (ms / 60000 % 60);
    int hours = (int) (ms / 3600000 % 24);
    String minuteText = (minutes < 10 ? String("0", false) : String("", false)) + String(minutes);
    hours += (int) (ms / 86400000) * 24;
    String hourText = (hours < 10 ? String("0", false) : String("", false)) + String(hours);
    out = hourText + String(":", false) + minuteText;
}

unsigned int Globals::createBillBoard(int halfWidth, int halfLength, float u0, float v0, float u1, float v1,
                                      int texture) {
    unsigned int mesh = 0;
    Globals::Canvas->MeshCreate(12, 6, 19, texture, mesh);
    Globals::Canvas->MeshSetTriangle(mesh, 0, 0, 1, 2);
    Globals::Canvas->MeshSetTriangle(mesh, 1, 2, 1, 3);
    Globals::Canvas->MeshSetTriangle(mesh, 2, 4, 5, 6);
    Globals::Canvas->MeshSetTriangle(mesh, 3, 6, 5, 7);
    Globals::Canvas->MeshSetTriangle(mesh, 4, 8, 9, 10);
    Globals::Canvas->MeshSetTriangle(mesh, 5, 10, 9, 11);
    Globals::Canvas->MeshSetUv(mesh, 0, u0, v0);
    Globals::Canvas->MeshSetUv(mesh, 1, u0, v1);
    Globals::Canvas->MeshSetUv(mesh, 2, u1, v0);
    Globals::Canvas->MeshSetUv(mesh, 3, u1, v1);
    Globals::Canvas->MeshSetUv(mesh, 4, u0, v0);
    Globals::Canvas->MeshSetUv(mesh, 5, u0, v1);
    Globals::Canvas->MeshSetUv(mesh, 6, u1, v0);
    Globals::Canvas->MeshSetUv(mesh, 7, u1, v1);
    Globals::Canvas->MeshSetUv(mesh, 8, u0, v0);
    Globals::Canvas->MeshSetUv(mesh, 9, u0, v1);
    Globals::Canvas->MeshSetUv(mesh, 10, u1, v0);
    Globals::Canvas->MeshSetUv(mesh, 11, u1, v1);
    Globals::Canvas->MeshSetPoint(mesh, 0, (float) -halfWidth, 0, (float) halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 1, (float) -halfWidth, 0, (float) -halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 2, (float) halfWidth, 0, (float) halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 3, (float) halfWidth, 0, (float) -halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 4, 0, (float) -halfWidth, (float) halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 5, 0, (float) -halfWidth, (float) -halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 6, 0, (float) halfWidth, (float) halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 7, 0, (float) halfWidth, (float) -halfLength);
    Globals::Canvas->MeshSetPoint(mesh, 8, (float) -halfWidth, (float) halfWidth, 0);
    Globals::Canvas->MeshSetPoint(mesh, 9, (float) -halfWidth, (float) -halfWidth, 0);
    Globals::Canvas->MeshSetPoint(mesh, 10, (float) halfWidth, (float) halfWidth, 0);
    Globals::Canvas->MeshSetPoint(mesh, 11, (float) halfWidth, (float) -halfWidth, 0);
    return mesh;
}

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

void Globals::drawLines(unsigned int font, Array<String *> *lines, int baseX, int startY) {
    drawLines(font, lines, baseX, startY, false);
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

void Globals::releaseResources() {
    Globals::Canvas->ReleaseAllResources();
    Globals::layout->reload();
}

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
                if (Globals::retinaDisplay)
                    Globals::Canvas->FontSetSpacing(mainFont, -5);
                else if (Globals::n9)
                    Globals::Canvas->FontSetSpacing(mainFont, -4);
                else if (Globals::iPadHD)
                    Globals::Canvas->FontSetSpacing(mainFont, -4);
                else
                    Globals::Canvas->FontSetSpacing(mainFont, -2);
            }
            break;
    }
    Globals::Canvas->field_0x1c = latinFont;
    Globals::Canvas->FontCreate(0x51e, alienFont, true);
    Globals::Canvas->FontSetSpacing(alienFont, 0);
    Globals::Canvas->FontCreate(0x2d7a, langSelectFont, true);
    Globals::Canvas->FontSetSpacing(langSelectFont, 0);
}

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

String Globals::getRandomPlanetName() {
    FileRead *f = new FileRead();
    int which = nextInt_71ad0(Globals::rnd, 0x64);
    Station *st = f->loadStation(which);
    String name = st->getName();
    delete st;
    delete f;
    return name;
}

Station *Globals::getRandomStation() {
    FileRead *f = new FileRead();
    int which = nextInt_71ad0(Globals::rnd, 0x87);
    Station *r = f->loadStation(which);
    delete f;
    return r;
}

SolarSystem *Globals::getRandomSystemForDrinks() {
    Galaxy *galaxy = Globals::galaxy;
    int picked = Globals::rnd->nextInt(0x16);
    return (SolarSystem *) (intptr_t) galaxy->getSystem(picked);
}

String Globals::getItemName(int item) {
    String *src = Globals::gameText->getText(item + 0x4fa);
    return String(*src, false);
}

void Globals::reportLeaderboards() {
    int kills = Globals::status->getKills();
    g_android_leaderboard_scores[0] = kills;
}

void Globals::reportSupernovaChallengeScore() {
}

// lint: void_ptr (Pv-mangled internal shim; ABI-fixed signature)


Array<BoundingVolume *> *Globals::getWreckCollision(int kind, AEGeometry * /*geom*/) {
    FileRead *reader = new FileRead();
    Array<int> *data = reader->loadWreckCollision(kind);
    delete reader;

    Array<BoundingVolume *> *volumes = nullptr;
    if (data != nullptr) {
        int count = (*data)[0];
        Vector center = {0.0f, 0.0f, 0.0f};
        Vector extent = {0.0f, 0.0f, 0.0f};
        volumes = new Array<BoundingVolume *>();
        ArraySetLength(count, *volumes);
        int pos = 1;
        for (int i = 0; i < count; i++) {
            BoundingVolume *volume;
            switch ((*data)[pos++]) {
            case 0:
                center.x = (float) -(*data)[pos];
                center.z = (float) (*data)[pos + 1];
                center.y = (float) (*data)[pos + 2];
                extent.x = (float) (*data)[pos + 3];
                extent *= 0.6f;
                if (extent.x < 0.0f) {
                    extent *= -1.0f;
                }
                pos += 4;
                if (kind == 0) {
                    center *= 2.0f;
                    extent *= 2.0f;
                }
                volume = new BoundingSphere(0, 0, 0, center.x, center.y, center.z, extent.x);
                break;
            case 1:
                center.x = (float) -(*data)[pos];
                center.z = (float) (*data)[pos + 1];
                center.y = (float) (*data)[pos + 2];
                extent.x = (float) (*data)[pos + 3];
                extent.z = (float) (*data)[pos + 4];
                extent.y = (float) (*data)[pos + 5];
                extent *= 1.1f;
                pos += 6;
                if (kind == 0) {
                    center *= 2.0f;
                    extent *= 2.0f;
                }
                volume = new BoundingAAB(0, 0, 0, center.x, center.y, center.z, extent.x * 2.0f, extent.y * 2.0f,
                                         extent.z * 2.0f);
                break;
            default:
                continue;
            }
            (*volumes)[i] = volume;
        }
        ArrayRelease(*data);
        delete data;
    }
    return volumes;
}

AEGeometry *Globals::getShipGroup(int kind, int variant, bool wireframe) {
    AEGeometry *geom;
    if (kind == 13) {
        geom = new AEGeometry((uint16_t) 17013, Globals::Canvas, false);
        unsigned t0 = 0xffffffff;
        Globals::Canvas->TransformCreate(t0);
        Globals::Canvas->TransformAddMesh(t0, 18713, false);
        geom->addChild(t0);
        unsigned t1 = 0xffffffff;
        Globals::Canvas->TransformCreate(t1);
        Globals::Canvas->TransformAddMesh(t1, 17038, false);
        geom->addChild(t1);
        uint16_t lodMeshes[2] = {17139, 17140};
        int lodDists[2] = {35000, 45000};
        geom->setLodMeshes(lodMeshes, lodDists, 2);
        uint16_t lodChildMeshes[2] = {17040, 17041};
        geom->setLodChildMeshes(lodChildMeshes);
    } else if (kind == 14) {
        geom = new AEGeometry((uint16_t) 14311, Globals::Canvas, false);
        unsigned t0 = 0xffffffff;
        Globals::Canvas->TransformCreate(t0);
        Globals::Canvas->TransformAddMesh(t0, 14316, false);
        geom->addChild(t0);
        unsigned t1 = 0xffffffff;
        Globals::Canvas->TransformCreate(t1);
        Globals::Canvas->TransformAddMesh(t1, 14315, false);
        geom->addChild(t1);
        uint16_t lodMeshes[2] = {14312, 14313};
        int lodDists[2] = {35000, 60000};
        geom->setLodMeshes(lodMeshes, lodDists, 2);
        uint16_t lodChildMeshes[2] = {14316, 14316};
        geom->setLodChildMeshes(lodChildMeshes);
        geom->setScaling(2.0f, 2.0f, 2.0f);
    } else if (kind == 15) {
        switch (variant) {
        case 3: {
            geom = new AEGeometry((uint16_t) 17049, Globals::Canvas, false);
            unsigned head = 0xffffffff;
            Globals::Canvas->TransformCreate(head);
            Globals::Canvas->TransformAddMesh(head, 17055, false);
            geom->addChild(head);
            int segments = Globals::rnd->nextInt(4);
            unsigned firstLeft = 0xffffffff;
            unsigned firstRight = 0xffffffff;
            for (int i = 0; i < segments; i++) {
                unsigned left = 0xffffffff;
                unsigned right = 0xffffffff;
                Globals::Canvas->TransformCreate(left);
                Globals::Canvas->TransformCreate(right);
                Globals::Canvas->TransformAddMesh(left, 17052, false);
                Globals::Canvas->TransformAddMesh(right, 17053, false);
                if (firstLeft != 0xffffffff) {
                    Globals::Canvas->TransformAddChild(firstLeft, left);
                    Globals::Canvas->TransformAddChild(firstRight, right);
                } else {
                    firstLeft = left;
                    firstRight = right;
                }
                float z = i == 0 ? -2150.0f : (i == 1 ? 2150.0f : 4300.0f);
                AbyssEngine::AEMath::MatrixSetTranslation(*(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(left), 0, 0, z);
                AbyssEngine::AEMath::MatrixSetTranslation(*(AbyssEngine::AEMath::Matrix *) Globals::Canvas->TransformGetLocal(right), 0, 0, z);
            }
            if (firstLeft != 0xffffffff) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(firstLeft))->boundingSphere.radius = 100000.0f;
                geom->addChild(firstLeft);
            }
            unsigned tail = 0xffffffff;
            Globals::Canvas->TransformCreate(tail);
            Globals::Canvas->TransformAddMesh(tail, 17054, false);
            geom->addChild(tail);
            uint16_t lodMeshes[1] = {17050};
            int lodDists[1] = {35000};
            geom->setLodMeshes(lodMeshes, lodDists, 1);
            geom->setLodChildTransform(firstRight);
            break;
        }
        case 0: {
            geom = new AEGeometry((uint16_t) 17065, Globals::Canvas, false);
            unsigned t0 = 0xffffffff;
            Globals::Canvas->TransformCreate(t0);
            Globals::Canvas->TransformAddMesh(t0, 17070, false);
            geom->addChild(t0);
            unsigned t1 = 0xffffffff;
            Globals::Canvas->TransformCreate(t1);
            Globals::Canvas->TransformAddMesh(t1, 17074, false);
            geom->addChild(t1);
            unsigned t2 = 0xffffffff;
            Globals::Canvas->TransformCreate(t2);
            Globals::Canvas->TransformAddMesh(t2, 17069, false);
            geom->addChild(t2);
            uint16_t lodMeshes[2] = {17066, 17067};
            int lodDists[2] = {25000, 45000};
            geom->setLodMeshes(lodMeshes, lodDists, 2);
            uint16_t lodChildMeshes[2] = {17070, 17070};
            geom->setLodChildMeshes(lodChildMeshes);
            break;
        }
        default: {
            geom = new AEGeometry((uint16_t) 17060, Globals::Canvas, false);
            unsigned t0 = 0xffffffff;
            Globals::Canvas->TransformCreate(t0);
            Globals::Canvas->TransformAddMesh(t0, 17061, false);
            geom->addChild(t0);
            unsigned t1 = 0xffffffff;
            Globals::Canvas->TransformCreate(t1);
            Globals::Canvas->TransformAddMesh(t1, 17064, false);
            geom->addChild(t1);
            uint16_t lodMeshes[2] = {17062, 17063};
            int lodDists[2] = {35000, 60000};
            geom->setLodMeshes(lodMeshes, lodDists, 2);
            break;
        }
        }
    } else {
        geom = new AEGeometry(gShipGeometryRes[kind], Globals::Canvas, true);
        unsigned transform = 0xffffffff;
        unsigned meshId = 0xffffffff;
        if (gShipMainMesh[kind] != 0xffff) {
            Globals::Canvas->MeshCreate(gShipMainMesh[kind], meshId, true);
            Globals::Canvas->TransformCreate(transform);
            Globals::Canvas->TransformAddMeshId(transform, meshId);
            geom->addChild(transform);
            geom->meshHandle = meshId;
        }
        if (!wireframe) {
            unsigned materialId = 0xffffffff;
            Globals::Canvas->MaterialCreate(32200 + kind, materialId);
            Globals::Canvas->MeshChangeResourceMaterial(geom->meshId, 32200 + kind);
        }
        if (gShipExtraMesh[kind] != 0xffff) {
            unsigned t = 0xffffffff;
            Globals::Canvas->TransformCreate(t);
            Globals::Canvas->TransformAddMesh(t, gShipExtraMesh[kind], false);
            geom->addChild(t);
        }
        if (wireframe) {
            if (kind != 39 && kind != 41) {
                unsigned t = 0xffffffff;
                Globals::Canvas->TransformCreate(t);
                Globals::Canvas->TransformAddMesh(t, 17900 + kind, false);
                geom->addChild(t);
            }
        } else {
            unsigned t = 0xffffffff;
            Globals::Canvas->TransformCreate(t);
            Globals::Canvas->TransformAddMesh(t, 18000 + kind, false);
            geom->addChild(t);
        }
        int lodCount = 0;
        for (int i = 0; i < 2; i++) {
            if (gShipLodMeshes[kind][i] != 0xffff) {
                lodCount++;
            }
        }
        if (lodCount > 0) {
            uint16_t *lodMeshes = new uint16_t[lodCount];
            uint32_t *lodMeshIds = new uint32_t[lodCount];
            int *lodDists = new int[lodCount];
            for (int i = 0; i < lodCount; i++) {
                lodMeshes[i] = gShipLodMeshes[kind][i];
                lodDists[i] = 5000 + i * 8000;
                if (!wireframe) {
                    Globals::Canvas->MeshCreate(gShipLodMeshes[kind][i], lodMeshIds[i], true);
                    Globals::Canvas->MeshChangeResourceMaterial(lodMeshIds[i], 32200 + kind);
                }
            }
            if (wireframe) {
                geom->setLodMeshes(lodMeshes, lodDists, lodCount);
            } else {
                geom->setLodMeshesWithMeshIds(lodMeshes, lodMeshIds, lodDists, lodCount);
            }
            int childCount = 0;
            for (int i = 0; i < 2; i++) {
                if (gShipLodChildMeshes[kind][i] != 0xffff) {
                    childCount++;
                }
            }
            if (childCount > 0) {
                uint16_t *childMeshes = new uint16_t[childCount];
                for (int i = 0; i < childCount; i++) {
                    childMeshes[i] = gShipLodChildMeshes[kind][i];
                }
                geom->setLodChildMeshes(childMeshes);
                delete[] childMeshes;
            }
            delete[] lodMeshes;
            delete[] lodDists;
        }
        geom->setLodLastVisibleDistance(80000);
    }
    return geom;
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

Array<int> *Globals::getSoundResourceList() {
    return soundResources;
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
                } else if (Globals::status->getMission() == nullptr || Globals::status->getMission()->isEmpty() ||
                           Globals::status->getMission()->getTargetStation() !=
                           Globals::status->getStation()->getIndex()) {
                    Globals::sound->play(148, nullptr, nullptr, 0.0f);
                } else if (Globals::status->getCurrentCampaignMission() >= 106) {
                    Globals::sound->play(2242, nullptr, nullptr, 0.0f);
                } else {
                    Globals::sound->play(2241, nullptr, nullptr, 0.0f);
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

int Globals::getDialogueSoundId(int code, Agent *agent) {
    for (unsigned i = 0; i < 3008; i += 2) {
        if (gDialogueSoundPairs[i] == code) {
            return gDialogueSoundPairs[i + 1];
        }
    }
    if (agent == nullptr) {
        return -1;
    }
    int race = agent->getRace();
    bool male = agent->isMale();
    int soundId;
    if (race == 3) {
        if (agent->getImageParts() == nullptr) {
            race = 2;
        } else {
            race = agent->getImageParts()[0] == 2 ? 3 : 0;
        }
    }
    switch (race) {
    case 0:
    case 5:
        if (!male) {
            switch (code) {
            case 389:
                soundId = 660;
                break;
            case 370:
                soundId = 679;
                break;
            case 371:
                soundId = 667;
                break;
            case 372:
                soundId = 673;
                break;
            case 373:
                soundId = 674;
                break;
            case 374:
                soundId = 675;
                break;
            case 375:
                soundId = 676;
                break;
            case 376:
                soundId = 677;
                break;
            case 377:
                soundId = 678;
                break;
            case 378:
                soundId = 661;
                break;
            case 379:
                soundId = 668;
                break;
            case 380:
                soundId = 669;
                break;
            case 381:
                soundId = 670;
                break;
            case 382:
                soundId = 671;
                break;
            case 383:
                soundId = 672;
                break;
            case 384:
                soundId = 662;
                break;
            case 385:
                soundId = 663;
                break;
            case 386:
                soundId = 664;
                break;
            case 387:
                soundId = 665;
                break;
            case 388:
                soundId = 666;
                break;
            default:
                return -1;
            }
        break;
        }
        switch (code) {
        case 389:
            soundId = 680;
            break;
        case 370:
            soundId = 699;
            break;
        case 371:
            soundId = 687;
            break;
        case 372:
            soundId = 693;
            break;
        case 373:
            soundId = 694;
            break;
        case 374:
            soundId = 695;
            break;
        case 375:
            soundId = 696;
            break;
        case 376:
            soundId = 697;
            break;
        case 377:
            soundId = 698;
            break;
        case 378:
            soundId = 681;
            break;
        case 379:
            soundId = 688;
            break;
        case 380:
            soundId = 689;
            break;
        case 381:
            soundId = 690;
            break;
        case 382:
            soundId = 691;
            break;
        case 383:
            soundId = 692;
            break;
        case 384:
            soundId = 682;
            break;
        case 385:
            soundId = 683;
            break;
        case 386:
            soundId = 684;
            break;
        case 387:
            soundId = 685;
            break;
        case 388:
            soundId = 686;
            break;
        case 426:
            soundId = 705;
            break;
        case 427:
            soundId = 706;
            break;
        case 428:
            soundId = 707;
            break;
        case 429:
            soundId = 710;
            break;
        case 430:
            soundId = 711;
            break;
        case 431:
            soundId = 712;
            break;
        case 445:
            soundId = 700;
            break;
        case 446:
            soundId = 701;
            break;
        case 447:
            soundId = 702;
            break;
        case 313:
            soundId = 715;
            break;
        default:
            return -1;
        }
        break;
    case 1:
        switch (code) {
        case 389:
            soundId = 716;
            break;
        case 370:
            soundId = 735;
            break;
        case 371:
            soundId = 723;
            break;
        case 372:
            soundId = 729;
            break;
        case 373:
            soundId = 730;
            break;
        case 374:
            soundId = 731;
            break;
        case 375:
            soundId = 732;
            break;
        case 376:
            soundId = 733;
            break;
        case 377:
            soundId = 734;
            break;
        case 378:
            soundId = 717;
            break;
        case 379:
            soundId = 724;
            break;
        case 380:
            soundId = 725;
            break;
        case 381:
            soundId = 726;
            break;
        case 382:
            soundId = 727;
            break;
        case 383:
            soundId = 728;
            break;
        case 384:
            soundId = 718;
            break;
        case 385:
            soundId = 719;
            break;
        case 386:
            soundId = 720;
            break;
        case 387:
            soundId = 721;
            break;
        case 388:
            soundId = 722;
            break;
        case 426:
            soundId = 741;
            break;
        case 427:
            soundId = 742;
            break;
        case 428:
            soundId = 743;
            break;
        case 429:
            soundId = 746;
            break;
        case 430:
            soundId = 747;
            break;
        case 431:
            soundId = 748;
            break;
        case 445:
            soundId = 736;
            break;
        case 446:
            soundId = 737;
            break;
        case 447:
            soundId = 738;
            break;
        default:
            return -1;
        }
        break;
    case 2:
    case 3:
        switch (code) {
        case 389:
            soundId = 619;
            break;
        case 370:
            soundId = 638;
            break;
        case 371:
            soundId = 626;
            break;
        case 372:
            soundId = 632;
            break;
        case 373:
            soundId = 633;
            break;
        case 374:
            soundId = 634;
            break;
        case 375:
            soundId = 635;
            break;
        case 376:
            soundId = 636;
            break;
        case 377:
            soundId = 637;
            break;
        case 378:
            soundId = 620;
            break;
        case 379:
            soundId = 627;
            break;
        case 380:
            soundId = 628;
            break;
        case 381:
            soundId = 629;
            break;
        case 382:
            soundId = 630;
            break;
        case 383:
            soundId = 631;
            break;
        case 384:
            soundId = 621;
            break;
        case 385:
            soundId = 622;
            break;
        case 386:
            soundId = 623;
            break;
        case 387:
            soundId = 624;
            break;
        case 388:
            soundId = 625;
            break;
        case 426:
            soundId = 645;
            break;
        case 427:
            soundId = 646;
            break;
        case 428:
            soundId = 647;
            break;
        case 429:
            soundId = 650;
            break;
        case 430:
            soundId = 651;
            break;
        case 431:
            soundId = 652;
            break;
        case 432:
            soundId = 588;
            break;
        case 434:
            soundId = 586;
            break;
        case 442:
            soundId = 589;
            break;
        case 445:
            soundId = 639;
            break;
        case 446:
            soundId = 641;
            break;
        case 447:
            soundId = 642;
            break;
        case 313:
            soundId = 655;
            break;
        default:
            return -1;
        }
        break;
    case 4:
        switch (code) {
        case 378:
            soundId = 597;
            break;
        case 389:
            soundId = 618;
            break;
        case 370:
            soundId = 615;
            break;
        case 371:
            soundId = 603;
            break;
        case 372:
            soundId = 609;
            break;
        case 373:
            soundId = 610;
            break;
        case 374:
            soundId = 611;
            break;
        case 375:
            soundId = 612;
            break;
        case 376:
            soundId = 613;
            break;
        case 377:
            soundId = 614;
            break;
        case 379:
            soundId = 604;
            break;
        case 380:
            soundId = 605;
            break;
        case 381:
            soundId = 606;
            break;
        case 382:
            soundId = 607;
            break;
        case 383:
            soundId = 608;
            break;
        case 384:
            soundId = 598;
            break;
        case 385:
            soundId = 599;
            break;
        case 386:
            soundId = 600;
            break;
        case 387:
            soundId = 601;
            break;
        case 388:
            soundId = 602;
            break;
        case 313:
            soundId = 616;
            break;
        default:
            return -1;
        }
        break;
    case 6:
        switch (code) {
        case 389:
            soundId = 751;
            break;
        case 370:
            soundId = 560;
            break;
        case 371:
            soundId = 758;
            break;
        case 372:
            soundId = 554;
            break;
        case 373:
            soundId = 555;
            break;
        case 374:
            soundId = 556;
            break;
        case 375:
            soundId = 557;
            break;
        case 376:
            soundId = 558;
            break;
        case 377:
            soundId = 559;
            break;
        case 378:
            soundId = 752;
            break;
        case 379:
            soundId = 549;
            break;
        case 380:
            soundId = 550;
            break;
        case 381:
            soundId = 551;
            break;
        case 382:
            soundId = 552;
            break;
        case 383:
            soundId = 553;
            break;
        case 384:
            soundId = 753;
            break;
        case 385:
            soundId = 754;
            break;
        case 386:
            soundId = 755;
            break;
        case 387:
            soundId = 756;
            break;
        case 388:
            soundId = 757;
            break;
        case 313:
            soundId = 561;
            break;
        default:
            return -1;
        }
        break;
    case 7:
        switch (code) {
        case 389:
            soundId = 563;
            break;
        case 370:
            soundId = 582;
            break;
        case 371:
            soundId = 570;
            break;
        case 372:
            soundId = 576;
            break;
        case 373:
            soundId = 577;
            break;
        case 374:
            soundId = 578;
            break;
        case 375:
            soundId = 579;
            break;
        case 376:
            soundId = 580;
            break;
        case 377:
            soundId = 581;
            break;
        case 378:
            soundId = 564;
            break;
        case 379:
            soundId = 571;
            break;
        case 380:
            soundId = 572;
            break;
        case 381:
            soundId = 573;
            break;
        case 382:
            soundId = 574;
            break;
        case 383:
            soundId = 575;
            break;
        case 384:
            soundId = 565;
            break;
        case 385:
            soundId = 566;
            break;
        case 386:
            soundId = 567;
            break;
        case 387:
            soundId = 568;
            break;
        case 388:
            soundId = 569;
            break;
        case 313:
            soundId = 583;
            break;
        default:
            return -1;
        }
        break;
    case 8:
        switch (code) {
        case 435:
            soundId = 590;
            break;
        case 436:
            soundId = 591;
            break;
        case 437:
            soundId = 592;
            break;
        case 438:
            soundId = 593;
            break;
        case 439:
            soundId = 594;
            break;
        case 440:
            soundId = 595;
            break;
        default:
            return -1;
        }
        break;
    default:
        return -1;
    }
    return soundId;
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

void Globals::setCoordsSteer(int p1, int p2, int p3, int p4,
                             unsigned short &o5, unsigned short &o6, unsigned short &o7,
                             unsigned short &o8, unsigned short &o9, unsigned short &o10,
                             unsigned short &o11, unsigned short &o12, unsigned short &o13,
                             unsigned short &o14) {
    CoordsObject *coords = (CoordsObject *) Globals::options;
    int bottom = Globals::h + (-25 - p2 - p3);
    float minSteer = Globals::iPadHD ? 210.9375f : (!Globals::iPadLarge ? 150.0f : 300.0f);
    if (bottom < p1) {
        p1 = bottom;
    }
    float steer = (float) p1;
    if (minSteer > steer) {
        coords->steerValue = Globals::iPadHD ? 210 : (!Globals::iPadLarge ? 150 : 300);
    } else {
        coords->steerValue = (int) steer;
    }
    float stickGap;
    if (Globals::iPadHD) {
        o5 = 28;
        o6 = coords->steerValue;
        o13 = 28;
        o14 = (unsigned short) ((float) o6 + -126.5625f);
        o7 = 20;
        stickGap = 92.8125f;
    } else {
        unsigned short margin = !Globals::iPadLarge ? 20 : 40;
        o5 = margin;
        o6 = coords->steerValue;
        o13 = margin;
        o14 = (unsigned short) ((float) o6 - (!Globals::iPadLarge ? 90.0f : 180.0f));
        o7 = 20;
        stickGap = !Globals::iPadLarge ? 66.0f : 132.0f;
    }
    o8 = (unsigned short) (stickGap + (float) o6);
    o9 = o7 + p2 / 2;
    o10 = o8 + p2 / 2;
    float knob = (float) o6;
    if (Globals::iPadHD) {
        knob = knob + 313.59375f;
    } else {
        knob = (!Globals::iPadLarge ? 223.0f : 446.0f) + knob;
    }
    unsigned knobPos = (unsigned) knob;
    o12 = knobPos;
    float top = (float) (Globals::h - p4);
    if (top - (Globals::iPadHD ? 2.8125f : (!Globals::iPadLarge ? 2.0f : 4.0f)) < (float) knobPos) {
        int small;
        int big;
        if (Globals::iPadHD) {
            big = 243;
            small = 187;
            knobPos = (unsigned) ((float) knobPos - (top + -2.8125f));
            o12 = knobPos;
        } else {
            knobPos = (unsigned) ((float) knobPos - (top - (!Globals::iPadLarge ? 2.0f : 4.0f)));
            o12 = knobPos;
            small = !Globals::iPadLarge ? 133 : 266;
            big = !Globals::iPadLarge ? 173 : 346;
        }
        float t = (float) knobPos / 54.0f;
        o11 = (unsigned short) ((float) small + (t < 1.0f ? t : 1.0f) * (float) (big - small));
        o12 = (unsigned short) (top - (Globals::iPadHD ? 2.8125f : (!Globals::iPadLarge ? 2.0f : 4.0f)));
    } else {
        o11 = Globals::iPadHD ? 28 : (!Globals::iPadLarge ? 20 : 40);
    }
}

void Globals::setCoordsFire(int p1, int p2, unsigned p3, unsigned p4,
                            unsigned &o5, unsigned short &o6, unsigned short &o7,
                            unsigned short &o8, unsigned short &o9, unsigned short &o10,
                            unsigned short &o11, unsigned short &o12, unsigned short &o13,
                            unsigned short &o14, unsigned short &o15, unsigned short &o16,
                            unsigned short &o17) {
    CoordsObject *coords = (CoordsObject *) Globals::options;
    float minFire = Globals::iPadHD ? 210.9375f : (Globals::iPadLarge ? 300.0f : 150.0f);
    float margin = Globals::iPadHD ? 63.28125f : (Globals::iPadLarge ? 90.0f : 45.0f);
    int bottom = Globals::h - p2;
    float requested = (float) p1;
    float fire = requested;
    if (margin + (float) bottom < requested) {
        fire = (Globals::iPadHD ? 63.28125f : (Globals::iPadLarge ? 90.0f : 45.0f)) + (float) bottom;
    }
    if (minFire > fire) {
        coords->fireValue = Globals::iPadHD ? 210 : (Globals::iPadLarge ? 300 : 150);
    } else {
        float clamped = requested;
        if ((Globals::iPadHD ? 63.28125f : (Globals::iPadLarge ? 90.0f : 45.0f)) + (float) bottom < requested) {
            clamped = (Globals::iPadHD ? 63.28125f : (Globals::iPadLarge ? 90.0f : 45.0f)) + (float) bottom;
        }
        coords->fireValue = (int) clamped;
    }
    o6 = (unsigned short) ((float) (Globals::w - p2) + (Globals::iPadHD ? 56.25f : (Globals::iPadLarge ? 80.0f : 40.0f)));
    o7 = coords->fireValue;
    o8 = o6 + (unsigned) p2 / 2;
    o9 = o7 + (unsigned) p2 / 2;
    float last;
    float six = (float) o6;
    if (Globals::iPadHD) {
        o10 = (unsigned short) (six + 107.28125f);
        o11 = (unsigned short) ((float) o7 + 5.625f);
        o12 = (unsigned short) ((float) o6 + 18.09375f);
        o13 = (unsigned short) ((float) o7 + 19.28125f);
        o16 = (unsigned short) ((float) o6 + 112.5f);
        last = 94.21875f;
    } else {
        o10 = (unsigned short) ((Globals::iPadLarge ? 152.0f : 77.0f) + six);
        o11 = (unsigned short) ((Globals::iPadLarge ? 8.0f : 4.0f) + (float) o7);
        o12 = (unsigned short) ((Globals::iPadLarge ? 28.0f : 15.0f) + (float) o6);
        o13 = (unsigned short) ((Globals::iPadLarge ? 27.0f : 13.0f) + (float) o7);
        o16 = (unsigned short) ((Globals::iPadLarge ? 160.0f : 80.0f) + (float) o6);
        last = Globals::iPadLarge ? 134.0f : 67.0f;
    }
    o17 = (unsigned short) ((float) o7 - last);
    float trigger;
    if (coords->fireValue > bottom) {
        o5 = p3;
        float base = (float) o6;
        if (Globals::iPadHD) {
            o14 = (unsigned short) (base + -2.0f);
            trigger = 132.1875f;
        } else {
            o14 = (unsigned short) ((Globals::iPadLarge ? -2.0f : 0.0f) + base);
            trigger = Globals::iPadLarge ? 189.0f : 94.0f;
        }
    } else {
        o5 = p4;
        float base = (float) o6;
        if (Globals::iPadHD) {
            o14 = (unsigned short) (base + 100.0625f);
            trigger = 209.53125f;
        } else {
            o14 = (unsigned short) ((Globals::iPadLarge ? 144.0f : 74.0f) + base);
            trigger = Globals::iPadLarge ? 298.0f : 149.0f;
        }
    }
    o15 = (unsigned short) (trigger + (float) o7);
}

String Globals::getKeyBindingReplaceString(int key) {
    (void) key;

    String tmp;
    return String(*tmp.ToUpperCase(), false);
}

String Globals::getKeyActionName(int action) {
    (void) action;
    String r;
    return r;
}

String Globals::replaceKeyBindingTokens(const String &src) {
    return String(src, false);
}

float Globals::sqrt(float x) {
    return AbyssEngine::AEMath::Sqrtf(x);
}

int Globals::getInAppPurchaseArrayIndex(int productCode, Array<String *> *list) {
    int result = -1;
    if (list != nullptr) {
        unsigned i = 0;
        bool searching = true;
        while (i < list->size() && searching) {
            String *entry = (*list)[i];
            String prefix = String("www.fishlabs.net.gof2hd") + String(".");
            if (entry == nullptr) {
                result = -1;
                searching = false;
            } else {
                int differs = 1;
                switch (productCode) {
                    case 0: differs = entry->Compare(prefix + String("dlc1")); break;
                    case 1: differs = entry->Compare(prefix + String("dlc2")); break;
                    case 2: differs = entry->Compare(prefix + String("dlc3")); break;
                    case 3: differs = entry->Compare(prefix + String("dlc4")); break;
                    case 4: differs = entry->Compare(prefix + String("dlc5")); break;
                    case 50: differs = entry->Compare(prefix + String("dlcCredits1")); break;
                    case 51: differs = entry->Compare(prefix + String("dlcCredits2")); break;
                    case 52: differs = entry->Compare(prefix + String("dlcCredits3")); break;
                    case 53: differs = entry->Compare(prefix + String("dlcCredits4.0")); break;
                    case 54: differs = entry->Compare(prefix + String("dlcCredits5")); break;
                    default: break;
                }
                if (differs == 0) {
                    result = (int) i;
                    break;
                }
            }
            i++;
        }
    }
    return result;
}
