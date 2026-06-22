#ifndef GOF2_MGAME_H
#define GOF2_MGAME_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"

#include "engine/core/IApplicationModule.h"


using ::AbyssEngine::ApplicationManager;

class MGame : public IApplicationModule {
public:
    int loadProgress;
    int loadingImage;
    int cameraMode;
    uint8_t field_0x18;
    int field_0x1c;
    unsigned startTime;
    unsigned startTimeHigh;
    unsigned lastTime;
    unsigned lastTimeHigh;
    int frameTime;
    int frameTimeHigh;
    int deltaTime;
    int field_0x44;
    unsigned elapsedTime;
    int elapsedTimeHigh;
    int loadingTime;
    uint8_t active;
    PlayerEgo *player;
    int field_0x5c;
    uint8_t pauseOpen;
    uint8_t cutsceneActive;
    uint8_t jumpActive;
    uint8_t gameOverActive;
    uint8_t campaignMission;
    AbyssEngine::String gameOverTitle;
    int field_0x70;
    Hud *hud;
    Level *level;
    LevelScript *levelScript;
    Radar *radar;
    Radio *radio;
    MenuTouchWindow *menuWindow;
    DialogueWindow *dialogueWindow;
    StarMap *starMap;
    ChoiceWindow *choiceWindow;
    int touch0Id;
    int touch1Id;
    int menuTime;
    int freeCamFinger1X;
    int freeCamFinger1Y;
    int freeCamFinger1Z;
    int freeCamFinger0X;
    int freeCamFinger0Y;
    int freeCamFinger0Z;
    float flCameraRoll;
    void *activeTouchId;
    uint8_t field_0xc1;
    uint8_t field_0xc2;
    uint8_t field_0xc3;
    uint8_t dockChoiceOpen;
    uint8_t autopilotMenuOpen;
    uint8_t field_0xc6;
    uint8_t starMapOpen;
    uint16_t field_0xc8;
    uint8_t menuTouchOpen;
    int field_0xca;
    uint8_t touchesStream;
    uint8_t touchesStation;
    uint8_t jumpGateSoundStarted;
    uint8_t choiceWindowOpen;
    uint8_t field_0xcf;
    int choiceItemCount;
    uint8_t field_0xd4;
    uint8_t turretMode;
    uint8_t hudMenuOpen;
    int field_0xd8;
    uint8_t jumpDriveActive;
    uint8_t usingJumpDrive;
    uint8_t field_0xe0;
    int egoJumpPosX;
    int egoJumpPosY;
    int egoJumpPosZ;
    unsigned cameraId;
    TargetFollowCamera *camera;
    int hudTouchFlags;
    int lastTapTime;
    int lastTapTimeHigh;
    int lastAlignTime;
    int lastAlignTimeHigh;
    uint8_t field_0x110;
    uint8_t needsRedraw;
    AEGeometry *jumpFlash;
    float flShakeX;
    float flShakeY;
    int field_0x120;
    int dragLastX;
    int dragLastY;
    int dragRotIntX;
    int dragRotIntY;
    int dragDeltaX;
    int dragDeltaY;
    float flShakeAmpX;
    float flShakeAmpY;
    float flShakePhaseX;
    float flShakePhaseY;
    int field_0x150;
    int dragStartX;
    int dragStartY;
    uint8_t freeCamDragging;
    uint8_t menuOpen;
    uint8_t freeCamMode;
    int cinematicPrevCamMode;
    uint8_t cinematicPrevLookAt;
    int flFastForwardFactor;
    int timeWarpState;
    int maneuverHoldTime;
    uint8_t maneuverActive;
    int maneuverStartX;
    int maneuverStartY;
    int field_0x19c;
    int field_0x1a0;
    uint8_t pauseSnapshot;
    float flFastForwardWeight;
    int field_0x1ac;
    int cloakAttribute;
    int cloakAttributeMax;
    uint16_t thrustActive;
    uint8_t thrustEngaged;
    int field_0x1bc;
    int thrustStartY;
    int field_0x1c4;
    int thrustResetX;
    int thrustThreshold;
    int thrustBase;
    int field_0x1d4;
    int field_0x1d8;
    uint8_t field_0x1dd;
    int field_0x1e0;
    uint16_t field_0x1e4;
    uint8_t field_0x1e6;
    int gameRecord;
    Array<AbyssEngine::String *> *missionInfoLines;

    MGame();

    ~MGame() override;

    int OnInitialize() override;

    void OnRelease() override;

    long long OnKeyPress(long long key, long long mod) override;

    long long OnKeyRelease(long long key, long long mod) override;

    void OnTouchBegin(int p1, int p2) override;

    void OnTouchMove(int p1, int p2) override;

    void OnTouchEnd(int p1, int p2) override;

    void OnTouchBegin(int p1, int p2, void *touchId) override;

    void OnTouchMove(int p1, int y, void *touch) override;

    void OnTouchEnd(int p1, int p2, void *touchId) override;

    void OnUpdate() override;

    void OnRender3D() override;

    void OnRender2D() override;

    void OnSuspend() override;

    void OnResume() override;

    int ShowLoadingScreen() override;

    void showLiteScreen();

    void pause();

    void UseKhadorDrive();

    void dialogueEvent();

    void dockEvent(int p1, int p2);

    void freeCamTouchBegin(int x, int y, void *id);

    void freeCamTouchEnd(int p1, int p2, void *id);

    void freeCamTouchMove(int x, int y, void *touchId);

    void gameOverCheck();

    void handleAccelerometer();

    void maneuverTouchBegin(int x, int y, void *p);

    void maneuverTouchEnd(int a, int b, void *p);

    void maneuverTouchMove(int a, int b, void *p);

    int nextCamId(int cur);

    void pauseSounds();

    void reset();

    void resumeSounds();

    void setCinematicMode(bool on);

    void startChargingJumpDrive();

    void startJumpScene();

    void successCheck();

    void switchCamera(int id);

    void updateJumpScene();

    void useCloak();
};
#endif
