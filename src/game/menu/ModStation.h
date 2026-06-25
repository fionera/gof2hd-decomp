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
namespace AbyssEngine { 
    class EaseInOutMatrix;
 }


class ModStation {
public:

    union FlagWord {
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
        AbyssEngine::EaseInOutMatrix *cameraTween;
        FlagWord cameraTweenFlags;
    };
    char stationActive;
    int dt;
    int loadTick;
    union {
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
        int departPending;
        FlagWord departPendingFlags;
    };
    void *dlcMenu;
    int activeMission;
    void *radioMessages;
    int field_0x5c;
    FlagWord m_nStarMapWindowOpen;
    FlagWord subWindowFlags;
    FlagWord modalFlags;
    FlagWord screenFlags;
    union {
        void *choiceWindow;
        FlagWord choiceWindowFlags;
    };
    void *spaceLounge;
    void *hangarWindow;
    void *statusWindow;
    DialogueWindow *m_pDialogueWindow;
    DialogueWindow *dialogueWindow;
    void *medalChoiceWindow;
    int *buttonRow;
    void *buttonLaunch;
    union {
        void *buttonCredits;
        FlagWord buttonCreditsFlags;
    };
    union {
        void *scrollBox;
        FlagWord scrollBoxFlags;
    };
    int introTimer;
    void *hangarShipGeom;

    int field_0xa4;
    void *hangarGeom;
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
    void *idleBox;
    int field_0x108;
    int field_0x10c;
    union { int touchX; float touchXf; };
    union { int touchY; float touchYf; };
    union {
        FlagWord scrollFlags;
        float scrollFlagsf;
    };
    union { int scrollTarget; float scrollTargetf; };
    int field_0x120;
    int field_0x124;
    void *activeTouch;
    FlagWord field_0x12c;
    int camKeyX;
    int camKeyY;
    int camKeyZ;
    void *easeX;
    void *easeY;
    void *easeZ;
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

    void OnTouchBegin(int x, int y, void *touch);

    void OnTouchEnd(int x, int y, void *touch);

    void OnTouchMove(int x, int y, void *touch);

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
