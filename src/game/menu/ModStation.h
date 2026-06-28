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
    union FlagWord {
        // lint: union_decl -- word/halfword/byte view type
        uint32_t word;
        uint16_t halfword;
        uint16_t halfwords[2];
        uint8_t bytes[4];
    };

    unsigned fadeColor;
    int *field_0x08;
    int state;
    StarMap *starMap;
    CutScene *cutScene;
    char pendingHangarClose;
    NewsTicker *newsTicker;

    union {
        // lint: union_decl -- pointer overlaid with FlagWord view
        AbyssEngine::EaseInOutMatrix *cameraTween;
        FlagWord cameraTweenFlags;
    };

    char stationActive;
    int dt;
    int loadTick;

    union {
        // lint: union_decl -- int64 overlaid with FlagWord pair
        long long accumTime;

        struct {
            FlagWord accumTimeLo;
            FlagWord accumTimeHi;
        };
    };

    String stationName;
    int selectedButton;
    int *buttonState;

    union {
        // lint: union_decl -- int overlaid with FlagWord view
        int departPending;
        FlagWord departPendingFlags;
    };

    MenuTouchWindow *dlcMenu;
    int activeMission;
    Array<Radio *> *radioMessages;
    int field_0x5c;
    FlagWord m_nStarMapWindowOpen;
    FlagWord subWindowFlags;
    FlagWord modalFlags;
    FlagWord screenFlags;

    union {
        // lint: union_decl -- pointer overlaid with FlagWord view
        ChoiceWindow *choiceWindow;
        FlagWord choiceWindowFlags;
    };

    SpaceLounge *spaceLounge;
    HangarWindow *hangarWindow;
    StatusWindow *statusWindow;
    DialogueWindow *m_pDialogueWindow;
    DialogueWindow *dialogueWindow;
    ChoiceWindow *medalChoiceWindow;
    int *buttonRow;
    TouchButton *buttonLaunch;

    union {
        // lint: union_decl -- pointer overlaid with FlagWord view
        TouchButton *buttonCredits;
        FlagWord buttonCreditsFlags;
    };

    union {
        // lint: union_decl -- pointer overlaid with FlagWord view
        ScrollTouchBox *scrollBox;
        FlagWord scrollBoxFlags;
    };

    int introTimer;
    AEGeometry *hangarShipGeom;

    int field_0xa4;
    AEGeometry *hangarGeom;
    int field_0xac;
    FlagWord cameraFlags;
    int field_0xb4;
    int field_0xb8;
    int *medalArray;
    int medalIndex;
    int medalCount;
    int field_0xc8;
    int introCountdown;
    int field_0xd0;
    int field_0xd4;
    FlagWord alarmFlags;
    FlagWord hintFlags;
    int field_0xe0;
    int camAngle;
    int camScrollPos;
    int field_0xec;
    int camScrollVel;
    int field_0xf4;
    int field_0xf8;
    int field_0xfc;
    FlagWord dragFlags;
    ScrollTouchBox *idleBox;
    int field_0x108;
    int field_0x10c;

    union { // lint: union_decl (genuine type-pun: differing types/sizes overlaid at one offset, both used)
        int touchX;
        float touchXf;
    }; // lint: union_decl -- int/float reinterpret
    union { // lint: union_decl -- int/float reinterpret (both used in ModStation.cpp)
        int touchY;
        float touchYf;
    }; // lint: union_decl -- int/float reinterpret
    union {
        // lint: union_decl -- FlagWord/float reinterpret
        FlagWord scrollFlags;
        float scrollFlagsf;
    };

    union { // lint: union_decl (genuine type-pun: differing types/sizes overlaid at one offset, both used)
        int scrollTarget;
        float scrollTargetf;
    }; // lint: union_decl -- int/float reinterpret
    int field_0x120;
    int field_0x124;
    void *activeTouch; // lint: void_ptr -- opaque platform touch handle, same ABI as OnTouch* touch param
    FlagWord field_0x12c;
    int camKeyX;
    int camKeyY;
    int camKeyZ;
    AbyssEngine::EaseInOut *easeX;
    AbyssEngine::EaseInOut *easeY;
    AbyssEngine::EaseInOut *easeZ;
    FlagWord field_0x148;
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

    int OnTouchBegin(int x, int y);

    int OnTouchEnd(int x, int y);

    int OnTouchMove(int x, int y);

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

#endif
