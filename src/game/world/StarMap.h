#ifndef GOF2_STARMAP_H
#define GOF2_STARMAP_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "SystemPathFinder.h"
#include "engine/core/AbyssEngine.h"
#include "engine/math/EaseInOut.h"
#include "engine/render/AEGeometry.h"
#include "game/ui/ChoiceWindow.h"
#include "game/ui/TouchButton.h"

#include "engine/math/Vector.h"

class SystemPathFinder;

class AEGeometry;
class ChoiceWindow;
class Mission;
class SolarSystem;
class Station;
class TouchButton;

namespace AbyssEngine {
    class EaseInOut;
}

class StarMap {
public:
    uint8_t exitRequested;
    uint8_t field_0x01;
    int32_t mode;
    float cameraBaseX;
    float cameraBaseZ;
    int32_t field_0x10;
    int32_t field_0x14;
    int32_t lastDt;
    uint32_t keyImageRetreat;
    uint32_t keyImageWanted;
    uint32_t keyImageMission;
    uint32_t keyImageCurrent;
    uint32_t keyImageDiscovered;
    uint32_t systemNameImage;
    uint32_t selectIcon;
    uint32_t cursorIcon;
    uint32_t currentMarkerIcon;
    TouchButton *backButton;
    SystemPathFinder *pathFinder;
    Array<SolarSystem *> *systems;
    Array<Station *> *stations;
    ChoiceWindow *choiceWindow;
    int32_t selectedSystem;
    int32_t selectedStation;
    Array<AEGeometry *> *systemGeoms;
    AEGeometry *systemRoot;
    uint32_t camera;
    uint32_t prevCamera;
    AbyssEngine::Vector scratchVector;
    AbyssEngine::Vector scratchVector2;
    Array<AEGeometry *> *stationGeoms;
    Array<AEGeometry *> *ringGeoms;
    int *stationAngles;
    int *stationDistances;
    Array<int> *systemPath;
    AEGeometry *starSystemRoot;
    uint8_t _pad_0x9c[12];
    uint8_t pad_0xa8_a;
    uint8_t choiceVisible;
    uint8_t jumpMapModeA;
    uint8_t jumpMapModeB;
    AbyssEngine::Vector field_0xac;
    uint8_t _pad_0xb8[4];
    AbyssEngine::Vector field_0xbc;
    uint8_t _pad_0xc8[4];
    AbyssEngine::Vector field_0xcc;
    uint8_t missionChangedFlag;
    uint8_t _pad_0xd9[12];
    int32_t field_0xe8;
    int32_t field_0xec;
    uint8_t isGalaxyMode;
    AEGeometry *markerGeom;
    int *iconBuffer;
    Array<bool> *usedFlags;
    int32_t targetSystem;
    uint8_t showKey;
    int32_t keyBoxWidth;
    int32_t keyBoxHeight;
    int32_t pulseSystem;
    uint8_t autoMode;
    int32_t autoTimer;
    uint8_t alienJumpPending;
    uint32_t raceImageNeutral;
    uint32_t raceImageA;
    uint32_t raceImageB;
    uint32_t raceImageDefault;
    uint8_t _pad_0x130[4];
    uint32_t image_0x134;
    uint8_t transitionIn;
    uint8_t transitionOut;
    uint8_t pathAnim;
    uint8_t stationCenterAnim;
    int32_t panX;
    int32_t panY;
    float touchStartX;
    float touchStartY;
    float field_0x14c;
    float touchDeltaX;
    float touchDeltaY;
    float field_0x158;
    float lastTouchX;
    float lastTouchY;
    int32_t field_0x164;
    float momentumFactor;
    float velocityX;
    float velocityY;
    uint8_t dragging;
    uint32_t planetTexture;
    AbyssEngine::EaseInOut *easeX;
    AbyssEngine::EaseInOut *easeY;
    AbyssEngine::EaseInOut *easeZ;
    float yaw;
    float pitch;
    int32_t field_0x190;
    Array<AbyssEngine::Vector *> *systemPositions;
    Array<AbyssEngine::Vector *> *stationPositions;
    int32_t lastSelectedSystem;
    int32_t lastSelectedStation;
    int32_t alpha;
    int32_t iconWidth;
    int32_t missionIconWidth;
    AEGeometry *bgLayer0;
    AEGeometry *bgLayer1;
    AEGeometry *bgLayer2;
    AEGeometry *planetGeom;
    float spin;
    int32_t centeredStation;
    int32_t hitRadius;
    float dragScale;
    int32_t jumpCost;
    uint8_t noRoute;
    int32_t cargoAmount;
    int32_t routeStart;
    int32_t routeTarget;
    uint8_t suppressNextClose;

    StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4);

    ~StarMap();

    int init(bool jumpMapMode, Mission *mission, bool param3, int param4);

    void initLights();

    void initStarSystem();

    void render();

    void renderBG();

    void draw();

    void drawKey();

    void drawOnScreenInfo(int index, bool stationMode);

    void update(int dt);

    uint32_t OnTouchBegin(int x, int y);

    void OnTouchMove(int x, int y);

    int OnTouchEnd(int x, int y);

    void depart(bool jump);

    void setStart(int start, int target);

    void setJumpMapMode(bool enabled, bool value);

    void askForJumpIntoAlienWorld();

    bool isInPlanetMode();

    uint8_t missionChanged();
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(StarMap, pad_0xa8_a) == 0xa8, "");
static_assert(__builtin_offsetof(StarMap, choiceVisible) == 0xa9, "");
static_assert(__builtin_offsetof(StarMap, field_0xac) == 0xac, "");
static_assert(__builtin_offsetof(StarMap, field_0xbc) == 0xbc, "");
static_assert(__builtin_offsetof(StarMap, field_0xcc) == 0xcc, "");
static_assert(__builtin_offsetof(StarMap, field_0xe8) == 0xe8, "");
static_assert(__builtin_offsetof(StarMap, field_0xec) == 0xec, "");
static_assert(__builtin_offsetof(StarMap, isGalaxyMode) == 0xf0, "");
static_assert(__builtin_offsetof(StarMap, image_0x134) == 0x134, "");
static_assert(__builtin_offsetof(StarMap, transitionIn) == 0x138, "");
static_assert(__builtin_offsetof(StarMap, field_0x14c) == 0x14c, "");
static_assert(__builtin_offsetof(StarMap, field_0x158) == 0x158, "");
static_assert(__builtin_offsetof(StarMap, field_0x164) == 0x164, "");
static_assert(__builtin_offsetof(StarMap, field_0x190) == 0x190, "");
static_assert(__builtin_offsetof(StarMap, systemPositions) == 0x194, "");
#endif
#endif
