#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
#include <cstdint>
#include "../../engine/core/AEString.h"
#include "engine/math/EaseInOutMatrix.h"
#include "game/core/CutScene.h"
#include "game/ui/DialogueWindow.h"
#include "game/world/NewsTicker.h"
#include "game/world/StarMap.h"

class CutScene;
class DialogueWindow;
class NewsTicker;
class StarMap;
class ChoiceWindow;
class SpaceLounge;
class MenuTouchWindow;
class TouchButton;
class ScrollTouchBox;
class AEGeometry;
class Radio;
struct HangarWindow;
struct StatusWindow;
template<class T>
class Array;

namespace AbyssEngine {
    class EaseInOutMatrix;
    class EaseInOut;
}

class ModStation {
public:
    unsigned fadeColor;
    int *field_0x08;
    int state;
    StarMap *starMap;
    CutScene *cutScene;
    char pendingHangarClose;
    NewsTicker *newsTicker;

    // was union { EaseInOutMatrix* cameraTween; FlagWord cameraTweenFlags; }
    AbyssEngine::EaseInOutMatrix *cameraTween;

    char stationActive;
    int dt;
    int loadTick;

    // was union { long long accumTime; struct { FlagWord accumTimeLo, accumTimeHi; }; }
    long long accumTime;

    String stationName;
    int selectedButton;
    int *buttonState;

    // was union { int departPending; FlagWord departPendingFlags; }
    int departPending;

    MenuTouchWindow *dlcMenu;
    int activeMission;
    Array<Radio *> *radioMessages;
    int field_0x5c;
    uint32_t m_nStarMapWindowOpen;
    uint32_t subWindowFlags;
    uint32_t modalFlags;
    uint32_t screenFlags;

    // was union { ChoiceWindow* choiceWindow; FlagWord choiceWindowFlags; }
    ChoiceWindow *choiceWindow;

    SpaceLounge *spaceLounge;
    HangarWindow *hangarWindow;
    StatusWindow *statusWindow;
    DialogueWindow *m_pDialogueWindow;
    DialogueWindow *dialogueWindow;
    ChoiceWindow *medalChoiceWindow;
    int *buttonRow;
    TouchButton *buttonLaunch;

    // was union { TouchButton* buttonCredits; FlagWord buttonCreditsFlags; }
    TouchButton *buttonCredits;

    // was union { ScrollTouchBox* scrollBox; FlagWord scrollBoxFlags; }
    ScrollTouchBox *scrollBox;

    int introTimer;
    AEGeometry *hangarShipGeom;

    int field_0xa4;
    AEGeometry *hangarGeom;
    int field_0xac;
    uint32_t cameraFlags;
    int field_0xb4;
    int field_0xb8;
    int *medalArray;
    int medalIndex;
    int medalCount;
    int field_0xc8;
    int introCountdown;
    int field_0xd0;
    int field_0xd4;
    uint32_t alarmFlags;
    uint32_t hintFlags;
    int field_0xe0;
    int camAngle;
    int camScrollPos;
    int field_0xec;
    int camScrollVel;
    int field_0xf4;
    int field_0xf8;
    int field_0xfc;
    uint32_t dragFlags;
    ScrollTouchBox *idleBox;
    int field_0x108;
    int field_0x10c;

    // was union { int touchX; float touchXf; } -- float view: reinterpret_cast<float&>(touchX)
    int touchX;
    // was union { int touchY; float touchYf; } -- float view: reinterpret_cast<float&>(touchY)
    int touchY;
    // was union { FlagWord scrollFlags; float scrollFlagsf; }
    //   flag bytes: reinterpret over &scrollFlags; float view: reinterpret_cast<float&>(scrollFlags)
    int scrollFlags;

    // was union { int scrollTarget; float scrollTargetf; } -- float view: reinterpret_cast<float&>(scrollTarget)
    int scrollTarget;
    int field_0x120;
    int field_0x124;
    void *activeTouch; // lint: void_ptr -- opaque platform touch handle, same ABI as OnTouch* touch param
    uint32_t field_0x12c;
    int camKeyX;
    int camKeyY;
    int camKeyZ;
    AbyssEngine::EaseInOut *easeX;
    AbyssEngine::EaseInOut *easeY;
    AbyssEngine::EaseInOut *easeZ;
    uint32_t field_0x148;
    uint8_t field_0x14c[0x278 - 0x14c];
    float camCoordX;
    float camCoordY;
    float camCoordZ;
    uint8_t field_0x284[0x2a0 - 0x284];
    int buttonCacheX[5];
    int buttonCacheY[5];
    int idleDeltaX;
    int idleDeltaY;

    ModStation();

    virtual ~ModStation();

    void OnInitialize();

    void OnKeyPress(long long unused, long long key);

    long long OnKeyRelease(long long unused, long long key);

    int ShowLoadingScreen();

    void OnRelease();

    void OnRender2D();

    void OnRender3D();

    void OnResume();

    void OnSuspend();

    void OnTouchBegin(int x, int y, void *touch); // lint: void_ptr

    void OnTouchEnd(int x, int y, void *touch); // lint: void_ptr

    void OnTouchMove(int x, int y, void *touch); // lint: void_ptr

    void OnTouchBegin(int x, int y);

    void OnTouchEnd(int x, int y);

    void OnTouchMove(int x, int y);

    void OnUpdate();

    void addAchievement(int medalId, int kind);

    void autosave();

    void checkHints();

    void checkMedals();

    void checkPendingProducts();

    void enterStation();

    void leaveStation();

    void resetLight();

    void resetIdleCamForHangar();

    void setGameLoaded();

    void showCBSMessage();

    void showDlcMenu();

    void showMapWindow();
};

// Lock the byte layout of every former-union slot (and the field after it) so that
// deleting the FlagWord unions stays byte-identical to the original 32-bit ABI.
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(ModStation, cameraTween) == 0x20, "");
static_assert(__builtin_offsetof(ModStation, stationActive) == 0x24, "");
static_assert(__builtin_offsetof(ModStation, accumTime) == 0x30, "");
static_assert(__builtin_offsetof(ModStation, stationName) == 0x38, "");
static_assert(__builtin_offsetof(ModStation, departPending) == 0x4c, "");
static_assert(__builtin_offsetof(ModStation, dlcMenu) == 0x50, "");
static_assert(__builtin_offsetof(ModStation, m_nStarMapWindowOpen) == 0x60, "");
static_assert(__builtin_offsetof(ModStation, subWindowFlags) == 0x64, "");
static_assert(__builtin_offsetof(ModStation, modalFlags) == 0x68, "");
static_assert(__builtin_offsetof(ModStation, screenFlags) == 0x6c, "");
static_assert(__builtin_offsetof(ModStation, choiceWindow) == 0x70, "");
static_assert(__builtin_offsetof(ModStation, spaceLounge) == 0x74, "");
static_assert(__builtin_offsetof(ModStation, buttonCredits) == 0x94, "");
static_assert(__builtin_offsetof(ModStation, scrollBox) == 0x98, "");
static_assert(__builtin_offsetof(ModStation, introTimer) == 0x9c, "");
static_assert(__builtin_offsetof(ModStation, cameraFlags) == 0xb0, "");
static_assert(__builtin_offsetof(ModStation, alarmFlags) == 0xd8, "");
static_assert(__builtin_offsetof(ModStation, hintFlags) == 0xdc, "");
static_assert(__builtin_offsetof(ModStation, dragFlags) == 0x100, "");
static_assert(__builtin_offsetof(ModStation, idleBox) == 0x104, "");
static_assert(__builtin_offsetof(ModStation, touchX) == 0x110, "");
static_assert(__builtin_offsetof(ModStation, touchY) == 0x114, "");
static_assert(__builtin_offsetof(ModStation, scrollFlags) == 0x118, "");
static_assert(__builtin_offsetof(ModStation, scrollTarget) == 0x11c, "");
static_assert(__builtin_offsetof(ModStation, field_0x120) == 0x120, "");
static_assert(__builtin_offsetof(ModStation, field_0x12c) == 0x12c, "");
static_assert(__builtin_offsetof(ModStation, camKeyX) == 0x130, "");
static_assert(__builtin_offsetof(ModStation, field_0x148) == 0x148, "");
static_assert(__builtin_offsetof(ModStation, field_0x14c) == 0x14c, "");
#endif

#endif
