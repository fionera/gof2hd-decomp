#ifndef GOF2_STARMAP_H
#define GOF2_STARMAP_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class AEGeometry;
class ChoiceWindow;
class Mission;
class SolarSystem;
class Station;
class SystemPathFinder;
class TouchButton;
namespace AbyssEngine { class EaseInOut; }

// StarMap — the top-level galaxy/system map screen. In galaxy mode it shows every solar
// system as a 3D icon the player can pan and pick; selecting a system zooms into a
// star-system view that orbits the stations around the central planet. Doubles as the
// jump-route planner: it can plot and animate a path between two systems and commit the
// player to departing/jumping.
class StarMap {
public:
    uint8_t  exitRequested;          // set to 1 to leave the screen (restore prev camera)
    uint8_t  field_0x01;             // init-only flag
    int32_t  mode;                   // 0 = galaxy map, 3 = star-system/planet view
    float    cameraBaseX;            // camera pan base X (screen/20)
    float    cameraBaseZ;            // camera pan base Z (screen/20)
    int32_t  field_0x10;             // (= 500)
    int32_t  field_0x14;             // (= 500)
    int32_t  lastDt;                 // last update() delta-time
    uint32_t keyImageRetreat;        // legend image: jump
    uint32_t keyImageWanted;         // legend image
    uint32_t keyImageMission;        // legend image
    uint32_t keyImageCurrent;        // legend image
    uint32_t keyImageDiscovered;     // legend image
    uint32_t systemNameImage;        // selected system name image
    uint32_t selectIcon;             // selection marker image
    uint32_t cursorIcon;             // cursor marker image
    uint32_t currentMarkerIcon;      // current-system marker
    TouchButton *backButton;
    SystemPathFinder *pathFinder;
    Array<SolarSystem *> *systems;   // Galaxy::getSystems()
    Array<Station *> *stations;      // current-system stations
    ChoiceWindow *choiceWindow;
    int32_t  selectedSystem;
    int32_t  selectedStation;
    Array<AEGeometry *> *systemGeoms;// per-system 3D icons
    AEGeometry *systemRoot;          // galaxy-view root geometry
    uint32_t camera;                 // star-map camera handle
    uint32_t prevCamera;             // camera to restore on exit
    Vector   scratchVector;          // scratch screen-space Vector
    Vector   scratchVector2;         // second scratch Vector
    Array<AEGeometry *> *stationGeoms;// per-station 3D icons (system view)
    Array<AEGeometry *> *ringGeoms;  // orbit-ring geometries
    int     *stationAngles;          // per-station orbit angle
    int     *stationDistances;       // per-station orbit radius
    Array<int> *systemPath;          // jump-route system index path
    AEGeometry *starSystemRoot;      // system-view root geometry
    uint8_t  pad_0xa8_a;             // jump-map mode flag
    uint8_t  choiceVisible;          // choice window shown
    uint8_t  jumpMapModeA;           // set by setJumpMapMode `enabled`
    uint8_t  jumpMapModeB;           // set by setJumpMapMode `value`
    Vector   field_0xac;
    Vector   field_0xbc;
    Vector   field_0xcc;
    uint8_t  missionChangedFlag;
    int32_t  field_0xe8;
    int32_t  field_0xec;
    uint8_t  isGalaxyMode;           // campaign > 0xf
    AEGeometry *markerGeom;          // optional jump-route overlay mesh
    int     *iconBuffer;             // scratch int[5] info-icon image ids
    Array<uint8_t> *usedFlags;       // per-station "used" flags
    int32_t  targetSystem;           // jump-map target system index
    uint8_t  showKey;                // legend visible
    int32_t  keyBoxWidth;
    int32_t  keyBoxHeight;
    int32_t  pulseSystem;            // auto-select system index
    uint8_t  autoMode;               // tutorial/auto
    int32_t  autoTimer;              // auto-select countdown (ms)
    uint8_t  alienJumpPending;
    uint32_t raceImageNeutral;
    uint32_t raceImageA;
    uint32_t raceImageB;
    uint32_t raceImageDefault;
    uint32_t image_0x134;
    uint8_t  transitionIn;           // zoom-into-system transition active
    uint8_t  transitionOut;          // zoom-out-to-galaxy transition active
    uint8_t  pathAnim;               // jump-route auto-pan active
    uint8_t  stationCenterAnim;      // station-centering animation active
    int32_t  panX;                   // galaxy pan offset X
    int32_t  panY;                   // galaxy pan offset Y
    float    touchStartX;
    float    touchStartY;
    float    field_0x14c;
    float    touchDeltaX;
    float    touchDeltaY;
    float    field_0x158;
    float    lastTouchX;
    float    lastTouchY;
    int32_t  field_0x164;
    float    momentumFactor;         // drag-momentum decay
    float    velocityX;
    float    velocityY;
    uint8_t  dragging;
    uint32_t planetTexture;
    AbyssEngine::EaseInOut *easeX;
    AbyssEngine::EaseInOut *easeY;
    AbyssEngine::EaseInOut *easeZ;
    float    yaw;                    // system-view yaw
    float    pitch;                  // system-view pitch
    int32_t  field_0x190;
    Array<Vector *> *systemPositions;// per-system screen positions
    Array<Vector *> *stationPositions;// per-station screen positions
    int32_t  lastSelectedSystem;
    int32_t  lastSelectedStation;
    int32_t  alpha;
    int32_t  iconWidth;
    int32_t  missionIconWidth;
    AEGeometry *bgLayer0;
    AEGeometry *bgLayer1;
    AEGeometry *bgLayer2;
    AEGeometry *planetGeom;          // system-view planet mesh
    float    spin;                   // star-icon spin accumulator
    int32_t  centeredStation;        // station index being centered (-1 none)
    int32_t  hitRadius;              // touch-pick radius
    float    dragScale;             // layout drag-scale factor
    int32_t  jumpCost;
    uint8_t  noRoute;
    int32_t  cargoAmount;
    int32_t  routeStart;
    int32_t  routeTarget;
    uint8_t  suppressNextClose;

    StarMap(bool jumpMapMode, Mission *mission, bool param3, int param4);
    ~StarMap();

    int init(bool jumpMapMode, Mission *mission, bool param3, int param4);
    void initLights();
    void initStarSystem();

    void render();
    void render_tail();              // draw the optional jump-route overlay geometry
    void renderBG();
    void draw();
    void drawKey();
    void drawOnScreenInfo(int index, bool stationMode);

    void update(int dt);

    uint32_t OnTouchBegin(int x, int y);
    void OnTouchMove(int x, int y);
    void OnTouchEnd(int x, int y);
    void OnTouchEnd_tail();          // open the help overlay
    int  touch_end(int x, int y);    // embedded (lounge) touch-release; nonzero => map dismissed

    void depart(bool jump);
    void depart_tail(void *app, int moduleId); // hand the application to module `moduleId`
    void setStart(int start, int target);
    void setJumpMapMode(bool enabled, bool value);
    void askForJumpIntoAlienWorld();

    bool isInPlanetMode();
    uint8_t missionChanged();
};
#endif
