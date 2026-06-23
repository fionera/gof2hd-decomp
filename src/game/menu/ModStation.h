#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
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
    unsigned fadeColor;
    int *field_0x08;
    int state;
    StarMap *starMap;
    CutScene *cutScene;
    char pendingHangarClose;
    NewsTicker *newsTicker;
    AbyssEngine::EaseInOutMatrix *cameraTween;
    char stationActive;
    int dt;
    int loadTick;
    long long accumTime;
    String stationName;
    int selectedButton;
    int *buttonState;
    int departPending;
    void *dlcMenu;
    int activeMission;
    void *radioMessages;
    int field_0x5c;
    int m_nStarMapWindowOpen;
    int subWindowFlags;
    int modalFlags;
    int screenFlags;
    void *choiceWindow;
    void *spaceLounge;
    void *hangarWindow;
    void *statusWindow;
    DialogueWindow *m_pDialogueWindow;
    DialogueWindow *dialogueWindow;
    void *medalChoiceWindow;
    int *buttonRow;
    void *buttonLaunch;
    void *buttonCredits;
    void *scrollBox;
    int introTimer;
    void *hangarShipGeom;

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
